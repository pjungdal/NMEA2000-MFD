#include "EVE.h"
#include "tft_data.h"
#include "tft.h"
#include "touch.h"
#include <N2kMessages.h>
#include <NMEA2000.h>
#include "apmessages.h"
#include "state.h"
#include "Preferences.h"

uint8_t pagenr = 0;
uint16_t display_list_size ;
uint8_t Selectedpage = 1;
uint8_t MenuPressed = 0;
uint8_t LightPressed = 0;

extern tNMEA2000 &NMEA2000;
extern double mfd_heading;
extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_windangle;
extern double mfd_commandedheading;
extern double mfd_commandedangle;
extern  unsigned int  mfd_APMode;
extern  unsigned char mfd_APStatus;
extern unsigned int  mfd_PrevAPMode;
Preferences pref;
void touch_calibrate(void)
{

/* activate this if you are using a module for the first time or if you need to re-calibrate it */
/* write down the numbers on the screen and either place them in one of the pre-defined blocks above or make a new block */
#if 0
    /* calibrate touch and displays values to screen */

#if 1
    EVE_cmd_dl(CMD_DLSTART);
    EVE_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
    EVE_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_cmd_text((EVE_HSIZE/2), 50, 26, EVE_OPT_CENTER, "Please tap on the dot.");
    EVE_cmd_calibrate();
    EVE_cmd_dl(DL_DISPLAY);
    EVE_cmd_dl(CMD_SWAP);
    EVE_execute_cmd();
#else
    EVE_calibrate_manual(EVE_HSIZE, EVE_VSIZE);
#endif

    uint32_t touch_a = EVE_memRead32(REG_TOUCH_TRANSFORM_A);
    uint32_t touch_b = EVE_memRead32(REG_TOUCH_TRANSFORM_B);
    uint32_t touch_c = EVE_memRead32(REG_TOUCH_TRANSFORM_C);
    uint32_t touch_d = EVE_memRead32(REG_TOUCH_TRANSFORM_D);
    uint32_t touch_e = EVE_memRead32(REG_TOUCH_TRANSFORM_E);
    uint32_t touch_f = EVE_memRead32(REG_TOUCH_TRANSFORM_F);

    EVE_cmd_dl(CMD_DLSTART);
    EVE_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
    EVE_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_cmd_dl(TAG(0));

    EVE_cmd_text(5, 15, 26, 0, "TOUCH_TRANSFORM_A:");
    EVE_cmd_text(5, 30, 26, 0, "TOUCH_TRANSFORM_B:");
    EVE_cmd_text(5, 45, 26, 0, "TOUCH_TRANSFORM_C:");
    EVE_cmd_text(5, 60, 26, 0, "TOUCH_TRANSFORM_D:");
    EVE_cmd_text(5, 75, 26, 0, "TOUCH_TRANSFORM_E:");
    EVE_cmd_text(5, 90, 26, 0, "TOUCH_TRANSFORM_F:");

    EVE_cmd_setbase(16L);
    EVE_cmd_number(310, 15, 26, EVE_OPT_RIGHTX|8, touch_a);
    EVE_cmd_number(310, 30, 26, EVE_OPT_RIGHTX|8, touch_b);
    EVE_cmd_number(310, 45, 26, EVE_OPT_RIGHTX|8, touch_c);
    EVE_cmd_number(310, 60, 26, EVE_OPT_RIGHTX|8, touch_d);
    EVE_cmd_number(310, 75, 26, EVE_OPT_RIGHTX|8, touch_e);
    EVE_cmd_number(310, 90, 26, EVE_OPT_RIGHTX|8, touch_f);

    EVE_cmd_dl(DL_DISPLAY); /* instruct the co-processor to show the list */
    EVE_cmd_dl(CMD_SWAP); /* make this list active */
    EVE_execute_cmd();

    while(1);
#endif
}

