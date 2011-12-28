/*
 * Particle.cpp
 * Implementation of a random-walking particle.
 */
#include "Particle.h"

Particle::Particle() : _x(0), _y(0), _oldx(0), _oldy(0)
{
}

Particle::Particle(int x, int y) : _x(x), _y(y), _oldx(x), _oldy(y)
{
}

void Particle::update()
{
  _oldx = _x;
  _oldy = _y;
  
  int dir = random(0, 5);
  switch (dir) {
    case 0:
      --_y;
      break;
    case 1:
      ++_x;
      break;
    case 2:
      ++_y;
      break;
    case 3:
      --_x;
      break;
  }
  _x = constrain(_x, 0, 7);
  _y = constrain(_y, 0, 7);
}

void Particle::draw(byte leds[8][8])
{
  leds[_oldy][_oldx] = 0;
  leds[_y][_x] = 1;
}


