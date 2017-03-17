#define ECHO1 22
#define TRIG1 24

#define ECHO2 30
#define TRIG2 32

#define ECHO3 40
#define TRIG3 42

#define AUDIO A0

#define RED1 3
#define BLUE1 4
#define GREEN1 2

#define RED2 7
#define BLUE2 6
#define GREEN2 5

#define RED3 9
#define BLUE3 10
#define GREEN3 8

int lastMode1 = 0;
int lastMode2 = 0;

static int FAR = 0,
           MEDIUM = 1,
           CLOSE = 2,
           STRIP_ONE = 1,
           STRIP_TWO = 2, 
           STRIP_THREE = 3,
           R = 0, G = 1, B = 2; 

int oldStrip1[3], oldStrip2[3], oldStrip3[3],
    newStrip1[3], newStrip2[3], newStrip3[3];


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

  // initialize 
  oldStrip1[R] = 200;
  oldStrip2[G] = 200;
  oldStrip3[B] = 200;

  // apply old state
  rgb(oldStrip1, RED1, GREEN1, BLUE1);
  rgb(oldStrip2, RED2, GREEN2, BLUE2);
  rgb(oldStrip3, RED3, GREEN3, BLUE3);
  
}

void loop() {
  // get distances from each ultrasonic sensor
  long distance1 = ultrasonic(TRIG1, ECHO1); 
  long distance2 = ultrasonic(TRIG2, ECHO2); 
  long distance3 = ultrasonic(TRIG3, ECHO3); 
  Serial.print("Strip one distance: "); 
  Serial.println(distance1); 
  Serial.print("Strip two distance: "); 
  Serial.println(distance2); 
  Serial.print("Strip three distance: ");
  Serial.println(distance3);
}

// Does the ultrasonic stuff
// Modifies global "distances"  
long ultrasonic(int trigPin, int echoPin) { 
  long maxDuration = 0.;
  
  int trials = 5; 
  for (int i = 0; i < trials; i++) { 
      digitalWrite(trigPin, LOW);
    
      delayMicroseconds(20); 
      
      digitalWrite(trigPin, HIGH); 
    
      delayMicroseconds(100);
    
      long tempDuration = pulseIn(echoPin, HIGH);

      if (tempDuration > maxDuration) {
        maxDuration = tempDuration;
      }
  }
  return durationToDistance(maxDuration);
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
    rgb({newR, newB, newG}, pinRed, pinGreen, pinBlue);
    delay(pause);
  }
}
