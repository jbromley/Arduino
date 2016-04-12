// LED Random Walk

// Pin assignments
#define LED_PIN 9

// Color range and offset per step
#define RANGE 64
#define CENTER (RANGE / 2);

// Frame rate
#define MIN_DELAY 10
#define MAX_DELAY 100

// Current color
byte brightness = 128;

void setLedBrightness(byte brightness)
{
  analogWrite(LED_PIN, ~brightness);
}

void testColors()
{
  setLedBrightness(255);
  delay(500);

  setLedBrightness(128);
  delay(500);
  
  setLedBrightness(0);
  delay(500);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  testColors();
  randomSeed(analogRead(0));
}

void loop()
{
  int step = random(0, RANGE) - CENTER;
  brightness += step;
  
  setLedBrightness(brightness);

  delay(random(MIN_DELAY, MAX_DELAY));
}

