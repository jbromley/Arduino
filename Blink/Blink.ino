/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() {
  Serial.begin(9600);  
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(13, OUTPUT);     
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  
  digitalWrite(13, HIGH);   // set the LED on
  delay(sensorValue);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(sensorValue);              // wait for a second
}
