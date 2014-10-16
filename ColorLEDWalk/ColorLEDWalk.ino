// ColorLED random walk
#include "ColorLED.h"

ColorLED led(8, 9, 10, 11);
int colorComp[3];

void setup() 
{
  // Initialize the LED to a random color.
  randomSeed(analogRead(A0));
  for (int i = 0; i < 3; ++i) {
    colorComp[i] = random(0, 256);
  }
  led.setColor(colorComp[0], colorComp[1], colorComp[2]);
  led.turnOn();
}

void loop()
{
  for (int i = 0; i < 3; ++i) {
    int choice = random(0, 3);
    switch (choice) {
    case 0:
      --colorComp[i];
      break;
    case 1:
      // Do nothing.
      break;
    case 2:
      ++colorComp[i];
      break;
    }

    // Clamp the component.
    colorComp[i] = max(0, colorComp[i]);
    colorComp[i] = min(255, colorComp[i]);
  }
  led.setColor(static_cast<byte>(colorComp[0]), 
	       static_cast<byte>(colorComp[1]), 
	       static_cast<byte>(colorComp[2]));
}
