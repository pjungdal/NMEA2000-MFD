
#ifndef TFT_DATA_H
#define TFT_DATA_H

#if	defined (__AVR__)
    #include <avr/pgmspace.h>
#else
    #if !defined(PROGMEM)
        #define PROGMEM
    #endif
#endif
extern  uint8_t page[6951] PROGMEM;    // 120x84
extern  uint8_t up[4575] PROGMEM;      // 120x60
extern  uint8_t enter[9734] PROGMEM;   // 120x90
extern  uint8_t down[4941] PROGMEM;    // 120x64
extern  uint8_t menu[7160] PROGMEM;    // 120x84
extern  uint8_t H5000[166323] PROGMEM; // 720x480
extern  uint8_t pic[3391] PROGMEM ;    //100x100
extern  uint8_t flash[7765] PROGMEM;
extern  uint8_t inner[37668] PROGMEM; // 348x350 59K
extern  uint8_t Heading_hold[3070] PROGMEM;



    // unsigned int Cprose_outer_425x419_RGB565_Converted_png_len = 32514;
extern  uint8_t outer[44724] PROGMEM; // 425x419 87K

#endif /* TFT_DATA_H */