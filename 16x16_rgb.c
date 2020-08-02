// run below line in cmd line for new AVR chip to set fuses for internal 8MHz clk
// avrdude -p atmega2560 -c usbtiny -U lfuse:w:0xc2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

// run below line in cmd line for new AVR chip to set fuses for 16MHz crystal
// avrdude -p atmega2560 -c usbtiny -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

// * color byte is rrrgggbb*

#define A1 PORTF
#define A2 PORTK
#define R1 PORTH
#define R2 PORTB
#define G1 PORTL
#define G2 PORTD
#define B1 PORTA
#define B2 PORTJ

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "frame_data.h"
#include "general_functions.h"
#include "animation_functions.h"

int main()
{

  // set data direction registers for all ports as outputs
  DDRF = 0xFF; // common anode 1
  DDRK = 0xFF; // common anode 2
  DDRH = 0xFF; // red 1
  DDRB = 0xFF; // red 2
  DDRL = 0xFF; // green 1
  DDRD = 0xFF; // green 2
  DDRA = 0xFF; // blue 1
  DDRJ = 0xFF; // blue 2

  // turn off color cathodes
  R1 = 0xFF;
  R2 = 0xFF;
  G1 = 0xFF;
  G2 = 0xFF;
  B1 = 0xFF;
  B2 = 0xFF;
  
  while(1)
  {  
    /*
   // color test
    A1 = A2 = 0xFF;
    R1 = R2 = 0x00;
    G1 = G2 = 0x00;
    B1 = B2 = 0x00;
    */
   
    // sequence of graphics and animations used on MakerFaire badge
   
    for(uint8_t i = 0; i < 20; i++)
      rainbow_transition(1);
   
   kirby_wave(3);
   ashpika_scroll(1);
   mario_walk(1);
   ghosty(5);
   sonic(2);
   mage(1);
   _delay_ms(200);
   pac_scroll_1(1);
   _delay_ms(1000);
   pac_scroll_2(1);
   _delay_ms(200);
   megaman(2);
   bub(2);
   link(1);
   snake(2);
   mr_saturn(4);
   nyan_scroll(2);
   drop(150, 5, 15);
   }
}
