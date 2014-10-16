/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// LED pins
const byte RED_LED = 8;
const byte GREEN_LED = 9;

void setup() {                
  // initialize the digital pin as an output.
  pinMode(RED_LED, OUTPUT);     
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {
  digitalWrite(RED_LED, HIGH);  
  digitalWrite(GREEN_LED, LOW);
  delay(250);
  digitalWrite(RED_LED, LOW);  
  digitalWrite(GREEN_LED, HIGH);
  delay(250);               // wait for a second
}
