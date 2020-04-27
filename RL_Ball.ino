const int redLEDPin  = 9;
const int greenLEDPin = 10;
const int blueLEDPin = 11;

int red = 128;
int green = 128;
int blue = 128;

enum MODE {
  PULSE,
  SHOW_TEAM
};

MODE mode = PULSE;

void setup() {
  Serial.begin(9600);

  pinMode(redLEDPin,  OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();

    if (Serial.read() == '\n') {
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);
    }
  }

  analogWrite(redLEDPin, red);
  analogWrite(greenLEDPin, green);
  analogWrite(blueLEDPin, blue);
}
