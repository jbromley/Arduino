// ColorLED
// Implementation
#include "ColorLED.h"

Color::Color() :
  red(255), green(255), blue(255)
{
}

Color::Color(byte r, byte g, byte b) :
  red(r), green(g), blue(b)
{
}

Color Color::randomColor()
{
  return Color(random(0, 256), random(0, 256), random(0, 256));
}
  

ColorLED::ColorLED(byte enablePin, byte redPin, byte greenPin, byte bluePin) :
  enablePin_(enablePin), 
  redPin_(redPin), 
  greenPin_(greenPin), 
  bluePin_(bluePin),
  enabled_(0),
  red_(0),
  green_(0),
  blue_(255)
{
  pinMode(enablePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  reset();
}

void ColorLED::reset()
{
  digitalWrite(enablePin_, LOW);
  analogWrite(redPin_, 255 - red_);
  analogWrite(greenPin_, 255 - green_);
  analogWrite(bluePin_, 255 - blue_);
}

ColorLED::~ColorLED()
{
  digitalWrite(enablePin_, LOW);
}

void ColorLED::turnOn()
{
  enabled_ = HIGH;
  digitalWrite(enablePin_, HIGH);
}

void ColorLED::turnOff()
{
  enabled_ = LOW;
  digitalWrite(enablePin_, LOW);
}

void ColorLED::toggle()
{
  enabled_ = (enabled_ == LOW ? HIGH : LOW);
  digitalWrite(enablePin_, enabled_);
}

void ColorLED::setColor(byte red, byte green, byte blue)
{
  red_ = red;
  green_ = green;
  blue_ = blue;

  analogWrite(redPin_, 255 - red_);
  analogWrite(greenPin_, 255 - green_);
  analogWrite(bluePin_, 255 - blue_);
}

void ColorLED::setColor(const Color& color)
{
  setColor(color.red, color.green, color.blue);
}

Color ColorLED::color() const
{
  return Color(red_, green_, blue_);
}

