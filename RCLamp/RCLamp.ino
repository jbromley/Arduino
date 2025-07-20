#include <Arduino.h>
#include <ATtinySerialOut.h>

#define IR_INPUT_PIN 0

#include "TinyIRReceiver.cpp.h"

void setup() {
  // IR receiver on D0 (pin 13)
  pinMode(0, INPUT);

  // LEDs on D1, D2, D3 (pins 12 to 10)
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  // Turn all LEDs off.
  for (int pin = 1; pin <= 3; ++pin) {
    digitalWrite(pin, LOW);
  }

  // Seed the random number generator.
  randomSeed(analogRead(0));

  // Set up serial output
  initTXPin();
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing ATtinySerialOut " VERSION_ATTINY_SERIAL_OUT));
  writeString("OSCCAL = ");
  writeUnsignedByteHexWithPrefix(OSCCAL);
  Serial.println(F("Receiving NEC IR signals at pin 0"));

  initPCIInterruptForTinyReceiver();
}

void loop() {
    int pin = random(1,  4);
    
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
}

void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat) {
    /*
     * Print only very short output, since we are in an interrupt context and do not want to miss the next interrupts of the repeats coming soon
     */
    Serial.print(F("A=0x"));
    Serial.print(aAddress, HEX);
    Serial.print(F(" C=0x"));
    Serial.print(aCommand, HEX);
    Serial.print(F(" R="));
    Serial.print(isRepeat);
    Serial.println();
}
