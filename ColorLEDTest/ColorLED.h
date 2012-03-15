// ColorLED 
// Control a color LED.
#ifndef COLOR_LED_H
#define COLOR_LED_H
#include <Arduino.h>


struct Color {
  Color();
  Color(byte r, byte g, byte b);
  static Color randomColor();
  byte red;
  byte green;
  byte blue;
};

class ColorLED {
public:
  ColorLED(byte enablePin, byte redPin, byte greenPin, byte bluePin);
  ~ColorLED();

  void reset();

  void turnOn();
  void turnOff();
  void toggle();

  void setColor(byte red, byte green, byte blue);
  void setColor(const Color& color);
  Color color() const;

private:
  byte enablePin_;
  byte redPin_;
  byte greenPin_;
  byte bluePin_;

  byte enabled_;
  byte red_;
  byte green_;
  byte blue_;
};

#endif // COLOR_LED_H
