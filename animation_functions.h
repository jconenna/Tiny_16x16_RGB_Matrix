// animation specific functions

// generates horizontal moving rainbow effect on display
void rainbow_transition(uint8_t rate)
{
  uint8_t type[256];

  for(uint16_t i = 0; i < 256; i++)
  {
    type[i] = 0;

    // red
    if(rainbow[i] & (1 << 7) | rainbow[i] & (1 << 6) | rainbow[i] & (1 << 5))
      type[i] |= (1 << 2);

    // green
    if(rainbow[i] & (1 << 4) | rainbow[i] & (1 << 3) | rainbow[i] & (1 << 2))
      type[i] |= (1 << 1);

    // blue
    // green
    if(rainbow[i] & (1 << 1) | rainbow[i] & (1))
      type[i] |= 1;
   }

   for(uint8_t i = 0; i < 8; i++)
     {
       for(uint16_t j = 0; j < 256; j++)
       {
         // blue to cyan
         if(type[j] == 1)
           rainbow[j] = ((i & (1<<2)) << 2) | ((i & (1<<1)) << 2) | ((i & (1)) << 2) | 3;
      
         //green to yellow
         if(type[j] == 2)
           rainbow[j] = ((i & (1<<2)) << 5) | ((i & (1<<1)) << 5) | ((i & (1)) << 5) | 28;

         //cyan to green
         if(type[j] == 3)
           rainbow[j] = (((7-i)&(1<<2)) >> 1) | (((7-i)&(1<<1)) >> 1) | 28;

         //red to magenta
         if(type[j] == 4)
           rainbow[j] = ((i&(1<<2)) >> 1) | ((i&(1<<1)) >> 1) | 224;

         //magenta to blue
         if(type[j] == 5)
           rainbow[j] = (((7-i) & (1<<2)) << 5) | (((7-i) & (1<<1)) << 5) | (((7-i) & (1)) << 5) | 3;

         //yellow to red
         if(type[j] == 6)
           rainbow[j] = (((7-i) & (1<<2)) << 2) | (((7-i) & (1<<1)) << 2) | (((7-i) & (1)) << 2) | 224;
       }
     display_rb(rate);
  }
}

// displays current rainbow transition frame on display for n cycles 
void display_rb(uint16_t n)
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
           color    = rainbow[i*16+j];
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
            // left row
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
          
            // right row
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

// animation of mario frames to make him stand and walk, n times
void mario_walk(uint8_t n)
{

  for(uint8_t x = 0; x < n; x++)
  {
    display_frame(mario_ptr, 300);
      
    for(uint8_t i = 0; i < 10; i++)
      {
        display_frame(mario_w1_ptr, 27);
        display_frame(mario_w2_ptr, 27);
        display_frame(mario_w3_ptr, 27);
      }
      
      display_frame(mario_ptr, 100);
      
      display_frame_mirror(mario_ptr, 300);
      
      for(uint8_t i = 0; i < 10; i++)
      {
        display_frame_mirror(mario_w1_ptr, 27);
        display_frame_mirror(mario_w2_ptr, 27);
        display_frame_mirror(mario_w3_ptr, 27);
      }
      
      display_frame_mirror(mario_ptr, 100);
  }
}

// animation of kirby frames to make him wave, n times
void kirby_wave(uint8_t n)
{
  for(uint8_t x = 0; x < n; x++)
  { 
    display_frame(kirby1_ptr, 200);
    display_frame(kirby3_ptr, 35);
    display_frame(kirby2_ptr, 35);
    display_frame(kirby3_ptr, 35);   
    display_frame(kirby2_ptr, 35);
    display_frame(kirby3_ptr, 35);   
  }
}

// animation of ghost frames, n times
void ghosty(uint8_t n)
{
  for(uint8_t x = 0; x < n; x++)
  {
    display_frame(ghost1_ptr, 80);
    display_frame(ghost2_ptr, 80);   
  }
}

// animation of megaman frames, n times
void megaman(uint8_t n)
{
  for(uint8_t x = 0; x < n; x++)
  {
    display_frame(megaman1_ptr, 150);
    display_frame(megaman2_ptr, 150);
    display_frame(megaman3_ptr, 150);
  }
}

