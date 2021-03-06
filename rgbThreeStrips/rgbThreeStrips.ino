#define ECHO1 22
#define TRIG1 24
#define ECHO2 34
#define TRIG2 36

#define RED1 4
#define BLUE1 5
#define GREEN1 3

#define RED2 8
#define BLUE2 7
#define GREEN2 6

#define RED3 10
#define BLUE3 11
#define GREEN3 9

int lastMode1 = 0;
int lastMode2 = 0;

static int FAR = 0,
           MEDIUM = 1,
           CLOSE = 2,
           STRIP_ONE = 1,
           STRIP_TWO = 2, 
           STRIP_THREE = 3; 

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  rgb(200, 0, 0, RED1, GREEN1, BLUE1); 
  rgb(0, 200, 0, RED2, GREEN2, BLUE2); 
  rgb(0, 0, 200, RED3, GREEN3, BLUE3); 

  delay(200); 
  rgb(0, 200, 0, RED1, GREEN1, BLUE1); 
  rgb(0, 0, 200, RED2, GREEN2, BLUE2); 
  rgb(200, 0, 0, RED3, GREEN3, BLUE3); 
  delay(200); 
  rgb(0, 0, 200, RED1, GREEN1, BLUE1); 
  rgb(200, 0, 0, RED2, GREEN2, BLUE2); 
  rgb(0, 200, 0, RED3, GREEN3, BLUE3); 
  delay(200);
}

void rgb(int r, int g, int b, int pinRed, int pinGreen, int pinBlue) {
  analogWrite(pinRed, r);
  analogWrite(pinGreen, g);
  analogWrite(pinBlue, b);
}
