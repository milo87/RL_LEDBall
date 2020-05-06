const int redLEDPin  = 9;
const int greenLEDPin = 10;
const int blueLEDPin = 11;
const int debugLEDPin = 12;

const char delim = ';';

enum State_enum { PULSE, TEAM, GOAL };

uint8_t state = PULSE;
uint8_t oldState;

int red   = 128;
int green = 128;
int blue  = 128;

int brightness = 0;
int fadeAmount = 5;

bool stateLocked = false;

void setup() {
  Serial.begin(9600);

  pinMode(redLEDPin,  OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(debugLEDPin, OUTPUT);
}

void loop() {
  // If we're in an uninterruptable state don't bother reading input
  //readSerial();
  switch (state) {
    case PULSE:
      pulse();
      break;
    case TEAM:
      showTeam();
      break;
    case GOAL:
      goalFlash();
      break;
    default:
      break;
  }
}

void serialEvent() {
  while (Serial.available()) {
    String command = Serial.readStringUntil(delim);

    if (command == "PULSE") {
      changeState(PULSE);
    }

    if (command == "TEAM") {
      red = Serial.parseInt();
      green = Serial.parseInt();
      blue = Serial.parseInt();

      if (Serial.read() == delim) {
        red = constrain(red, 0, 255);
        green = constrain(green, 0, 255);
        blue = constrain(blue, 0, 255);
      }
      
      changeState(TEAM);
    }

    if (command == "GOAL") {
      changeState(GOAL);
    }
  }
}

bool changeState(uint8_t newState) {
  if (stateLocked) return false;
  
  if(newState != oldState) {
    oldState = state;
    state = newState;
    return true;
  }

  return false;
}

// Events
void showTeam() {
  analogWrite(redLEDPin, red);
  analogWrite(greenLEDPin, green);
  analogWrite(blueLEDPin, blue);
}

void goalFlash() {
  stateLocked = true;
  for (int i = 0; i < 10; i++) {
    analogWrite(redLEDPin, 0);
    analogWrite(greenLEDPin, 0);
    analogWrite(blueLEDPin, 0);
    delay(100);
    analogWrite(redLEDPin, red);
    analogWrite(greenLEDPin, blue);
    analogWrite(blueLEDPin, green);
    delay(100);
  }
  stateLocked = false;
  changeState(TEAM);
}

void pulse() {
  analogWrite(redLEDPin, brightness);
  analogWrite(greenLEDPin, brightness);
  analogWrite(blueLEDPin, brightness);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(30);
}
