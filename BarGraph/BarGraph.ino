/*
 * 10-Segment multicolor bar graph demo
 *
 * Pin Assignments
 * A0 -> 1 (orange common anode)
 * A1 -> 10 (green common anode)
 * D2 - D11 -> 20 - 11 (common cathode)
 */
 
void setup()
{
  for (int pin = 2; pin < 12; ++pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
}

void loop()
{
  for (int color = 0; color < 3; ++color) {
    switch (color) {
      case 0:
        digitalWrite(A0, HIGH);
        digitalWrite(A1, LOW);
        break;
      case 1:
        digitalWrite(A0, LOW);
        digitalWrite(A1, HIGH);
        break;
      case 2:
        digitalWrite(A0, HIGH);
        digitalWrite(A1, HIGH);
    }
    
    int value = 0;
    for (int segment = 0; segment <= 10; ++segment) {
      for (int bit = 0; bit < 10; ++bit) {
        digitalWrite(2 + bit, value & (1 << bit) ? LOW : HIGH);
      }
      value |= (1 << segment);
      delay(66);
    }
  }
}
