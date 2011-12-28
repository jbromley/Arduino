#ifndef PARTICLE_H
#define PARTICLE_H
/*
 * Particle.h
 * Simple particle model.
 */
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Particle
{
public:
  Particle();
  Particle(int x, int y);
  void update();
  void draw(byte leds[8][8]);

private:
  int _x;
  int _y;
  int _oldx;
  int _oldy;
};

#endif // PARTICLE_H