// animation of bub frames, n times
void bub(uint8_t n)
{
  for(uint8_t x = 0; x < n; x++)
    {
      display_frame(bub1_ptr, 100);
      display_frame(bub2_ptr, 40);
      display_frame(bub3_ptr, 40);
      display_frame(bub4_ptr, 40);
      display_frame(bub1_ptr, 40);
      display_frame(bub2_ptr, 40);
      display_frame(bub3_ptr, 40);
      display_frame(bub4_ptr, 40);
      display_frame(bub1_ptr, 40);
      display_frame(bub5_ptr, 250);
    }
}

// animation of snake frames, n times
void snake(uint8_t n)
{
  for(uint8_t j = 0; j < n; j++)
  {  
      
    for(uint8_t i = 0; i < 7; i++)
    { 
      display_frame(snake1_ptr, 50);
      display_frame(snake2_ptr, 50);
    }
     
    for(uint8_t i = 0; i < 2; i++)
    { 
      display_frame(snake1_ptr, 50);
      display_frame(snake3_ptr, 50);
    }
  }
}

// animation of link frames, n times
void link(uint8_t n)
{
  for(uint8_t j = 0; j < n; j++)
  {  
    for(uint8_t i = 0; i < 5; i++)
    { 
      display_frame(link1_ptr, 60);
      display_frame(link2_ptr, 60);
    }
     
    for(uint8_t i = 0; i < 5; i++)
    { 
      display_frame(link3_ptr, 60);
      display_frame(link4_ptr, 60);
    }
     
    for(uint8_t i = 0; i < 5; i++)
    { 
      display_frame(link5_ptr, 60);
      display_frame(link6_ptr, 60);
    }
     
    for(uint8_t i = 0; i < 5; i++)
    { 
      display_frame(link7_ptr, 60);
      display_frame(link8_ptr, 60);
    }
  }
}

// animation of mr. saturn frames, n times
void mr_saturn(uint8_t n)
{
  for(uint8_t j = 0; j < n; j++)
  {  
    for(uint8_t i = 0; i < 2; i++)
    { 
      display_frame(mrs1_ptr, 125);
      display_frame(mrs2_ptr, 125);
    }
  }
}

// animation of mage frames, n times
void mage(uint8_t n)
{
  for(uint8_t j = 0; j < n; j++)
  {  
    for(uint8_t i = 0; i < 5; i++)
    { 
      display_frame(bm1_ptr, 75);
      display_frame(bm2_ptr, 75);
    }
     
    for(uint8_t i = 0; i < 5; i++)
    { 
      display_frame(bm3_ptr, 75);
      display_frame(bm4_ptr, 75);
    }
  }
}

// animation of sonic frames, n times
void sonic(uint8_t n)
{
  for(uint8_t j = 0; j < n; j++)
  {
    display_frame(sonic1_ptr, 300);
    display_frame(sonic2_ptr, 25);
    display_frame(sonic3_ptr, 200);
    display_frame(sonic2_ptr, 25);

  }
  display_frame(sonic1_ptr, 200);
}