/* check for touch events and setup vars for TFT_display() */
void TFT_touch(void)
{
    static uint32_t touch_previous_millis = 0;
    uint32_t touch_current_millis;
    tN2kMsg N2kMsg;
    touch_current_millis = millis();
    if (tft_active != 0)
    {
        uint8_t tag;
        static uint8_t toggle_lock = 0;

        if (EVE_IS_BUSY == EVE_busy()) /* is EVE still processing the last display list? */
        {
            return;
        }

        display_list_size = EVE_memRead16(REG_CMD_DL); /* debug-information, get the size of the last generated display-list */

        tag = EVE_memRead8(REG_TOUCH_TAG); /* read the value for the first touch point */

        switch (tag)
        {
        case 0:
            toggle_lock = 0;
            break;

        case 10: 
                 if ( ((touch_current_millis - touch_previous_millis) > 500)){
                     touch_previous_millis = touch_current_millis;
                     if (Selectedpage == pagedisplayed)
                         Selectedpage++;
                 if (Selectedpage == 9)
                     Selectedpage = 1;
#ifdef DEBUG
            //Serial.println(Selectedpage);
#endif

                 }
            break;
        case 20: /* use button on top as on/off toggle-switch */
            if (((touch_current_millis - touch_previous_millis) > 50))
            {
                touch_previous_millis = touch_current_millis;
               heading ++;
               if (heading >360)
                   heading = 0;
            }
#ifdef DEBUG
            //Serial.println("up");
#endif
            break;
        case 30: /* use button on top as on/off toggle-switch */
            //Selectedpage = 3;
#ifdef DEBUG
            //Serial.println("enter");
#endif
            break;
        case 40: /* use button on top as on/off toggle-switch */
            if (((touch_current_millis - touch_previous_millis) > 50))
            {
                touch_previous_millis = touch_current_millis;
                heading --;
                if (heading <0)
                    heading =  360;
            }

#ifdef DEBUG
                //Serial.println("down");
#endif
            break;
        case 50: /* use button on top as on/off toggle-switch */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {
                touch_previous_millis = touch_current_millis;
                LightPressed = true;
                LightLevel = LightLevel + 0x10;
                if (LightLevel > 0x80) LightLevel =0x10;

#ifdef DEBUG
            //Serial.println("menu/light");
#endif
            }
            break;


        case 100: /* <<10 */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;
            
            if(mfd_APMode==0x10 || mfd_APMode==0x100 || mfd_APMode==0x40 || mfd_APMode==0x400 && mfd_APStatus==16){
            unsigned char str[6] = {0x0A,0x1A,00,02,0xD1,06};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
#ifdef MSG_PRINT_CMD
            //Serial.println("<<10");
            //Serial.print("Set heading:" ); //Serial.println(mfd_commandedheading);
            //Serial.print("Set andle:" ); //Serial.println(mfd_commandedangle);
 #endif          
            }

            }
            break;
            case 105: /* 10>> */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;
            
            if(mfd_APMode==0x10 || mfd_APMode==0x100 || mfd_APMode==0x40 || mfd_APMode==0x400 && mfd_APStatus==16){
            unsigned char str[6] = {0x0A,0x1A,00,03,0xD1,06};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);

#ifdef MSG_PRINT_CMD
            //Serial.println("10>>");
            //Serial.print("Set heading:" ); //Serial.println(mfd_commandedheading);
            //Serial.print("Set andle:" ); //Serial.println(mfd_commandedangle);
#endif      
      }
            }
            break;
            case 110: /* <1 */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;
         //mfd_APMode==0x10 || mfd_APMode==0x100 || mfd_APMode==0x40 || mfd_APMode==0x400 && mfd_APStatus==16
            if(mfd_APStatus==16 && mfd_APMode==0x8){
            unsigned char str[6] = {0x02,0x1A,00,02,0xAE,0x00};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);}
            if(mfd_APStatus==2||mfd_APStatus==6||mfd_APMode==0x20){
            unsigned char str[6] = {0x02,0x0D,00,04,0xFF,0xFF};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            do {} while(EVE_memRead8(REG_TOUCH_TAG)==110);
            unsigned char str1[6] = {0x02,0x0D,00,0xFF,0xFF,0xFF};
            SetN2kAp(N2kMsg,(const char * ) str1);
            NMEA2000.SendMsg(N2kMsg);
            state=E;mode=NFU;
            //Serial.println("Nfu <1");
            }
            else
            if(mfd_APMode==0x10 || mfd_APMode==0x100 || mfd_APMode==0x40){
            unsigned char str[6] = {0x0A,0x1A,00,02,0xAE,00};          
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            //Serial.println("<1");
            }
            if(mfd_APMode==0x400){
            unsigned char str[6] = {0x0A,0x1A,00,03,0xAE,00};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            //Serial.println("1>");

            }

#ifdef MSG_PRINT_CMD
            //Serial.print("Set heading:" ); //Serial.println(mfd_commandedheading);
            //Serial.print("Set angle:" ); Serial.println(mfd_commandedangle); 
