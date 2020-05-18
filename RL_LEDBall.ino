#include <Adafruit_NeoPixel.h>

const int dataPin = 3;
const int ledCount = 5;
const char delim = ';';

enum State_enum { PULSE, TEAM, GOAL };

uint8_t state = PULSE;
uint8_t oldState;

int red   = 128;
int green = 0;
int blue  = 0;

Adafruit_NeoPixel strip(ledCount, dataPin, NEO_RGB + NEO_KHZ800);

int brightness = 0;
int fadeAmount = 5;

bool stateLocked = false;

void setup() {
  Serial.begin(9600);

  pinMode(dataPin,  OUTPUT);
  strip.begin();
  strip.show();
}

void loop() {
  if (stateLocked) return;
  
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

    if (command == "CONNECT") {
      Serial.write("RL_BALL_CONNECTED");
      return;
    }

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

  oldState = state;
  state = newState;
  
  return true;
}

void setLEDs(int r, int g, int b) {
  uint32_t colour = strip.Color(r, g, b);
  strip.fill(colour, 0);
  strip.show();
}

void showTeam() {
  setLEDs(red, green, blue);
}

void goalFlash() {
  stateLocked = true;
  for (int i = 0; i < 10; i++) {
    setLEDs(0, 0, 0);
    delay(100);
    setLEDs(red, green, blue);
    delay(100);
  }
  stateLocked = false;
  
  changeState(oldState);
}

void pulse() {
  setLEDs(brightness, brightness, brightness);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(50);
}