// scrolling and cycling of nyan cat frames across the display, n times
void nyan_scroll(uint16_t n)
{
  
  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = n; x--;)
  { 
    for( uint16_t y = 77; y > 14; y--)
    {
      for(uint16_t z = 0; z < 16; z++) 
        {
          // for all cols
          for(uint8_t i = 0; i <= 15; i++)
            { 
              // get rgb pwm components from col i
              for(uint8_t j = 0; j <= 15; j++)
                { 
                  color = pgm_read_byte(&nyan[y%12][(((y)-i)*16)+j]);
                  red[j] = color >> 5;
                  green[j] = (color >> 2) & 0b00000111;
                  blue[j] = (color << 1) & 0b00000111;
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
        } // for z
    } // for y
  } // for x
}

// scrolling and cycling of pacman being chased, n times
void pac_scroll_1(uint16_t n)
{
  
  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = n; x--;)
  {
    for( uint16_t y = 113; y > 14; y--)
    {
      for(uint16_t z = 0; z < 12; z++)
        {
          // for all cols
          for(uint8_t i = 0; i <= 15; i++)
          { 
            // get rgb pwm components from col i
            for(uint8_t j = 0; j <= 15; j++)
            { 
              color = pgm_read_byte(&pac_1[y%2][(((y)-i)*16)+j]);
              red[j] = color >> 5;                  
              green[j] = (color >> 2) & 0b00000111;
              blue[j] = (color << 1) & 0b00000111;
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
        } // for z
    } // for y
  } // for x
}

// scrolling and cycling of pacman chasing ghosts, n times
void pac_scroll_2(uint16_t n)
{
  
  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = n; x--;)
  {
    for( uint16_t y = 15; y < 115; y++)
    {
      for(uint16_t z = 0; z < 12; z++) 
      {
        // for all cols
        for(uint8_t i = 0; i <= 15; i++)
          { 
            // get rgb pwm components from col i
            for(uint8_t j = 0; j <= 15; j++)
              { 
                color = pgm_read_byte(&pac_2[y%2][(((y)-i)*16)+j]);
                red[j] = color >> 5;                  
                green[j] = (color >> 2) & 0b00000111;
                blue[j] = (color << 1) & 0b00000111;
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
      } // for z
    } // for y
  } // for x
}

// scrolling and cycling of ash & pikachu frames, n times
void ashpika_scroll(uint16_t n)
{
  
  // arrays of pwm vals for each color pixel in row
  uint8_t red   [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t blue  [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t green [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  uint8_t color = 0x00;
    
  // iterate for up to n times
  for(uint16_t x = n; x--;)
  {
    for( uint16_t y = 61; y > 14; y--)
    {
      for(uint16_t z = 0; z < 25; z++)
      {
      
        // for all cols
        for(uint8_t i = 0; i <= 15; i++)
        { 
          // get rgb pwm components from col i
          for(uint8_t j = 0; j <= 15; j++)
          { 
            color    = pgm_read_byte(&ashpika[y%2][(((y)-i)*16)+j]);
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
      } // for z
    } // for y
  } // for x
}

// animate a number of random colored expanding circles
// keep num_drops <= 5 to avoid flickering
void drop(uint16_t iterations, uint8_t num_drops, uint8_t del)
{
  uint8_t n[num_drops]; // offset in radius of drop circle
  uint8_t x[num_drops]; // center - x
  uint8_t y[num_drops]; // center - y
  uint8_t c[num_drops]; // color
   
  // initialize n, x, y, c
  for(uint8_t i = 0; i < num_drops; i++)
  {
    n[i] = (22 / num_drops)*i;
    x[i] = random()%15+1;
    y[i] = random()%15+1;
    c[i] = random()%255+1;
   }  
   
  // iterate animation
  for(uint16_t i = iterations; i--;)
  { 
    // clear frame
    clear_drop();
     
    // expand of reset drops   
    for(uint8_t i = 0; i < num_drops; i++)
      update_drop(x[i], y[i], n[i], c[i]); 
     
    // display frame
    display_drop(del);
     
    // update drop parameters
    for(uint8_t i = 0; i < num_drops; i++)
    {
      if(n[i] != 22)
        n[i]++;
      else
      {
        n[i] = 0;
        x[i] = random()%15+1;
        y[i] = random()%15+1;
        c[i] = random()%255+1;
      }
    }     
  } 
}

// expand size of all drops
void update_drop(uint8_t x, uint8_t y, uint8_t n, uint8_t color)
{
  for(int8_t i = 0; i < 16; i++)
  {
    int16_t imx2 = (i-x)*(i-x);
     
    for(int8_t j = 0; j < 16; j++) 
      if(sqrt_lt[(imx2 + (j-y)*(j-y))] == n)
        drop_frame[i*16+j] |= color;
  }
}

// display the drop frame
void display_drop(uint16_t n)
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
          color    = drop_frame[i*16+j];
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

// clear the drop frame
void clear_drop()
{
  for(uint16_t i = 0; i < 256; i++)
    drop_frame[i] = 0x00;
}