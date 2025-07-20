
/*
 * IRTest.ino
 * Test Car MP3 remote contol
 *
 * Connections
 * 
 * D4 -> AX1838 output pin
 */
//#include <ATtinySerialOut.h>
#include <IRremote.h>
#include "RemoteCodes.h"


const int IR_RECV_PIN = 0; 

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

const byte HEARTBEAT_PIN = 1;
int counter = 0;
int counterState = LOW;


void setup()
{
  pinMode(HEARTBEAT_PIN, OUTPUT);
  digitalWrite(HEARTBEAT_PIN, LOW);

  // Set up IR receiver.
  irrecv.enableIRIn();

  Serial.begin(115200);
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
