/* May 11, 2017 
 * This is the final code for strips that require a reduced light output 
 * (Used specifically for Mark II, which has the highest light density) 
 */


// pin definitions

#define ECHO1 23
#define TRIG1 22

#define ECHO2 25
#define TRIG2 24

#define ECHO3 27
#define TRIG3 26

#define AUDIO 20

#define RED1 4
#define BLUE1 5
#define GREEN1 3

#define RED2 8
#define BLUE2 7
#define GREEN2 6

#define RED3 10
#define BLUE3 11
#define GREEN3 9


/********************************************************************************/
/************************************CONSTANTS***********************************/
/********************************************************************************/

// codes
static int FAR = 0,                   // distance codes
           MEDIUM = 1,
           CLOSE = 2,
           STRIP_1 = 0,             // strip codes
           STRIP_2 = 1, 
           STRIP_3 = 2,
           R = 0, G = 1, B = 2,       // indices of color values in strip arrays
           MAX_BRIGHTNESS = 2,        // index of max brightness value
           MED_BRIGHTNESS = 1,        // index of medium brightness value
           IDLE_BRIGHTNESS = 0;       // index of idle brightness value

// constants
static int FAR_DISTANCE = 50,         // distance specified by FAR, in cm
           MEDIUM_DISTANCE = 20,      // distance specified by MEDIUM, in cm
           CLOSE_DISTANCE = 0,        // distance specified by CLOSE, in cm
           REFRESH_TIME = 30,          // number of millis between render updates
           PING_FREQ = 10;            // number of frames between pings
 
static int colorBrightnesses[3][3] = {{0, 20, 50}, // red
                                    {0, 20, 50}, // green
                                    {0, 20, 50}}; // blue

// personality codes
static const int THE_OG = 0;

/********************************************************************************/
/**************************************STATE*************************************/
/********************************************************************************/

// global state
int strips[3][3];
unsigned long timeOfLastPing;
unsigned long timeOfLastAudioInterrupt;
unsigned long lastInterrupt;
int distance1, distance2, distance3;

// function pointer for personality
void (* updateStripState)(int stripCode,  // STRIP_ONE, STRIP_TWO, or STRIP_THREE
                          int strips[][3],    // strip to modify
                          int distance);  // distance to respond to


/********************************************************************************/
/**********************************BASE METHODS**********************************/
/********************************************************************************/

