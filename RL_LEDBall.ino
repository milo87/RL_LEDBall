const int redLEDPin  = 9;
const int greenLEDPin = 10;
const int blueLEDPin = 11;

int red   = 128;
int green = 128;
int blue  = 128;

int brightness = 0;
int fadeAmount = 5;

bool isPulsing = true;

void ShowTeam(int r, int g, int b) {
  red = constrain(r, 0, 255);
  green = constrain(g, 0, 255);
  blue = constrain(b, 0, 255);
  analogWrite(redLEDPin, red);
  analogWrite(greenLEDPin, green);
  analogWrite(blueLEDPin, blue);
}

void GoalFlash() {
  for (int i = 0; i < 10; i++) {
    analogWrite(redLEDPin, 0);
    analogWrite(greenLEDPin, 0);
    analogWrite(blueLEDPin, 0);
    delay(500);
    analogWrite(redLEDPin, red);
    analogWrite(greenLEDPin, blue);
    analogWrite(blueLEDPin, green);
    delay(50);
  }
}

void Pulse() {
  analogWrite(redLEDPin, brightness);
  analogWrite(greenLEDPin, brightness);
  analogWrite(blueLEDPin, brightness);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(30);
}

void setup() {
  Serial.begin(9600);

  pinMode(redLEDPin,  OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(' ');

    if (command == "PULSE") {
      isPulsing = true;
    }

    if (command == "TEAM") {
      isPulsing = false;
      int new_r = Serial.parseInt();
      int new_g = Serial.parseInt();
      int new_b = Serial.parseInt();

      if (Serial.read() == 'N') {
        ShowTeam(new_r, new_g, new_b);
      }
    }
  }
  if(isPulsing) {
    Pulse();
  }
}
