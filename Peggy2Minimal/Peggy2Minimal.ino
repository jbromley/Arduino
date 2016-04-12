/* Simple example code for Peggy 2.0, using the Peggy2 library.

  Initialize a single frame buffer array, fill it with dots, and display it.
  -- much like the default Peggy code.

  Copyright (c) 2008 Windell H Oskay.  All right reserved.

  This example is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this software; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <avr/io.h>
#include <Peggy2.h>

Peggy2 frame;     // Make a first frame buffer object, called firstframe
int fractionOn = 48;
unsigned char lastInput;
unsigned char thisInput;

void setup()
{
  randomSeed(analogRead(A0));

  // Serial.begin(9600);
  
  // Initialize the Peggy hardware. Call this only once, even if there are
  // multiple frames.
  frame.HardwareInit();
  generateFrame();
  showFraction();

  // Set up for button input
  PORTC = 0xFF;       // Set pull up resistors.
  DDRC = 0x00;        // All port C pins are inputs.

  lastInput = PINC & 0x1F;
}

void loop()
{
  thisInput = PINC & 0x1F;
  lastInput ^= thisInput;

  if (lastInput & (1 << 2)) {
    // The down button is pressed.
    fractionOn -= 2;
    if (fractionOn < 0) {
      fractionOn = 0;
    }
    showFraction();
  }

  if (lastInput & (1 << 3)) {
    // The up button is pressed.
    fractionOn += 2;
    if (fractionOn > 100) {
      fractionOn = 100;
    }
    showFraction();
  }

  lastInput = thisInput;

  // Serial.println(fractionOn);

  // Randomly update a cell.
  int x = random(24);
  int y = random(25);
  int isOn = (random(100) < fractionOn ? 1 : 0);
  frame.WritePoint(x, y, isOn);
  frame.RefreshAll(1); //Draw frame buffer one time
}

void generateFrame()
{
  for (unsigned short y = 0; y < 25; ++y) {
    for (unsigned short x = 0; x < 24; ++x) {
      frame.WritePoint(x, y, random(100) < fractionOn ? 1 : 0);
    }
  }
}

void showFraction()
{
  int leds = fractionOn / 4;
  for (int y = 24; y >= 0; --y) {
    frame.WritePoint(24, y, (24 - y) < leds ? 1 : 0);
  }
}

