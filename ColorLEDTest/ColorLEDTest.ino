// ColorLED test
#include "ColorLED.h"

const int DWELL_TIME = 2000;
const int TRANSITION_TIME = 1000;

enum Mode {DWELL, TRANSITION};
Mode mode = DWELL;

ColorLED led(8, 9, 10, 11);
Color color = Color::randomColor();
Color nextColor;
int dred;
int dgreen;
int dblue;
unsigned long startTime = millis();

void setup() 
{
  randomSeed(analogRead(A0));
  led.turnOn();
}

void loop()
{
  unsigned long time = millis() - startTime;
  if (mode == DWELL) {
    if (time > DWELL_TIME) {
      // Done dwelling, change to the transition mode.
      mode = TRANSITION;
      nextColor = Color::randomColor();
      dred = nextColor.red - color.red;
      dgreen  = nextColor.green - color.green;
      dblue = nextColor.blue - color.blue;
      startTime = millis();
    }
  } else {
    if (time > TRANSITION_TIME) {
      // Done transitioning.
      mode = DWELL;
      color = nextColor;
      led.setColor(color);
      startTime = millis();
    } else {
      float fraction = static_cast<float>(time) / TRANSITION_TIME;
      led.setColor(color.red + fraction * dred,
		   color.green + fraction * dgreen,
		   color.blue + fraction * dblue);
    }
  }
  
}
