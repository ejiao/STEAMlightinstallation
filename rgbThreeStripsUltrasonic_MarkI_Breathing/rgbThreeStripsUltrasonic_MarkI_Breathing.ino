#define ECHO1 40
#define TRIG1 42

#define ECHO2 22
#define TRIG2 24

#define ECHO3 30
#define TRIG3 32

#define AUDIO 18

#define RED1 3
#define GREEN1 2
#define BLUE1 4

#define RED2 7
#define BLUE2 6
#define GREEN2 5

#define RED3 9
#define BLUE3 10
#define GREEN3 8

static int FAR = 0,
           MEDIUM = 1,
           CLOSE = 2,
           STRIP_ONE = 1,
           STRIP_TWO = 2,
           STRIP_THREE = 3,
           R = 0, G = 1, B = 2,
           MAX_BRIGHTNESS = 255,
           MED_BRIGHTNESS = 180,
           IDLE_BRIGHTNESS = 120,
           LOW_BRIGHTNESS = 50, 
           FRAME_RATE = 10;
           
int strip1[3], strip2[3], strip3[3];
int count = 0; // timer 
int breathing_down = false; // exhale mode (decreasing)
int breathing_up = true; // inhale mode (increasing) 
long distance1 = 151, distance2 = 151, distance3 = 151;
unsigned long lastInterrupt;

void setup() {
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

  // initialize
  strip1[R] = 0;
  strip1[G] = 0;
  strip1[B] = IDLE_BRIGHTNESS;

  strip2[R] = 0;
  strip2[G] = 0;
  strip2[B] = IDLE_BRIGHTNESS;

  strip3[R] = 0;
  strip3[G] = 0;
  strip3[B] = IDLE_BRIGHTNESS;


  // apply old state
  rgb(strip1, RED1, GREEN1, BLUE1);
  rgb(strip2, RED2, GREEN2, BLUE2);
  rgb(strip3, RED3, GREEN3, BLUE3);
}

void loop() {
  if (count == 10) {
    // get distances from each ultrasonic sensor
    //distance1 = ultrasonic(TRIG1, ECHO1);
    distance2 = ultrasonic(TRIG2, ECHO2);
    distance3 = ultrasonic(TRIG3, ECHO3);
    //Serial.print("Strip one distance: ");
    //Serial.println(distance1);
    Serial.print("Strip two distance: ");
    Serial.println(distance2);
    Serial.print("Strip three distance: ");
    Serial.println(distance3);
    count = 0;
  }

  updateStripStates(distance1, distance2, distance3);

  // apply new state
  rgb(strip1, RED1, GREEN1, BLUE1);
  rgb(strip2, RED2, GREEN2, BLUE2);
  rgb(strip3, RED3, GREEN3, BLUE3);

  //  Serial.print("Strip 3 Red: ");
  //  Serial.println(strip3[R]);

  delay(FRAME_RATE);

  count++;

}

// updates all strip states
// depending on distance (red) and breathing (blue) 
void updateStripStates(int d1, int d2, int d3) {
  updateParticularStrip(d1, R, strip1, 3);
  updateBreath(B, strip1, 1); 
  updateParticularStrip(d2, R, strip2, 3);
  updateBreath(B, strip2, 1); 
  updateParticularStrip(d3, R, strip3, 3);
  updateBreath(B, strip3, 1); 
}

// fades a color between idle and low brightness 
void updateBreath(int color, int strip[], float inc) {
  if (count % 2 == 0) { // only update every other loop (slows down breath)
    if(breathing_up) { 
      if (strip[color] == IDLE_BRIGHTNESS){ 
        breathing_up = false; 
        breathing_down = true; 
      }
      else { 
        strip[color] += inc;
      } 
    }
    if(breathing_down) { 
      if (strip[color] == LOW_BRIGHTNESS){ 
        breathing_up = true; 
        breathing_down = false; 
      }
      else { 
        strip[color] -= inc; 
      }
    }
  }
}

// updates specific strip state
void updateParticularStrip(int d, int color, int strip[], int inc) {
  if (chunkDistance(d) == FAR) {
    if (strip[color] > 0) {
      strip[color] -= inc;
    }
    else {
      // do nothing
    }
    //    Serial.println("REACHED FAR");
  }
  else if (chunkDistance(d) == MEDIUM) {
    if (strip[color] < MED_BRIGHTNESS) {
      strip[color] += inc;
    }
    else if (strip1[color] > MED_BRIGHTNESS) {
      strip[color] -= inc;
    }
    else {
      // do nothing
    }
  }
  else if (chunkDistance(d) == CLOSE) {
    if (strip[color] < MAX_BRIGHTNESS - inc) {
      strip[color] += inc;
    }
    else {
      // do nothing
    }
  }
}

// decide how close something is to a sensor
int chunkDistance(long d) { // zero distance may = far
  if (d > 55) {
    return FAR;
  } else if (d > 30) {
    return MEDIUM;
  } else {
    return CLOSE;
  }
}

// Does the ultrasonic stuff
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
  //  Serial.println(durationToDistance(minDuration));
  return durationToDistance(minDuration);
}

// does the audio stuff
void readAudio() {
  if (millis() - lastInterrupt > 10) {
    for (int i = 0; i < MAX_BRIGHTNESS; i++) {
      analogWrite(GREEN1, i);
      analogWrite(GREEN2, i);
      analogWrite(GREEN3, i);
      delayMicroseconds(250);
    }
    for (int i = MAX_BRIGHTNESS; i >= 0; i--) {
      analogWrite(GREEN1, i);
      analogWrite(GREEN2, i);
      analogWrite(GREEN3, i);
      delayMicroseconds(2500);
    } 
    lastInterrupt = millis();
  }
}

long durationToDistance(long duration) {
  return duration * (0.0344 / 2.0);
}

void rgb(int strip[], int pinRed, int pinGreen, int pinBlue) {
  analogWrite(pinRed, strip[R]);
  analogWrite(pinGreen, strip[G]);
  analogWrite(pinBlue, strip[B]);
}

void fadeColor(int startR, int startG,
               int startB, int endR,
               int endG, int endB, int pause, int pinRed, int pinGreen, int pinBlue) {
  double incR = (double) (endR - startR) / (double) 255 ;
  double incB = (double) (endB - startB) / (double) 255 ;
  double incG = (double) (endG - startG) / (double) 255 ;
  for (int i = 0; i < 255; i++) {
    int newR = startR + (i * incR);
    int newB = startB + (i * incB);
    int newG = startG + (i * incG);
    int newRGB[] = {newR, newB, newG};
    rgb(newRGB, pinRed, pinGreen, pinBlue);
    rgb(newRGB, RED2, GREEN2, BLUE2);
    rgb(newRGB, RED3, GREEN3, BLUE3);
    delay(pause);
  }
}
