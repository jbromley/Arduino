/*
  ClockSimulator

  Simulates three quadrature signals and a single clock pendulum signal.

  Current state is just set up to test output connections.
  TODO: Debounce input buttons and light an LED when a button is pressed (input test)
*/
static uint8_t quadPins[] = {2, 3, 4, 5, 6, 7, 8};
static uint8_t numPins = 7;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  for (uint8_t pin = 0; pin < numPins; pin++) {
    pinMode(quadPins[pin], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  for (uint8_t pin = 0; pin < numPins; pin++) {
    digitalWrite(quadPins[pin], HIGH);  // turn the LED on (HIGH is the voltage level)
  }
  delay(500);                      // wait for a second
  for (uint8_t pin = 0; pin < numPins; pin++) {
    digitalWrite(quadPins[pin], LOW);  // turn the LED on (HIGH is the voltage level)
  }
  delay(500);                      // wait for a second
}
