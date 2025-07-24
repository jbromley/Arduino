// -*- arduino-cli-default-fqbn: "lgt8fx:avr:328"; arduino-cli-default-port: "/dev/ttyUSB0"; -*-
/*
  ClockSimulator

  Simulates three quadrature signals and a single clock pendulum signal.

  Current state is just set up to test output connections.
  TODO: Debounce input buttons and light an LED when a button is pressed (input test)
*/
#include <TimerOne.h>

static uint8_t quadPins[6] = {D2, D3, D4, D5, D6, D7};
static uint8_t numQuadPins = 6;

static uint8_t pendPin = D8;

static uint8_t buttonPins[4] = {D10, D11, D12, D13};
static uint8_t numButtonPins = 4;
static int buttonState[4] = {HIGH};
static int lastButtonState[4] = {HIGH};
static unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long DEBOUNCE_DELAY = 50; // milliseconds debounce

void checkButtons() {
  for (uint8_t i = 0; i < numButtonPins; i ++) {
    int reading = digitalRead(buttonPins[i]);
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (buttonState[i] == LOW) {
          int ledState = digitalRead(quadPins[i]);
          digitalWrite(quadPins[i], !ledState);
        }
      }
    }
    lastButtonState[i] = reading;
  }
}

void blinkLeds() {
  int pinState;
  
  for (uint8_t i = 4; i < numQuadPins; i++) {
    pinState = digitalRead(quadPins[i]);
    digitalWrite(quadPins[i], !pinState);
  }

  pinState = digitalRead(pendPin);
  digitalWrite(pendPin, !pinState);
}

// the setup function runs once when you press reset or power the board
void setup() {
  for (uint8_t pin = 0; pin < numQuadPins; pin++) {
    pinMode(quadPins[pin], OUTPUT);
  }
  
  pinMode(pendPin, OUTPUT);
  
  for (uint8_t pin = 0; pin < numButtonPins; pin++) {
    pinMode(buttonPins[pin], INPUT_PULLUP);
  }

  unsigned long now = millis();
  for (uint8_t i = 0; i < numButtonPins; i++) {
    lastDebounceTime[i] = now;
  }

  Timer1.initialize(500000);
  Timer1.attachInterrupt(blinkLeds);
}

// the loop function runs over and over again forever
void loop() {
  checkButtons();
}
