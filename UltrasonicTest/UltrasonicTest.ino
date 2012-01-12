/*
 * UltrasonicTest.ino
 * Test the HC-SR04 Ultrasonic sensor
 *
 * Pin assignments
 * A0 = HC-SR04 trig
 * A1 = HC-SR04 echo
 */
 
int triggerPin = A1;
int echoPin = A0;
int ledPin = 13;

void trigger()
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

long rangeMicroseconds()
{
  return pulseIn(echoPin, HIGH);
}

long microsecondsToCentimeters(long microseconds)
{
  return (long) (microseconds / 2.0 / 29.4);
}

void printRange(long echoTime, long distance)
{
  Serial.print(echoTime);
  Serial.print(" microseconds = ");
  Serial.print(distance);
  Serial.println(" cm");
}

void setup()
{
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  trigger();
  long echoTime = rangeMicroseconds();
  digitalWrite(ledPin, LOW);
  long distance = microsecondsToCentimeters(echoTime);
  printRange(echoTime, distance);
  delay(1000);
}
