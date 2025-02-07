/*
@file    tft.c
@brief   TFT handling functions for EVE_Test project
@version 1.22
@date    2023-03-24
@author  Rudolph Riedel

@section History

1.17
- replaced the UTF-8 font with a freshly generated one and adjusted the parameters for the .xfont file

1.18
- several minor changes

1.19
- removed most of the history
- changed a couple of "while (EVE_busy()) {};" lines to "EVE_execute_cmd();"
- renamed PINK to MAGENTA

1.20
- several minor changes

1.21
- several minor changes

1.22
- added touch calibration values for EVE_GD3X

 */
//#include <Arduino.h>
#include "EVE.h"
#include "tft_data.h"
#include "tft.h"
#include "touch.h"
//#include "globals.h"
#include <iostream>
#include <string>
#include "apmessages.h"
//#include "state.h"
#include <math.h>
#include <chrono>
#include <time.h>
#include "imagemap.h"
#include "display_state.h"
#define TEST_UTF8 0






extern double mfd_heading;
extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_TWA;
extern double mfd_AWA;
extern double mfd_TWS;
extern double mfd_TWD;

extern double mfd_commandedheading;
extern double mfd_commandedangle;
extern unsigned char Engaged; 
extern  unsigned int  mfd_APMode;
extern  unsigned char mfd_APStatus;
extern double mfd_HeadingToSteer;
extern double mfd_angle_order;
extern double mfd_SetRudder;
extern double WPTDist;

extern double WPTDist;
extern double LLDist;
extern double LLTime;
extern double TTW;
extern double SETA;
extern double NLB;
extern double WPTBrg;
extern double Cog;
extern double Sog;
extern double TimeLoc;
extern double PosLat;
extern double PosLon;

extern uint8_t menubutton[24000] PROGMEM;
extern uint8_t pagebutton[24000] PROGMEM;

//extern const uint8_t outer[44724] PROGMEM;
extern void  Display_Steering_Page(void);
extern void init_steering_page(void);
extern void Display_sailsteer(void);
extern void init_sailsteer(void);
extern void Display_gps_page(void);
extern void display_pilot(void);
extern void init_speed_depth(void);
extern void display_speed_depth(void);
extern void init_combined1_page(void);   
extern void  Display_combined1_Page(void);
extern void init_combined2_page(void);   
extern void  Display_combined2_Page(void);
extern void init_digital(void);
extern void display_digital(void);
uint32_t num_dl_static; /* amount of bytes in the static part of our display-list */
uint8_t tft_active;
uint8_t pagedisplayed = 0;
uint8_t LightLevel = 10;
int heading = 0;
bool buttons_init =false;
rg_state_t rg_state = EMPTY;

void initStaticBackground(void)
{
    EVE_cmd_dl(CMD_DLSTART); /* Start the display list */
    EVE_cmd_dl(DL_TAG); /* no tag = 0 - do not use the following objects for touch-detection */
    EVE_cmd_bgcolor(WHITE); /* light grey */
    EVE_cmd_dl(DL_VERTEX_FORMAT); /* set to 0 - reduce precision for VERTEX2F to 1 pixel instead of 1/16 pixel default */
    EVE_cmd_dl(DL_END);
    EVE_execute_cmd();


}

void TFT_init(void)
{

    if (E_OK == EVE_init())
    {
        tft_active = 1;
        EVE_memWrite8(REG_PWM_DUTY, 0x10);  /* setup backlight, range is from 0 = off to 0x80 = max */
        touch_calibrate();
        // Load bitmaps for buttons on right side of display
        //These are static and loaded in start of RAM_G
        EVE_cmd_loadimage(MEM_PAGE,EVE_OPT_NODL,pagebutton,sizeof(pagebutton));
        EVE_cmd_loadimage(MEM_MENU,EVE_OPT_NODL,menubutton,sizeof(menubutton));
        // As Sailsteer is the default page, this must be loaded in RAM_G
        init_sailsteer();
        initStaticBackground();
        EVE_execute_cmd;
    }
}

void TFT_buttons(void)
{
    static int32_t rotate = 0;
#ifdef DEBUG
//Serial.println("Display buttons");
#endif
    if(tft_active != 0U)
    {

if (!buttons_init){
EVE_cmd_dl(DL_TAG + 10U);
EVE_cmd_fgcolor(0);
EVE_cmd_button_burst(680,0,120,100,27,0,"");
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(MEM_PAGE, EVE_ARGB4, 120U, 100U);
EVE_cmd_dl_burst(VERTEX2F(10880,0));
EVE_cmd_dl_burst(DL_END);
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 20U);
EVE_cmd_button_burst(680,100,120,90,27,0,"");
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 30U);
EVE_cmd_button_burst(680,190,120,100,27,0,"");
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 40U);
EVE_cmd_button_burst(680,290,120,90,27,0,"");
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 50U);
EVE_cmd_button_burst(680,380,380,100,27,0,"");
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(MEM_MENU, EVE_ARGB4, 120U, 100U);
EVE_cmd_dl_burst(VERTEX2F(10880,6080));
EVE_cmd_dl_burst(DL_END);
EVE_cmd_dl(DL_TAG);
//buttons_init=true;

}
}
}
void TFT_display(void){
char timestr[10];
int hours;
int mins;
int secs;
int deg;

    EVE_start_cmd_burst();

    EVE_cmd_romfont(0,34);
    EVE_cmd_romfont(1,32);

    EVE_cmd_dl_burst(CMD_DLSTART);
    EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK);
    EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_cmd_dl_burst(DL_TAG);

    TFT_buttons();
    EVE_memWrite8(REG_PWM_DUTY, LightLevel);  /* setup backlight, range is from 0 = off to 0x80 = max */
    EVE_color_rgb(WHITE);
    EVE_cmd_dl(DL_VERTEX_FORMAT);

    switch ((Selectedpage))

    {
    case 1:/* Sail steer*/{
        init_sailsteer();
        Display_sailsteer();
        pagedisplayed = 1;
    break;
    }
    case 2:{/*Speed and depth*/
        init_speed_depth();
        display_speed_depth();
        pagedisplayed = 2;
    break;
        }
    case 3:{
        init_steering_page();
        Display_Steering_Page();
        pagedisplayed = 3;
    break;}
    case 4:{
        init_digital();
        display_digital();
        pagedisplayed = 4;
    break;}
    case 5:{
        init_combined2_page();
        Display_combined2_Page();
        pagedisplayed = 5;
    break;}
    case 6:{/*Navigate*/
        Display_gps_page(); 
        pagedisplayed = 6;
    break;}
    case 7:{
        init_combined1_page();
        Display_combined1_Page();
        pagedisplayed = 7;
    break;}
    case 8:{/* B&G pilot controller*/
        display_pilot();
        pagedisplayed = 8;
    break;}

    default:
    break;
    }

    EVE_execute_cmd;
    EVE_cmd_dl_burst(DL_DISPLAY);
    EVE_cmd_dl_burst(CMD_SWAP);
    EVE_end_cmd_burst();
    };