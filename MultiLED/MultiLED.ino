/*
 * multiled
 * Blink multiple LEDs in several different patterns. Written for the
 * Arduino Nano version 2.3.
 *
 * Pin Assignments
 * A0 (PC0) = LED 0
 * A1 (PC1) = LED 1
 * A2 (PC2) = LED 2
 * A3 (PC3) = LED 3
 * A4 (PC4) = momentary switch (brings pin high)
 * A7 (PC7) = potentiometer (delay time)
 * D8 (PB0) = LED 4
 * D9 (PB1) = LED 5
 * D10 (PB2) = LED 6
 * D11 (PB3) = LED 7
 * D12 (PB4) = LED 8
 * D13 (PB5) = LED 9
 * D2 (PD2) = LED 10
 * D3 (PD3) = LED 11
 * D4 (PD4) = LED 12
 * D5 (PD5) = LED 13
 * D6 (PD6) = LED 14
 * D7 (PD7) = LED 15
 */

/* Constants */
const int NUMBER_BITS = 16;
const int MAX_VALUE = 65536;

/* Control variables */
char analog_control = -1;

/* Time control for changing animations. */
unsigned long last_time = millis();
unsigned long this_time = millis();
const unsigned long duration = 10000;
char change_animation = 0;       /* Flag: change to the next animation. */

/* Array of animation function pointers and names */
typedef void (*animation_fn)(int);

struct animation_t {
  animation_fn fn;
  char* name;
};

animation_t* animations = 0; /* Array of possible animations */
int animation = 0;           /* Current animation */
int num_animations = 0;      /* Total animations */


void writeBits(word bits)
{
  byte d_bits = (byte) ((bits & 0xfc00) >> 8);
  byte b_bits = (byte) ((bits & 0x03f0) >> 4);
  byte c_bits = (byte) (bits & 0x000f);
  PORTD = d_bits;
  PORTB = b_bits;
  PORTC = c_bits;
}

void writeXorBits(word bits)
{
  byte d_bits = (byte) ((bits & 0xfc00) >> 8);
  byte b_bits = (byte) ((bits & 0x03f0) >> 4);
  byte c_bits = (byte) (bits & 0x000f);
  PORTD ^= d_bits;
  PORTB ^= b_bits;
  PORTC ^= c_bits;
}

void count(int delay_interval)
{
  static word value = 0;
  
  writeXorBits(value);
  delay(delay_interval);
  writeXorBits(value);
  ++value;
}

word binary_to_gray(word num)
{
  return (num >> 1) ^ num;
}

void gray_count(int delay_interval)
{
  static word value = 0;
  
  word gray_value = binary_to_gray(value);
  writeXorBits(gray_value);
  delay(delay_interval);
  writeXorBits(gray_value);
  ++value;
}

void random_single(int delay_interval)
{
  int bit = random(NUMBER_BITS);
  writeXorBits((word) (1 << bit));
  delay(delay_interval);
  writeXorBits((word) (1 << bit));
}

void random_leds(int delay_interval)
{
  int value = random(65536);
  writeXorBits((word) value);
  delay(delay_interval);
  writeXorBits((word) value);
}

void scan(int delay_interval)
{
  static char bit = 0;
  bit %= NUMBER_BITS;
  writeXorBits((word) (1 << bit));
  delay(delay_interval);
  writeXorBits((word) (1 << bit));
  ++bit;
}

void scan_both(int delay_interval)
{
  static char bit = 0;
  static char delta = -1;

  writeXorBits((word) (1 << bit));
  delay(delay_interval);
  writeXorBits((word) (1 << bit));
  if (bit == 0) {
    delta *= -1;
  } else if (bit == NUMBER_BITS - 1) {
    delta *= -1;
  }
  bit += delta;
}

void spring(int delay_interval)
{
  static word bits = 0;
  static char cur_bit = 0;
  static char delta = -1;

  writeXorBits(bits);
  delay(delay_interval);
  writeXorBits(bits);
  if (cur_bit < 0) {
    delta *= -1;
  } else if (cur_bit == NUMBER_BITS) {
    delta *= -1;
  }
  cur_bit += delta;
  if (delta > 0) {
    bits |= (1 << cur_bit);
  } else {
    bits ^= (1 << cur_bit);
  }
}

void flash_all(int delay_interval)
{
  static word all_on = 0xFFFF;
  static word all_off = 0x0000;
  
  writeXorBits(all_on);
  delay(delay_interval);
  writeXorBits(all_off);
  delay(delay_interval);
}

void random_walk(int delay_interval)
{
  static char pos = 3;
  
  pos += (random(2) == 0 ? -1 : 1);
  if (pos < 0) {
    pos = 0;
  } else if (pos > NUMBER_BITS - 1) {
    pos = NUMBER_BITS - 1;
  }
  
  writeXorBits((word) (1 << pos));
  delay(delay_interval);
  writeXorBits((word) (1 << pos));
}

void side_side(int delay_interval)
{
  const word left = 0xff00;
  const word right = 0x00ff;

  writeXorBits(left);
  delay(delay_interval);
  writeXorBits(left);
  delay(delay_interval);
  writeXorBits(right);
  delay(delay_interval);
  writeXorBits(right);
  delay(delay_interval);
}

void setup() 
{
  Serial.begin(9600);

  randomSeed(millis());
  
  for (int pin = 2; pin <= 13; ++pin) {
    pinMode(pin, OUTPUT);
  }
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
 
  num_animations = 10; 
  animations = (animation_t*) malloc(num_animations * sizeof(animation_t));
  animations[0].fn = &count;
  animations[0].name = "count";
  animations[1].fn = &gray_count;
  animations[1].name = "gray code count";
  animations[2].fn = &random_single;
  animations[2].name = "random single";
  animations[3].fn = &random_leds;
  animations[3].name = "random leds";
  animations[4].fn = &scan;
  animations[4].name = "scan";
  animations[5].fn = &scan_both;
  animations[5].name = "scan bidirectional";
  animations[6].fn = &spring;
  animations[6].name = "spring";
  animations[7].fn = &random_walk;
  animations[7].name = "random walk";
  animations[8].fn = &side_side;
  animations[8].name = "side to side";
  animations[9].fn = &flash_all;
  animations[9].name = "flash all";
}

void loop()
{
  this_time = millis();
  if (this_time - last_time > duration) {
    change_animation = ~change_animation;
    last_time = this_time;
  }
 
  if (Serial.available() > 0) {
    char byte_in = Serial.read();
    if (byte_in == 'a') {
      analog_control = ~analog_control;
      Serial.print("analog control: ");
      Serial.println(analog_control, DEC);
    }
  }
    
  int led_delay = 66;
  if (analog_control) {
    led_delay = analogRead(A7);
  }

  if (change_animation) {
    animation = ++animation % num_animations;
    change_animation = ~change_animation;
    Serial.println(animations[animation].name);
    writeBits((char) 0);
  }
  
  (*animations[animation].fn)(led_delay);
}