#endif
            }
            break;
            
            
            case 115: /* 1> */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;
            if(mfd_APStatus==16 && mfd_APMode==0x8){
            unsigned char str[6] = {0x02,0x1A,00,03,0xAE,0x00};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg); }          

            if(mfd_APStatus==2||mfd_APStatus==6||mfd_APMode==0x20){
            unsigned char str[6] = {0x02,0x0D,00,05,0xFF,0xFF};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            do {} while(EVE_memRead8(REG_TOUCH_TAG)==115);
            unsigned char str1[6] = {0x02,0x0D,00,0xFF,0xFF,0xFF};
            SetN2kAp(N2kMsg,(const char * ) str1);
            NMEA2000.SendMsg(N2kMsg);
            state=E;mode=NFU;
            Serial.println("Nfu 1>");
            }
            else
            if(mfd_APMode==0x10 || mfd_APMode==0x100 || mfd_APMode==0x40){
            unsigned char str[6] = {0x0A,0x1A,00,03,0xAE,00};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            Serial.println("1>");

            }
            if(mfd_APMode==0x400){
            unsigned char str[6] = {0x0A,0x1A,00,02,0xAE,00};
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            Serial.println("1>");

            }

#ifdef MSG_PRINT_CMD
            Serial.print("Set heading:" ); Serial.println(mfd_commandedheading);
            Serial.print("Set angle:" ); Serial.println(mfd_commandedangle); 
#endif
            }
            break;
            case 120: /* STBY */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;

            unsigned char str[6] = {0x0A,0x6,00,0xFF,0xFF,0xFF};         
            SetN2kAp(N2kMsg,(const char * ) str);
            NMEA2000.SendMsg(N2kMsg);
            state=S;mode=previous_mode;
            Serial.println("STBY");

            }
            break;
            case 125: /* AUTO  */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;
            //Serial.print("Test: ");  Serial.print("APMode: ");Serial.print(mfd_APMode,HEX);Serial.print("Prev APMode: ");Serial.print(mfd_PrevAPMode,HEX);Serial.print(" APStatus: ");Serial.println(mfd_APStatus,HEX);
            if(mfd_APStatus == 2||mfd_APStatus == 6){
            
            switch (mfd_PrevAPMode)
            {
            case 0x10:{
               unsigned char str[6] = {0x0A,0x09,0,0xFF,0xFF,0xFF};
                SetN2kAp(N2kMsg,(const char * )str);                              
                break;}
            case 0x40:{
               unsigned char str[6] = {0x0A,0x0A,0,0xFF,0xFF,0xFF};
                SetN2kAp(N2kMsg,(const char * )str);             
                break;}
            case 0x100:{
               unsigned char str[6] = {0x0A,0x0C,0,0xFF,0xFF,0xFF};
                SetN2kAp(N2kMsg,(const char * )str);             
                break;}
            case 0x400:{
               unsigned char str[6] = {0x0A,0x0F,0,0xFF,0xFF,0xFF};
                SetN2kAp(N2kMsg,(const char * )str);                     
                break;}
            case 0x8:{
               unsigned char str[6] = {0x02,0x0E,0,0xFF,0xFF,0xFF};
                SetN2kAp(N2kMsg,(const char * )str);                          
                break;}
            
            default:
                break;
            }
            NMEA2000.SendMsg(N2kMsg);
            } }
            break;
            case 130: /* Mode */
            if (((touch_current_millis - touch_previous_millis) > 250))
            {touch_previous_millis = touch_current_millis;
            //        EVE_cmd_button_burst(EVE_HSIZE-450,150,150,64,31,EVE_OPT_FLAT,"<<10");
            switch (mfd_APMode)
            {
            case 0x10:      {unsigned char str[6] = {0x0A,0x0c,0,0xFF,0xFF,0xFF};
                            SetN2kAp(N2kMsg,(const char * )str);   }
            break;
            case 0x100:     {unsigned char str[6] = {0x0A,0x0F,0,0xFF,0xFF,0xFF};
                            SetN2kAp(N2kMsg,(const char * )str);  } 
            break;
            case 0x400:     {unsigned char str[6] = {0x02,0x0e,0,0xFF,0xFF,0xFF};
                            SetN2kAp(N2kMsg,(const char * )str);} 
            break;
            case 0x8: 
            case 0x20:
                            {unsigned char str[6] = {0x0A,0x09,0,0xFF,0xFF,0xFF};
                            SetN2kAp(N2kMsg,(const char * )str);}   
            break;
            
            default:
                break;
            }
            ////Serial.println("MODE");

            }
            NMEA2000.SendMsg(N2kMsg);
            break;
 

        default:
            break;
        }
    }
}
