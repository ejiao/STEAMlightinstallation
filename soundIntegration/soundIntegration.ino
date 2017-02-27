#define AUDIO_ANALOG A0

#define RED 5
#define BLUE 9
#define GREEN 12

int red;
int green;
int blue;

int audioReading;

bool breathing;

/**
 * sets up pins for I/O.
 */
void setup() {
  // put your setup code here, to run once:

  // LED's
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);

  // microphone
  pinMode(AUDIO, INPUT);

  Serial.begin(9600);

  red = 0;
  green = 0;
  blue = 2;

  breathing = true;
}

void loop() {

  audioReading = getMinimumAudioReading();

  if (audioReading < 500) {
    
    breathing = false;
  }
  
  // put your main code here, to run repeatedly:
  if (breathing) {
    if (blue == 60) {
      blue--;
    } else if (blue >= 2) {
      blue++;
    }
  }
}

int getMinimumAudioReading() {
  
  numTrials = 10.
  for (int i = 0; i<numTrials; i++) {
    
  }
}
