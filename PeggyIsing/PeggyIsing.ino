/*
  Title:         PeggyIsing
  Date Created:  5/7/2014
  Last Modified: 5/7/2014
  Target:        Atmel ATmega168 
  Environment:   Arduino - 0011
  Purpose:       Drive 25x25 LED array
  Application:   2-D Ising Model

  Copyright (C) 2014 J. Bromley

  Distributed under the terms of the GNU General Public License, please see
  below.

  Adapted from a program by David Gustafik, and re-released under the GPL. :)
  Portions Copyright David Gustafik, 2007

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <avr/io.h> 
#include <stdlib.h> 
#include <avr/eeprom.h>
#include <stdint.h>

// Number of cells in frame buffer.
const int FRAME_X = 25;
const int FRAME_Y = 25;

// Number of cells to use for simulation.
const int CELLS_X = 25;
const int CELLS_Y = 24;

/////////////////////////////////////////////////////////////////////////


unsigned long lattice[FRAME_X];
volatile unsigned long frame_buffer[FRAME_X];
unsigned char input_reg, input_reg_old;
unsigned int refresh_num = 8;
int temperature = 23;
  
  
/////////////////////////////////////////////////////////////////////////
#define RAND_INV_RANGE(r) ((int) ((RAND_MAX + 1) / (r)))

unsigned int uniform_rand(unsigned int n) 
{
    int x = 0;

    do {
        x = rand();
    } while (x >= n * RAND_INV_RANGE(n));
    x /= RAND_INV_RANGE(n);
    return x;
}

/////////////////////////////////////////////////////////////////////////

unsigned char get_cell(unsigned long from[], char x, char y)
{
    if (x < 0) { 
        x = FRAME_X - 1; 
    }
    if (x > FRAME_X - 1) { 
        x = 0; 
    }
    if (y < 0) { 
        y = FRAME_Y - 1; 
    }
    if (y > FRAME_Y - 1) { 
        y = 0; 
    }

    return ((from[x] & ((unsigned long) 1 << y)) > 0);
}

/////////////////////////////////////////////////////////////////////////
static inline void set_cell(unsigned long to[], char x, char y, 
                            unsigned char value)
{
    if (value) {
        to[x] |= (unsigned long) 1 << y;
    } else {
        to[x] &= ~((unsigned long) 1 << y);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////
static inline void fill_cell(unsigned long to[], char x, char y)
{ 
    to[x] |= (unsigned long) 1 <<  y;
    return;
}

/////////////////////////////////////////////////////////////////////////
static inline void clear_cell(unsigned long to[], char x, char y)
{
    to[x] &= ~( (unsigned long) 1 << y);
    return;
}

/////////////////////////////////////////////////////////////////////////
unsigned int get_total(unsigned long from[])
{
    unsigned int total = 0;

    for (unsigned char x = 0; x < FRAME_X; x++) {
        for (unsigned char y = 0; y < FRAME_Y; y++) {
            if (get_cell(from, x, y)) {
                total++;
            }
        }
    }

    return total;
}

/////////////////////////////////////////////////////////////////////////
static inline void fill_random_data(unsigned long to[])
{
    unsigned char temp;
    for (temp = 0; temp < FRAME_X; temp++) {
        to[temp] = rand();
        to[temp] <<= 15;
        to[temp] |= rand();
    }
    return;
}

/////////////////////////////////////////////////////////////////////////
static inline void clear_data(unsigned long to[])
{
    unsigned char temp;
    
    for (temp = 0; temp < FRAME_X; temp++) {
        to[temp] = 0; 
    }

    return;
} 

/////////////////////////////////////////////////////////////////////////
unsigned char get_neighbours(unsigned long from[], signed char x, signed char y)
{
    unsigned char out = 0;

    if (get_cell(from, x - 1, y - 1)) { out++; }
    if (get_cell(from, x - 1, y)) { out++; }
    if (get_cell(from, x - 1, y + 1)) { out++; }
    if (get_cell(from, x, y - 1)) { out++; }
    if (get_cell(from, x, y + 1)) { out++; }
    if (get_cell(from, x + 1, y - 1)) { out++; }
    if (get_cell(from, x + 1, y)) { out++; }
    if (get_cell(from, x + 1, y + 1)) { out++; }

    return out;
}

/////////////////////////////////////////////////////////////////////////
void display_temperature(int temp)
{
    char y = FRAME_Y - 1;
    for (char x = 0; x < FRAME_X; ++x) {
        if (temp - 11 >= x) {
            fill_cell(lattice, FRAME_X - x - 1, y);
        } else {
            clear_cell(lattice, FRAME_X - x - 1, y);
        }
    }
}

/////////////////////////////////////////////////////////////////////////
static inline void display(unsigned long from[])
{
    unsigned long longtemp;

    for (unsigned char x = 0; x < FRAME_X; x++) { 
        longtemp = 0;
        for(unsigned char y = 0; y < FRAME_Y; y++) {  
            if (get_cell(from, x, y)) {
                longtemp |= (unsigned long) 1 << y;
            }
            frame_buffer[x] = longtemp;
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////////

void transmit_spi(char cData)
{
    //Start Transmission
    SPDR = cData;
    //Wait for transmission complete:
    while (!(SPSR & _BV(SPIF))) ;
}

void display_leds()
{
    unsigned int j, k;
    unsigned char out1, out2, out3, out4;
    unsigned long dtemp;  

    k = 0;
    while (k < refresh_num) {
        k++;
        j = 0;

        while (j < 25) {
            if (j == 0) {
                PORTD = 160;
            } else if (j < 16) {
                PORTD = j;
            } else {
                PORTD = (j - 15) << 4;  
            }
            dtemp = frame_buffer[j]; 
            out4 = dtemp & 255U;
            dtemp >>= 8;
            out3 = dtemp & 255U;
            dtemp >>= 8;
            out2 = dtemp & 255U;   
            dtemp >>= 8;
            out1 = dtemp & 255U;  

            transmit_spi(out1);
            transmit_spi(out2);
            transmit_spi(out3);

            PORTD = 0;  // Turn displays off
            
            transmit_spi(out4);

            PORTB |= _BV(1);              //Latch Pulse 
            PORTB &= ~( _BV(1));

            j++;
        }
    }
}


void delay_long(unsigned int delay)
{
    unsigned int delay_count = 0;

    while (delay_count <=  delay) { 
        asm("nop");  
        delay_count++;
    } 
}

void setup()                    // run once, when the sketch starts
{ 
    srand(eeprom_read_word((uint16_t *) 2));
    for (unsigned char temp = 0; temp != 255; temp++) {
        TCNT0 = rand();
    }

    eeprom_write_word((uint16_t *) 2, rand());

    PORTD = 0U;           // All B Input
    DDRD = 255U;          // All D Output

    PORTB = 1;            // Pull up on ("OFF/SELECT" button)
    PORTC = 255U;         // Pull-ups on C

    DDRB = 254U;          // B0 is an input ("OFF/SELECT" button)
    DDRC = 0;             //All inputs

    // Set MOSI, SCK output, all other SPI as input:
    DDRB = (1 << 3) | (1 << 5) | (1 << 2) | (1 << 1);

    // Enable SPI, MASTER, CLOCK RATE fck/4:        //TEMPORARY:: 1/128
    SPCR = (1 << SPE) | (1 << MSTR);//| ( 1 << SPR0 );//| ( 1 << SPR1 ) | ( 1 << CPOL ); 

    transmit_spi(0);
    transmit_spi(0);
    transmit_spi(0);

    PORTB |= _BV(1);              //Latch Pulse 
    PORTB &= ~( _BV(1));

    // Set up the initial screen.
    fill_random_data(lattice);

    display_temperature(temperature);
    
    input_reg_old = (PINC & 31) | ((PINB & 1)<<5);  
}

void loop()                       // run over and over again
{ 
    // Some routines follow to do things if the optional buttons are
    // installed-- a simple editor is implemented.
    input_reg = (PINC & 31) | ((PINB & 1) << 5);    

    // Input reg measures OFF ( bit 5) and b1-b5 (bits 0-4). 
    input_reg_old ^= input_reg;    
    // input_reg_old is now nonzero if there has been a change.

    if (input_reg_old) {
        input_reg_old &= input_reg;  

        // input_reg_old is now nonzero if the change was to the
        // button-released (not-pulled-down) state. I.e., the bit that was
        // high after the XOR corresponds to a bit that is presently
        // high. The checks that follow will handle MULTIPLE buttons being
        // pressed and unpressed at the same time.
        if (input_reg_old & 1) {
            // b1 "ANY" button is pressed
            temperature = 12;
            display_temperature(temperature);
        }  

        if (input_reg_old & 2)   
            {
                // b2 "left" button is pressed
            }  

        if (input_reg_old & 4) {
            // b3 "down" button is pressed        
            if (--temperature < 11) {
                temperature = 11;
            }
            display_temperature(temperature);
        }                         

        if (input_reg_old & 8) {
            // b4 "up" button is pressed
            if (++temperature > 35) {
                temperature = 35;
            }
            display_temperature(temperature);
        }  

        if (input_reg_old & 16) {
            // b5 "right" button is pressed
        }

        if (input_reg_old & 32) {
            // s2 "Off/Select" button is pressed. Toggle in/out of edit mode.
        }  
    }

    input_reg_old = input_reg;

    display(lattice);
    display_leds();

    for (char x = 0; x < CELLS_X; x++) { 
        for (char y = 0; y < CELLS_Y; y++) {  
            // Inner loop; should be made quick as possible.
            if (uniform_rand(50) < temperature) {
                fill_cell(lattice, x, y);
            } else {
                clear_cell(lattice, x, y);
            }
        }
        display_leds();
    }

    display_leds();
}
