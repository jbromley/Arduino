/*
 * 7219Matrix
 * Controls an LED matrix with a Maxim 7219.
 *
 * Pin assignments
 * pin 10 is connected to LOAD pin 12
 * pin 11 is connected to the CLK pin 13
 * pin 12 is connected to the MAX7219 pin 1
 */
#include <LedControl.h>

const int waitTime = 16;
LedControl lc = LedControl(12,11,10,1);

void setup()
{
  // The zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);  // Turn off power saving, enables display.
  lc.setIntensity(0, 15);  // Sets brightness (0~15 possible values).
  lc.clearDisplay(0);     // Clear screen.
}

void loop()
{
  for (int rowOffset = 7; rowOffset > -8; --rowOffset) {
    int row = 7 - abs(rowOffset);
    lc.setRow(0, row, 0b11111111);
    delay(waitTime);
    lc.setRow(0, row, 0b00000000);
    delay(waitTime);
  }

  for (int colOffset = 7; colOffset > -8; --colOffset) {
    int col = 7 - abs(colOffset);
    lc.setColumn(0, col, 0b11111111);
    delay(waitTime);
    lc.setColumn(0, col, 0b00000000);
    delay(waitTime);
  }

  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      lc.setLed(0, col, row, true);   // Turns on LED at col, row.
      delay(waitTime);
      lc.setLed(0, col, row, false);  // Turns off LED at col, row.
      delay(waitTime);
    }
  }
  
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      lc.setLed(0, col, row, true);
      delay(2 * waitTime);
    }
  }
}
