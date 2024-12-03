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
extern uint8_t pages[10];


extern void  Display_Steering_Page(void);
extern bool init_steering_page(void);
extern void Display_sailsteer(void);
extern bool init_sailsteer(void);
extern void Display_gps_page(void);
extern void display_pilot(void);
extern bool init_speed_depth(void);
extern void display_speed_depth(void);
extern bool init_combined1_page(void);   
extern void  Display_combined1_Page(void);
extern bool init_combined2_page(void);   
extern void  Display_combined2_Page(void);
extern bool init_digital(void);
extern void display_digital(void);
extern void display_pagemenu(void);
extern uint8_t menuselect;
extern uint8_t pmpage,pmfunction;
uint32_t num_dl_static; /* amount of bytes in the static part of our display-list */
uint8_t tft_active;
uint8_t pagedisplayed = 0;
extern uint8_t LightLevel;
int heading = 0;
bool buttons_init =false;
rg_state_t rg_state = EMPTY;
char status_str[200];
uint8_t flashstatus = 0;




void TFT_init(void)
{

    if (E_OK == EVE_init())
    {
        tft_active = 1;
        EVE_cmd_flashdetach();
        EVE_execute_cmd();

        EVE_cmd_flashattach();
		Serial.println(EVE_cmd_flashfast(),16);
		flashstatus=EVE_memRead8(REG_FLASH_STATUS);
        EVE_memWrite8(REG_PWM_DUTY, LightLevel);
		switch (flashstatus){
			case 0:sprintf(status_str,"%s ","Flash status: FLASH_STATUS_INIT ");
			break;
			case 1:sprintf(status_str,"%s ","Flash status: FLASH_STATUS_DETACHED");
			break;
			case 2:sprintf(status_str,"%s ","Flash status: FLASH_STATUS_BASIC");
			break;
			case 3:sprintf(status_str,"%s ","Flash status: FLASH_STATUS_FULL");
			break;
		}
				Serial.print("TFT init");Serial.print(" status:");Serial.println(status_str);

        EVE_memWrite8(REG_PWM_DUTY, LightLevel);  /* setup backlight, range is from 0 = off to 0x80 = max */

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
EVE_color_rgb_burst(WHITE);
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_button_burst(680,0,120,100,27,0,"");
		EVE_cmd_setbitmap_burst(0x800000|128,EVE_ASTC_4X4,120,100);
		EVE_cmd_dl_burst(VERTEX2F(10880,0));
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 20U);
EVE_cmd_button_burst(680,100,120,90,27,0,"");
		EVE_cmd_setbitmap_burst(0x800000|512,EVE_ASTC_4X4,120,100);
		EVE_cmd_dl_burst(VERTEX2F(10880,1600-5*16));
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 30U);
EVE_cmd_button_burst(680,190,120,100,27,0,"");
		EVE_cmd_setbitmap_burst(0x800000|896,EVE_ASTC_4X4,120,100);
		EVE_cmd_dl_burst(VERTEX2F(10880,3040));
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 40U);
EVE_cmd_button_burst(680,290,120,90,27,0,"");
		EVE_cmd_setbitmap_burst(0x800000|1280,EVE_ASTC_4X4,120,100);
		EVE_cmd_dl_burst(VERTEX2F(10880,4640-5*16));
EVE_cmd_dl(DL_TAG);

EVE_cmd_dl(DL_TAG + 50U);
EVE_cmd_button_burst(680,380,380,100,27,0,"");
		EVE_cmd_setbitmap_burst(0x800000|1664,EVE_ASTC_4X4,120,100);
		EVE_cmd_dl_burst(VERTEX2F(10880,6080));
EVE_cmd_dl(DL_TAG);
EVE_cmd_dl_burst(DL_END);}

}
}

void TFT_display(void){
char timestr[10];
int hours;
int mins;
int secs;
int deg;
bool loading=false;
    if(menuselect==0){
    switch ((pages[Selectedpage]))
    {
    case 1:{ // Sail steer
        loading=init_sailsteer();
    break;
    }
    case 2:{//Speed and depth
        loading=init_speed_depth();
    break;
        }
    case 3:{
        loading=init_steering_page();
    break;}
    case 4:{
        loading=init_digital();
    break;}
    case 5:{
        loading=init_combined2_page();
    break;}
    case 6:{//Navigate
    break;}
    case 7:{
        loading=init_combined1_page();
    break;}
    case 8:{    // B&G pilot controller

    break;}

    default:
    break;
    }}
if(loading) return;

        //if (EVE_IS_BUSY == EVE_busy()) /* is EVE still processing the last display list? */
        /*{
            return;
        }*/

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
   //Serial.print("Selected: "); Serial.print(Selectedpage); Serial.print(" Display: ");Serial.println(pages[Selectedpage]);

    switch (menuselect)
    {
    case 1:{ //Serial.println("display_pagemenu()");
            display_pagemenu();
                EVE_cmd_dl_burst(DL_DISPLAY);
                EVE_cmd_dl_burst(CMD_SWAP);
                EVE_end_cmd_burst();
            return;
            }
        break;
    
    default:
        break;
    }



    switch ((pages[Selectedpage]))
    {
    case 1:{ // Sail steer
//        init_sailsteer();
        Display_sailsteer();
        pagedisplayed = 1;
    break;
    }
    case 2:{//Speed and depth
 //       init_speed_depth();
        display_speed_depth();
        pagedisplayed = 2;
    break;
        }
    case 3:{
//        init_steering_page();
        Display_Steering_Page();
        pagedisplayed = 3;
    break;}
    case 4:{
//        init_digital();
        display_digital();
        pagedisplayed = 4;
    break;}
    case 5:{
//        init_combined2_page();
        Display_combined2_Page();
        pagedisplayed = 5;
    break;}
    case 6:{//Navigate
        Display_gps_page(); 
        pagedisplayed = 6;
    break;}
    case 7:{
//        init_combined1_page();
        Display_combined1_Page();
        pagedisplayed = 7;
    break;}
    case 8:{    // B&G pilot controller
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