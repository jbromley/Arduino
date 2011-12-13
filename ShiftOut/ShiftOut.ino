/*
 * Shift Out
 * Sample program for using a 74164 shift register. This program reads an
 * analog value from A0 and lights a number of LEDS proportional to the analog
 * value read.
 * 
 * D2 - 74164 CP input
 * D3 - 74164 A/B input
 * A0 - potentiometer
 */ 
const int CLK_PIN = 2;
const int DATA_PIN = 3;

void setup() 
{
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  digitalWrite(CLK_PIN, LOW);
}

void loop()
{
  static int last_number = -1;
  
  int number = analogRead(A0);
  if (number != last_number) {
    last_number = number;
    
    int segments = number / 114;
    int bits = 0;
    for (int i = 0; i < segments; ++i) {
      bits |= (1 << i);
    }
    
    shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, bits);
  }
  delay(50);
}
