/*
 * Character LCD Example Code
 * 
 * Connections
 * LCD -> Arduino
 * 4 (RS) -> D12
 * 5 (RW) -> D11
 * 6 (ENABLE) -> D10
 * 11 (D4) -> D5
 * 12 (D5) -> D4
 * 13 (D6) -> D3
 * 14 (D7) -> D2
 * 15 (BACKLIGHT+) -> D13
 */
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
const int backLight = 13;

void setup()
{
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);    // Turn on the backlight
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("hacktronics.com");
  lcd.setCursor(0, 2);
  lcd.print("AaBbCcDdEeFfGgHhIiJj");
  lcd.setCursor(0, 3);
  lcd.print("~!@#$%^&*()_+-=[]{}?");
}

void loop()
{
}
