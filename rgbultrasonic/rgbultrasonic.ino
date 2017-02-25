
#define TRIG 3 
#define ECHO 2 

#define RED 5 
#define BLUE 6 
#define GREEN 10 

int lastMode = 0; 
static int OUT_OF_RANGE = 0, 
                ONE_METER = 1,
                HALF_METER = 2;


void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT); 
  pinMode(BLUE, OUTPUT); 
  pinMode(GREEN, OUTPUT); 

  // ultrasonic 
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 

  Serial.begin(9600); 

  rgb(50,20,20); 

}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(20);

  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(100); 
  digitalWrite(TRIG, LOW); 
  
  long duration = pulseIn(ECHO, HIGH); 
  long distance = durationToDistance(duration);
  if (distance < 100 && distance > 50) { 
    if (lastMode == OUT_OF_RANGE) { 
      lastMode = ONE_METER = 1; 
      fadeColor(0, 0, 0, 125, 125, 125, 5);
    } 
    else if (lastMode == ONE_METER) { 
      rgb(125,125,125);
    }
  } else if (distance < 50) {
    if (lastMode == ONE_METER) { 
      lastMode = HALF_METER; 
      fadeColor(125, 125, 125, 250, 250, 250, 5);
    }
    else if (lastMode == HALF_METER) {
      rgb(250, 250, 250);
    }
  }
  else { 
    if (lastMode == HALF_METER) { 
       lastMode = OUT_OF_RANGE;
       fadeColor(250, 250, 250, 0, 0, 0, 5);
    } else if (lastMode == ONE_METER) {
       lastMode = OUT_OF_RANGE;
       fadeColor(125, 125, 125, 0, 0, 0, 5);
    }
  }
  
  Serial.print("Distance: "); 
  Serial.println(distance); 
}

double durationToDistance(int duration){ 
  return duration*(0.034/2);
}

//void logDistance(int distance) { 
//  if(distance < 50) { 
//     int brightness = 250 - (log(distance * 5) * (250/5.51)); 
//     rgb(brightness, 0, brightness); 
//  } else { 
//     rgb(0, 0, 0); 
//  }
//}

void simpleBlink() { 
  digitalWrite(RED, HIGH); 
  delay(500); 
  digitalWrite(BLUE, HIGH); 
  delay(500); 
  digitalWrite(GREEN, HIGH); 
  delay(500); 
  digitalWrite(RED, LOW); 
  delay(500); 
  digitalWrite(BLUE, LOW); 
  delay(500); 
  digitalWrite(GREEN, LOW); 
  delay(500); 
}

void fastBlink() { 
  int flash = 250; 
  digitalWrite(RED, HIGH); 
  delay(flash); 
  digitalWrite(BLUE, HIGH); 
  delay(flash); 
  digitalWrite(GREEN, HIGH); 
  delay(flash); 
  digitalWrite(RED, LOW); 
  delay(flash); 
  digitalWrite(BLUE, LOW); 
  delay(flash); 
  digitalWrite(GREEN, LOW); 
  delay(flash); 
}

void fadeBlue() { 
 for (int r = 0; r < 256; r++) { 
    delay(5);
    analogWrite(BLUE, r);
  } 
  // delay(100); 
  for (int r = 256; r >= 0; r--) { 
    analogWrite(BLUE, r);
    delay(5);
  } 
}

void fadeRed() { 
 for (int r = 0; r < 256; r++) { 
    analogWrite(RED, r);
    delay(5);
  } 
  for (int r = 256; r >= 0; r--) { 
    analogWrite(RED, r);
    delay(5);
  } 
}

void fadeGreen() {
  for (int r = 0; r < 250; r++) { 
    analogWrite(GREEN, r);
    delay(5);
  } 
  for (int r = 250; r >= 0; r--) { 
    analogWrite(GREEN, r);
    delay(5);
  } 
}

void fadeColor(int startR, int startG, 
            int startB, int endR, 
            int endG, int endB, int pause) { 
  double incR = (double) (endR - startR)/ (double) 255 ;
  double incB = (double) (endB - startB)/ (double) 255 ;
  double incG = (double) (endG - startG)/ (double) 255 ; 
  for (int i = 0; i < 255; i++) { 
        int newR = startR + (i * incR); 
        int newB = startB + (i * incB); 
        int newG = startG + (i * incG); 
        rgb(newR, newB, newG);
        delay(pause);
  }
}

void rgb(int r, int g, int b) { 
  analogWrite(RED, r); 
  analogWrite(GREEN, g); 
  analogWrite(BLUE, b); 
}

