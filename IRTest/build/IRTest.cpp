#include <Arduino.h>
/*
 * IRTest.ino
 * Test Car MP3 remote contol
 *
 * Connections
 * 
 * D4 -> AX1838 output pin
 */
#include <IRremote.h>
#include "RemoteCodes.h"


const int IR_RECV_PIN = 4; 

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

const byte HEARTBEAT_PIN = 13;
int counter = 0;
int counterState = LOW;


void setup()
{
  pinMode(HEARTBEAT_PIN, OUTPUT);
  digitalWrite(HEARTBEAT_PIN, LOW);

  // Set up IR receiver.
  irrecv.enableIRIn();

  Serial.begin(9600);
}

void loop()
{
  if (irrecv.decode(&results)) {
    Serial.println(remoteKeyFromCode(results.value));
    // Serial.println(results.value, HEX);
    irrecv.resume();
  }

  if (++counter % 10000 == 0) {
    counter = 0;
    counterState = (counterState == LOW ? HIGH : LOW);
    digitalWrite(HEARTBEAT_PIN, counterState);
  }
}
