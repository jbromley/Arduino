/*
 * Life_7219
 *
 * The game of life implemented on an 8x8 matrix controlled by a Maxim 7219 chip.
 */
#include "LedControl.h"

const byte SIZE = 8;
const byte MAX_SAME_POP = 16;
const int MAX_GENERATIONS = 900;
const int DELAY = 100;

LedControl lc = LedControl(12, 11, 10, 1);
byte world[SIZE][SIZE][2];
int generation = 0;
byte population = 0;
byte last_population = 0;
byte same_pop_count = 0;
byte density = 25;

void setup()
{
  Serial.begin(9600);
  
  /* Wake up the LED matrix. */
  lc.shutdown(0, false);
  
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  
  /* Clear the display. */
  lc.clearDisplay(0);  
  
  /* Set up the world. */
  reset_world();
}

void loop()
{
  ++generation;
  last_population = population;
  
  /* Display the current generation. */
  for (int row = 0; row < SIZE; ++row) {
    int row_value = 0;
    for (int col = 0; col < SIZE; ++col) {
      row_value = (row_value << 1) + world[row][col][0];
    }
    lc.setRow(0, row, row_value);
  }
  
  /* Create the next generation. */
  for (int row = 0; row < SIZE; ++row) {
     for (int col = 0; col < SIZE; ++col) {
       /* Default is that the cell stays the same. */
       world[row][col][1] = world[row][col][0];
       int neighbors = count_neighbors(row, col);
       if (neighbors == 3 && world[row][col][0] == 0) {
         /* A new cell is born. */
         world[row][col][1] = 1;
         ++population;
       }
       else if ((neighbors < 2 || neighbors > 3) && world[row][col][0] == 1) {
         /* Cell is either isolated or crowded, dies. */
         world[row][col][1] = 0;
         --population;
       }
     }
  }
        
  /* Copy the next generation into place. */
  /* memcpy(&world[0][0][0], &world[0][0][1], SIZE * SIZE); */
  for (int row = 0; row < SIZE; ++row) {
    for (int col = 0; col < SIZE; ++col) {
      world[row][col][0] = world[row][col][1];
    }
  }
  
  /* Check to make sure things are still changing. */
  if (population == last_population) {
    ++same_pop_count;
    if (same_pop_count > MAX_SAME_POP) {
      reset_world();
    }
  } 
  else {
    same_pop_count = 0;
  }
  
  if (generation > MAX_GENERATIONS) {
    reset_world();
  }    

  delay(DELAY);
}


int count_neighbors(int row, int col) 
{
   return world[(row + 1) % SIZE][col][0] + 
         world[row][(col + 1) % SIZE][0] + 
         world[(row + SIZE - 1) % SIZE][col][0] + 
         world[row][(col + SIZE - 1) % SIZE][0] + 
         world[(row + 1) % SIZE][(col + 1) % SIZE][0] + 
         world[(row + SIZE - 1) % SIZE][(col + 1) % SIZE][0] + 
         world[(row + SIZE - 1) % SIZE][(col + SIZE - 1) % SIZE][0] + 
         world[(row + 1) % SIZE][(col + SIZE - 1) % SIZE][0]; 
}

void reset_world()
{
  population = 0;
  last_population = 0;
  same_pop_count = 0;
  generation = 0;
  respawn(density);
}

void respawn(byte density)
{
  for (int row = 0; row < SIZE; ++row) {
    for (int col = 0; col < SIZE; ++col) {
      if (random(100) < density) {
        // Add an individual at the current position.
        world[row][col][0] = 1;
        ++population;
      }
      else {
        world[row][col][0] = 0;
      }
      world[row][col][1] = 0;
    }
  }
  
  Serial.println(population);
}