void setup() {
  // setup the personality
  setupPersonality(THE_OG);
  
  // initialize pin modes
  pinMode(RED1, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(BLUE2, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(RED3, OUTPUT);
  pinMode(BLUE3, OUTPUT);
  pinMode(GREEN3, OUTPUT);

  // ultrasonic
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);

  Serial.begin(9600);

  // add interrupt for audio
  attachInterrupt(digitalPinToInterrupt(AUDIO), readAudio, RISING);

  // initialize strips
  for (int i = 0; i < 3; i++) {
    strips[i][R] = 0;
    strips[i][G] = 0;
    strips[i][B] = colorBrightnesses[B][IDLE_BRIGHTNESS];
  }
  
  rgb(strips, STRIP_1, RED1, GREEN1, BLUE1);
  rgb(strips, STRIP_2, RED2, GREEN2, BLUE2);
  rgb(strips, STRIP_3, RED3, GREEN3, BLUE3);

  // set the time
  timeOfLastPing = millis();
}

void loop() {
  if (millis() - timeOfLastPing > REFRESH_TIME * PING_FREQ) {
    // get distances from each ultrasonic sensor
    distance1 = ultrasonic(TRIG1, ECHO1);
    distance2 = ultrasonic(TRIG2, ECHO2);
    distance3 = ultrasonic(TRIG3, ECHO3);
//    Serial.print("Strip one distance: ");
//    Serial.println(distance1);
//    Serial.print("Strip two distance: ");
//    Serial.println(distance2);
//    Serial.print("Strip three distance: ");
//    Serial.println(distance3);
    timeOfLastPing = millis();
  }

  updateStripStates(distance1, distance2, distance3);

  // apply new state
  rgb(strips, STRIP_1, RED1, GREEN1, BLUE1);
  rgb(strips, STRIP_2, RED2, GREEN2, BLUE2);
  rgb(strips, STRIP_3, RED3, GREEN3, BLUE3);
  
  delay(REFRESH_TIME);

}

// updates all strip states
void updateStripStates(int d1, int d2, int d3) {
  updateStripState(STRIP_1, strips, chunkDistance(d1));
  updateStripState(STRIP_2, strips, chunkDistance(d2));
  updateStripState(STRIP_3, strips, chunkDistance(d3));
}

// Decides how close something is to a sensor in terms
// of three categories (FAR, MEDIUM, and CLOSE).
int chunkDistance(long d) {
  if (d > FAR_DISTANCE) {
    return FAR;
  } else if (d > MEDIUM_DISTANCE) {
    return MEDIUM;
  } else {
    return CLOSE;
  }
}

// Takes a sample of 3 ultrasonic pings and returns the 
// minimum distance of the three.
long ultrasonic(int trigPin, int echoPin) {
  long minDuration = 50000.;
  int trials = 3;
  for (int i = 0; i < trials; i++) {
    delayMicroseconds(2);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(100);

    long tempDuration = pulseIn(echoPin, HIGH);

    if (tempDuration < minDuration) {
      minDuration = tempDuration;
    }
  }
  delayMicroseconds(200);
  return durationToDistance(minDuration);
}

// Helper method to convert duration in 
long durationToDistance(long duration) {
  return duration * (0.0344 / 2.0);
}

// Function called on audio interrupt.
void readAudio() {
  timeOfLastAudioInterrupt = millis();
}

// Writes a red/green/blue combination to a strip.
void rgb(int strips[][3], int stripIndex, int pinRed,
          int pinGreen, int pinBlue) {
  analogWrite(pinRed, strips[stripIndex][R]);
  analogWrite(pinGreen, strips[stripIndex][G]);
  analogWrite(pinBlue, strips[stripIndex][B]);
}


/********************************************************************************/
/*******************************PERSONALITY METHODS******************************/
/********************************************************************************/

// constants for THE_OG personality
static int theOG_increment = 1,
  theOG_numFramesRise = 7,
  theOG_numFramesFall = 30,
  theOG_greenResponsePosSlope = colorBrightnesses[G][MAX_BRIGHTNESS] / theOG_numFramesRise,
  theOG_greenResponseNegSlope = - colorBrightnesses[G][MAX_BRIGHTNESS] / theOG_numFramesFall,
  theOG_greenResponseIntercept = - theOG_greenResponseNegSlope * (theOG_numFramesRise + theOG_numFramesFall);
// Updates state according to THE_OG personality.
// distance is a CODE, not an actual distance value.
void updateStripStateTheOG(int stripCode, int strips[][3], int chunkedDistance) {
  // deal with audio
  // get the number of frames since the last audio trigger
  unsigned int frames = (millis() - timeOfLastAudioInterrupt) / REFRESH_TIME;
  if (frames <= 6) {
    strips[stripCode][G] = theOG_greenResponsePosSlope * frames;
  } else if (frames <= 30) {
    strips[stripCode][G] = theOG_greenResponseNegSlope * frames + theOG_greenResponseIntercept;
  } else {
    strips[stripCode][G] = 0;
  }
  // deal with distance
  if (chunkedDistance == FAR && strips[stripCode][R] > colorBrightnesses[R][IDLE_BRIGHTNESS]) {
    strips[stripCode][R] -= theOG_increment;
  }
  else if (chunkedDistance == MEDIUM) {
    if (strips[stripCode][R] < colorBrightnesses[R][MED_BRIGHTNESS]) {
      strips[stripCode][R] += theOG_increment;
    }
    else if (strips[stripCode][R] > colorBrightnesses[R][MED_BRIGHTNESS]) {
      strips[stripCode][R] -= theOG_increment;
    }
  }
  else if (chunkedDistance == CLOSE && strips[stripCode][R] < colorBrightnesses[R][MAX_BRIGHTNESS] - theOG_increment) {
    strips[stripCode][R] += theOG_increment;
  }
}

/********************************************************************************/
/********************************PERSONALITY SETUP*******************************/
/********************************************************************************/

// sets up the global updateStripState function pointer
void setupPersonality(int personalityType) {
  switch (personalityType) {
    case THE_OG:
      updateStripState = updateStripStateTheOG;
      break;
    default:
      updateStripState = updateStripStateTheOG;
  }
}

