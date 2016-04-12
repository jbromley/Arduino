// RGB Random Walk

// Pin assignments
#define RED_PIN 9
#define GREEN_PIN 11
#define BLUE_PIN 10

// Color range and offset per step
#define RANGE 64
#define CENTER (RANGE / 2);

// Frame rate
#define MIN_DELAY 10
#define MAX_DELAY 50

// Current color
byte r = 128;
byte g = 128; 
byte b = 128;

void setLEDColor(byte red, byte green, byte blue)
{
  analogWrite(RED_PIN, ~red);
  analogWrite(GREEN_PIN, ~green);
  analogWrite(BLUE_PIN, ~blue);
}

void testColors()
{
  setLEDColor(255, 0, 0);
  delay(1000);

  setLEDColor(0, 255, 0);
  delay(1000);
  
  setLEDColor(0, 0, 255);
  delay(1000);
}

void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  testColors();

  randomSeed(analogRead(0));
}

void loop()
{
  int step = random(0, RANGE) - CENTER;
  r += step;
  step = random(0, RANGE) - CENTER;
  g += step;
  step = random(0, RANGE) - CENTER;
  b += step;
  
  setLEDColor(r, g, b);

  delay(random(MIN_DELAY, MAX_DELAY));
}

