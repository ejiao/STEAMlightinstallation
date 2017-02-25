
#define TRIG 3 
#define ECHO 2 

#define RED 5 
#define BLUE 6 
#define GREEN 10 

int lastMode = 0; 

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
  long distance = duration*(0.034/2);
  if (distance < 50) { 
    if (lastMode == 0) { 
      lastMode = 1; 
      for (int i = 0; i < 200; i++) { 
        rgb(50 + i,20,20); 
        delay(5); 
      }
    } 
    else if (lastMode == 1) { 
      rgb(250,20,20); 
    }
  }
  else { 
    if (lastMode == 1) { 
       lastMode = 0; 
       for (int i = 200; i >= 0; i--) { 
        rgb(50 + i,20,20); 
        delay(3); 
      } 
    }
  }
  
  Serial.print("Distance: "); 
  Serial.println(distance); 
}

void discreteDistance(int distance){ 

  
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

void rgb(int r, int g, int b) { 
  analogWrite(RED, r); 
  analogWrite(GREEN, g); 
  analogWrite(BLUE, b); 
}

