
#define RED 5 
#define BLUE 6 
#define GREEN 10 

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT); 
  pinMode(BLUE, OUTPUT); 
  pinMode(GREEN, OUTPUT); 

}

void loop() {
  // simpleBlink(); 
  // fastBlink(); 
  fadeBlue(); 
  fadeRed(); 
  fadeGreen(); 
}

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

/* 
 *  
 *  
 */
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


