/*
 * Eight-digit seven-segment display test
 * This uses two four-digit seven segment displays and the Maxim 7219 controller.
 * 
 * Pin assignments
 * 10 - LOAD (pin 12)
 * 11 - CLK (pin 13)
 * 12 - DIN (pin 1)
 */
#include "LedControl.h"

const int delayTime = 200;
LedControl lc(12, 11, 10, 1);

const int bufSize = 8;
char buffer[bufSize];
int bufPos = 0;
int dispPos = 1;

void writeArduino(int startPos)
{
  const char arduino[] = {'a', (char) 0x05, 'd', (char) 0x1c, (char) 0x10, (char) 0x15, (char) 0x1D, ' '};
  const int numChars = sizeof(arduino) / sizeof(arduino[0]);
  
  for (int i = 0; i < numChars; ++i) {
    int pos = (startPos + i) % 8;
    if (arduino[i] >= 0x20) {
      lc.setChar(0, pos, arduino[i], false);
    } else {
      lc.setRow(0, pos, arduino[i]);
    }
  }
}

void setup()
{
  randomSeed(analogRead(0));
 
  lc.shutdown(0, false);
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);

  memset(buffer, ' ', 8);
}

void loop()
{
   buffer[bufPos] = random(0, 19);
  if (++bufPos >= bufSize) {
    bufPos = 0;
  }
  
  for (int i = 0; i < bufSize; ++i) {
    char c = buffer[(dispPos + i) % 8];
    if (c < 10) {
      lc.setDigit(0, i, c, false);
    } else if (c < 16) {
      lc.setChar(0, i, 'A' + c - 10, false);
    } else {
      lc.setChar(0, i, ' ', false);
    }
  }
  ++dispPos;
  delay(delayTime);
}

