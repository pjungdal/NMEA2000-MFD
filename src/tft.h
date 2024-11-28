
#ifndef TFT_H
#define TFT_H
#include "touch.h"

/* some pre-definded colors */
#define RED 0xff0000UL
#define ORANGE 0xffa500UL
#define GREEN 0x00ff00UL
#define BLUE 0x0000ffUL
#define BLUE_1 0x5dade2L
#define YELLOW 0xffff00UL
#define MAGENTA 0xff00ffUL
#define PURPLE 0x800080UL
#define WHITE 0xffffffUL
#define BWHITE 0xAAAAAA
#define BLACK 0x000000UL
#define GREY 0x202020UL
#define LIGHTGREY 0x505050UL
#define DARKGREY 0x303030UL


#ifdef __cplusplus
extern "C"
{
#endif
   // extern uint16_t num_profile_a, num_profile_b;
    extern uint8_t pagenr ;
    extern uint32_t num_dl_static ; /* amount of bytes in the static part of our display-list */
    extern uint8_t tft_active ;
    extern uint16_t toggle_state ;
    extern uint16_t display_list_size ;
    extern uint8_t Selectedpage;
    extern uint8_t LightPressed;
      
    void TFT_init(void);
    void TFT_touch(void);
    void TFT_display(void);
    void TFT_buttons(void);
    void initStaticBackground(void);
#ifdef __cplusplus
}
#endif



#endif /* TFT_H */
