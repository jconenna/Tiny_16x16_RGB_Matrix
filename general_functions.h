// general frame display functions

// displays a 16x16 frame on the display
// takes pointer to 256 byte aray in PROGMEM
// n is number of times to cycles frame on display, to set duration
void display_frame(const uint8_t *frame, uint16_t n)
{

  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = n + 1; x--;)
  {
    // for all cols
    for(uint8_t i = 0; i <= 15; i++)
      { 
        // get rgb pwm components from col i
        for(uint8_t j = 0; j <= 15; j++)
        { 
          color    = pgm_read_byte_near(&frame[i*16+j]);
          red[j]   = color >> 5;
          green[j] = (color >> 2) & 0b00000111;
          blue[j]  = (color << 1) & 0b00000111;
        }
  
        // begin with RGB output off
        R1 = 0xFF;
        R2 = 0xFF;
        G1 = 0xFF;
        G2 = 0xFF;
        B1 = 0xFF;
        B2 = 0xFF;
        
        // use i to check which column to turn on
        if(i < 8)
          A1 = (1 << i);
        else 
          A2 = (1 << (i-8));

        // for 8 pwm iterations
        for(uint8_t p = 0;  p < 8; p++)
        {
          // set output during pwm iteration based on value that determine time on vs off
          for(uint8_t b = 0; b < 8; b++)
          {
            if(red[b] <= p)
              R1 |= (1<<b);
            else
              R1 &= ~(1<<b);
            if(green[b] <= p)
              G1 |= (1<<b);
            else
              G1 &= ~(1<<b);
            if(blue[b] <= p)
              B1 |= (1<<b);
            else
              B1 &= ~(1<<b);
          }
          for(uint8_t b = 0; b < 8; b++)
          {
            if(red[b+8] <= p)
              R2 |= (1<<b);
            else
              R2 &= ~(1<<b);
            if(green[b+8] <= p)
              G2 |= (1<<b);
            else
              G2 &= ~(1<<b);
            if(blue[b+8] <= p)
              B2 |= (1<<b);
            else
              B2 &= ~(1<<b);
          }
          
        }

        // turn off all column anodes
        A1 = 0x00;
        A2 = 0x00;
       
      } // for i
  } // for x
}


// displays a 16x16 frame on the display mirrored horizontally
// takes pointer to 256 byte aray in PROGMEM
// n is number of times to cycles frame on display, to set duration
void display_frame_mirror(const uint8_t *frame, uint16_t n)
{

  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = 0; x <= n; x++)
  {
    // for all cols
    for(uint8_t i = 0; i <= 15; i++)
      { 
        // get rgb pwm components from col i
        for(uint8_t j = 0; j <= 15; j++)
        { 
          color    = pgm_read_byte_near(&frame[i*16+j]);
          red[j]   = color >> 5;
          green[j] = (color >> 2) & 0b00000111;
          blue[j]  = (color << 1) & 0b00000111;
        }
  
        // begin with RGB output off
        R1 = 0xFF;
        R2 = 0xFF;
        G1 = 0xFF;
        G2 = 0xFF;
        B1 = 0xFF;
        B2 = 0xFF;
        
        // use i to check which column to turn on
        if(i < 8)
          A2 = (1 << (7-i));
        else 
          A1 = (1 << (7 - (i-8)));

        // for 8 pwm iterations
        for(uint8_t p = 0;  p < 8; p++)
        {
          // set output during pwm iteration based on value that determine time on vs off
          for(uint8_t b = 0; b < 8; b++)
          {
            if(red[b] <= p)
              R1 |= (1<<b);
            else
              R1 &= ~(1<<b);
            if(green[b] <= p)
              G1 |= (1<<b);
            else
              G1 &= ~(1<<b);
            if(blue[b] <= p)
              B1 |= (1<<b);
            else
              B1 &= ~(1<<b);
          }
          for(uint8_t b = 0; b < 8; b++)
          {
            if(red[b+8] <= p)
              R2 |= (1<<b);
            else
              R2 &= ~(1<<b);
            if(green[b+8] <= p)
              G2 |= (1<<b);
            else
              G2 &= ~(1<<b);
            if(blue[b+8] <= p)
              B2 |= (1<<b);
            else
              B2 &= ~(1<<b);
          }
          
        }

        // turn off all column anodes
        A1 = 0x00;
        A2 = 0x00;
       
      } // for i
  } // for x
}

void display_frame_ram(const uint8_t *frame, uint16_t n)
{

  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = n + 1; x--;)
  {
    // for all cols
    for(uint8_t i = 0; i <= 15; i++)
      { 
        // get rgb pwm components from col i
        for(uint8_t j = 0; j <= 15; j++)
        { 
          color    = frame[i*16+j];
          red[j]   = color >> 5;
          green[j] = (color >> 2) & 0b00000111;
          blue[j]  = (color << 1) & 0b00000111;
         }
  
         // begin with RGB output off
         R1 = 0xFF;
         R2 = 0xFF;
         G1 = 0xFF;
         G2 = 0xFF;
         B1 = 0xFF;
         B2 = 0xFF;
        
         // use i to check which column to turn on
         if(i < 8)
           A1 = (1 << i);
         else 
           A2 = (1 << (i-8));

         // for 8 pwm iterations
         for(uint8_t p = 0;  p < 8; p++)
           {
             // set output during pwm iteration based on value that determine time on vs off
             for(uint8_t b = 0; b < 8; b++)
               {
                 if(red[b] <= p)
                   R1 |= (1<<b);
                 else
                   R1 &= ~(1<<b);
                 if(green[b] <= p)
                   G1 |= (1<<b);
                 else
                   G1 &= ~(1<<b);
                 if(blue[b] <= p)
                   B1 |= (1<<b);
                 else
                  B1 &= ~(1<<b);
               }
             for(uint8_t b = 0; b < 8; b++)
               {
                 if(red[b+8] <= p)
                   R2 |= (1<<b);
                 else
                   R2 &= ~(1<<b);
                 if(green[b+8] <= p)
                   G2 |= (1<<b);
                 else
                   G2 &= ~(1<<b);
                 if(blue[b+8] <= p)
                   B2 |= (1<<b);
                 else
                   B2 &= ~(1<<b);
               }
          
           }

         // turn off all column anodes
         A1 = 0x00;
         A2 = 0x00;
       
      } // for i
  } // for x
}