#include <NewPing.h>

#define ECHO1 22
#define TRIG1 24
#define ECHO2 44
#define TRIG2 46

#define RED1 3
#define BLUE1 4
#define GREEN1 2

#define RED2 5
#define BLUE2 6
#define GREEN2 7

int lastMode1 = 0;
int lastMode2 = 0;

static int OUT_OF_RANGE = 0,
           ONE_METER = 1,
           HALF_METER = 2,
           STRIP_ONE = 1,
           STRIP_TWO = 2;


void setup() {
  // put your setup code here, to run once:
  pinMode(RED1, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(BLUE2, OUTPUT);
  pinMode(GREEN2, OUTPUT);

  // ultrasonic
  unsigned int cm[2]; // Store ping distances.
  NewPing sonar(TRIG1, ECHO1, 150)
  NewPing sonar(TRIG2, ECHO2, 150)

  // ultrasonic
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  Serial.begin(9600);

  rgb(0, 0, 50, RED1, GREEN1, BLUE1);
  rgb(0, 0, 50, RED2, GREEN2, BLUE2);

}

void loop() {

  loopOnStrip(RED1, GREEN1, BLUE1, TRIG1, ECHO1, lastMode1, STRIP_ONE);
  loopOnStrip(RED2, GREEN2, BLUE2, TRIG2, ECHO2, lastMode2, STRIP_TWO);
  
}

void loopOnStrip(int pinRed, int pinGreen, int pinBlue, int pinTrig, int pinEcho, int lastMode,int whichStrip) {
  long minDuration = 2000000.;
  long trials = 5.;
  for (int i = 0; i < trials; i++) {
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(20);

    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(100);
    digitalWrite(pinTrig, LOW);

    long tempDuration = pulseIn(pinEcho, HIGH);
    if (tempDuration < minDuration) {
      minDuration = tempDuration;
    }
  }

  long distance = durationToDistance(minDuration);

  if (whichStrip == STRIP_TWO) {
    Serial.println("Strip two distance: ");
    Serial.println(distance);
  } else {
    Serial.println("Strip one distance: ");
    Serial.println(distance);
  }

  if (distance < 150 && distance >= 100) {
    if (lastMode == ONE_METER) {
      //do nothing
    }
    else if (lastMode == OUT_OF_RANGE) {
      if (whichStrip == STRIP_ONE) {
        lastMode1 = ONE_METER;
      } else if (whichStrip == STRIP_TWO) {
        lastMode2 = ONE_METER;
      }
      fadeColor(0, 0, 50, 50, 0, 50, 3, pinRed, pinGreen, pinBlue);
    }
    else if (lastMode == HALF_METER) {
      if (whichStrip == STRIP_ONE) {
        lastMode1 = ONE_METER;
      } else if (whichStrip == STRIP_TWO) {
        lastMode2 = ONE_METER;
      }
      fadeColor(250, 0, 50, 50, 0, 50, 3, pinRed, pinGreen, pinBlue);
    }
  } else if (distance < 100) {
    if (lastMode == HALF_METER) {
      //do nothing
    }
    else if (lastMode == ONE_METER) {
      if (whichStrip == STRIP_ONE) {
        lastMode1 = HALF_METER;
      } else if (whichStrip == STRIP_TWO) {
        lastMode2 = HALF_METER;
      }
      fadeColor(50, 0, 50, 250, 0, 50, 3, pinRed, pinGreen, pinBlue);
    }
    else if (lastMode == OUT_OF_RANGE) {
      if (whichStrip == STRIP_ONE) {
        lastMode1 = HALF_METER;
      } else if (whichStrip == STRIP_TWO) {
        lastMode2 = HALF_METER;
      }
      fadeColor(0, 0, 50, 250, 0, 50, 3, pinRed, pinGreen, pinBlue);
    }
  } else {
    if (lastMode == OUT_OF_RANGE) {
      //do nothing
    }
    else if (lastMode == HALF_METER) {
      if (whichStrip == STRIP_ONE) {
        lastMode1 = OUT_OF_RANGE;
      } else if (whichStrip == STRIP_TWO) {
        lastMode2 = OUT_OF_RANGE;
      }
      fadeColor(250, 0, 50, 0, 0, 50, 3, pinRed, pinGreen, pinBlue);
    } else if (lastMode == ONE_METER) {
      if (whichStrip == STRIP_ONE) {
        lastMode1 = OUT_OF_RANGE;
      } else if (whichStrip == STRIP_TWO) {
        lastMode2 = OUT_OF_RANGE;
      }
      fadeColor(50, 0, 50, 0, 0, 50, 3, pinRed, pinGreen, pinBlue);
    }
  }

  //  Serial.print("Distance: ");
  //  Serial.println(distance);
}

long durationToDistance(long duration) {
  return duration * (0.034 / 2);
}

//void logDistance(int distance) {
//  if(distance < 50) {
//     int brightness = 250 - (log(distance * 5) * (250/5.51));
//     rgb(brightness, 0, brightness);
//  } else {
//     rgb(0, 0, 0);
//  }
//}

//void simpleBlink() {
//  digitalWrite(RED, HIGH);
//  delay(500);
//  digitalWrite(BLUE, HIGH);
//  delay(500);
//  digitalWrite(GREEN, HIGH);
//  delay(500);
//  digitalWrite(RED, LOW);
//  delay(500);
//  digitalWrite(BLUE, LOW);
//  delay(500);
//  digitalWrite(GREEN, LOW);
//  delay(500);
//}

//void fastBlink() {
//  int flash = 250;
//  digitalWrite(RED, HIGH);
//  delay(flash);
//  digitalWrite(BLUE, HIGH);
//  delay(flash);
//  digitalWrite(GREEN, HIGH);
//  delay(flash);
//  digitalWrite(RED, LOW);
//  delay(flash);
//  digitalWrite(BLUE, LOW);
//  delay(flash);
//  digitalWrite(GREEN, LOW);
//  delay(flash);
//}

//void fadeBlue() {
//  for (int r = 0; r < 256; r++) {
//    delay(5);
//    analogWrite(BLUE, r);
//  }
//  // delay(100);
//  for (int r = 256; r >= 0; r--) {
//    analogWrite(BLUE, r);
//    delay(5);
//  }
//}
//
//void fadeRed() {
//  for (int r = 0; r < 256; r++) {
//    analogWrite(RED1, r);
//    delay(5);
//  }
//  for (int r = 256; r >= 0; r--) {
//    analogWrite(RED1, r);
//    delay(5);
//  }
//}
//
//void fadeGreen() {
//  for (int r = 0; r < 250; r++) {
//    analogWrite(GREEN1, r);
//    delay(5);
//  }
//  for (int r = 250; r >= 0; r--) {
//    analogWrite(GREEN1, r);
//    delay(5);
//  }
//}

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
    rgb(newR, newG, newB, pinRed, pinGreen, pinBlue);
    delay(pause);
  }
}

void rgb(int r, int g, int b, int pinRed, int pinGreen, int pinBlue) {
  analogWrite(pinRed, r);
  analogWrite(pinGreen, g);
  analogWrite(pinBlue, b);
}

