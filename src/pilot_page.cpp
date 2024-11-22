#include "EVE.h"
#include "tft_data.h"
#include "tft.h"
#include "touch.h"
//#include "globals.h"
#include <iostream>
#include <string>
#include "apmessages.h"

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


void display_pilot(void)
    {char str1[20];
    EVE_cmd_romfont(0, 34);

if (mfd_APStatus==16||mfd_APStatus==6||mfd_APStatus==12){    /*Ap Engaged Setup mode and status*/
    EVE_color_rgb_burst(0x008A36L);
    EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-450,10));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-140,130));
    EVE_cmd_dl_burst(DL_END);
    EVE_color_rgb_burst(0x005624L);
    EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-440,20));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-340,120));
    EVE_cmd_dl_burst(DL_END);
    //EVE_execute_cmd();
    switch (mfd_APMode)
    {
    case 0x10://Heading hold
        EVE_color_rgb_burst(WHITE);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-420,20,0,0,"A");
        EVE_cmd_text_burst(EVE_HSIZE-330,40,29,0,"Hold\nkursretning");
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);

        // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Set kurs");
        snprintf(str1,5,"%03.f",mfd_commandedheading);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line ____________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);

    //EVE_execute_cmd();        
    break;
    case 0x20: //NFU
        EVE_color_rgb_burst(WHITE);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-440,20,0,0,"N");EVE_cmd_text_burst(EVE_HSIZE-380,67,31,0,"fu");
        EVE_cmd_text_burst(EVE_HSIZE-330,40,29,0,"Ingen\nopfoelgning");
        /******************/
                /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);
                // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Set kurs");
        snprintf(str1,5,"%03.f",mfd_commandedheading);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line ____________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);

        break;
    case 0x0100: //No drift
        EVE_color_rgb_burst(WHITE);
        EVE_cmd_text_burst(EVE_HSIZE-430,20,0,0,"N");
        EVE_cmd_text_burst(EVE_HSIZE-370,67,31,0,"D");
        EVE_cmd_text_burst(EVE_HSIZE-330,40,29,0,"Ingen\nafdrift");
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);
                // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Set kurs");
        snprintf(str1,5,"%03.f",mfd_HeadingToSteer);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line ____________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);

    break;
    case 0x40://Nav
        EVE_color_rgb_burst(WHITE);
        EVE_cmd_text_burst(EVE_HSIZE-425,20,0,0,"N");
        EVE_cmd_text_burst(EVE_HSIZE-330,55,29,0,"Navigation");
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);
                // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Set kurs");
        snprintf(str1,5,"%03.f",mfd_HeadingToSteer);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line ____________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);
    break;
    case 0x0400://Wind
        EVE_color_rgb_burst(WHITE);
        EVE_cmd_text_burst(EVE_HSIZE-430,20,0,0,"W");
        EVE_cmd_text_burst(EVE_HSIZE-330,55,29,0,"Wind");
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Windangle");
        snprintf(str1,5,"%03.f",mfd_AWA);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);
        // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Commanded Windangle");
        snprintf(str1,5,"%03.f",mfd_commandedangle);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);

    break;
    case 0x8: // Follow up
        EVE_color_rgb_burst(WHITE);
        EVE_cmd_text_burst(EVE_HSIZE-425,20,0,0,"F");
        EVE_cmd_text_burst(EVE_HSIZE-380,67,31,0,"U");
        EVE_cmd_text_burst(EVE_HSIZE-330,55,29,0,"Opfoelgning");
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);
                // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Set Ror");
        snprintf(str1,5,"%03.f", mfd_SetRudder);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line ____________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);

    break;

    case 0x111: // Nfu
        EVE_color_rgb_burst(WHITE);
        EVE_cmd_text_burst(EVE_HSIZE-435,20,0,0,"N");
        EVE_cmd_text_burst(EVE_HSIZE-365,67,31,0,"F");
        EVE_cmd_text_burst(EVE_HSIZE-330,40,29,0,"Ingen\nOpfoelgning");
        EVE_cmd_text_burst(EVE_HSIZE-700,100,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-730,150,0,0,str1);
        EVE_cmd_text_burst(EVE_HSIZE-700,300,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-730,350,0,0,str1);

    break;

    default:
    break;
}
    }
    else /* Grey out Engaged pilot*/
    {
    EVE_color_rgb_burst(LIGHTGREY);
    EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-450,10));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-140,130));
    EVE_cmd_dl_burst(DL_END);
    EVE_color_rgb_burst(DARKGREY);
    EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-440,20));
    EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-340,120));
    EVE_cmd_dl_burst(DL_END);
    EVE_color_rgb_burst(WHITE);
    EVE_cmd_text_burst(EVE_HSIZE-425,20,0,0,"S");
        EVE_cmd_text_burst(EVE_HSIZE-795,5,29,0,"Heading");
        snprintf(str1,5,"%03.f",mfd_heading);
        EVE_cmd_text_burst(EVE_HSIZE-700,25,0,0,str1);
                // Line _____________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,130));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,130));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(5,130,29,0,"Set kurs");
        snprintf(str1,5,"%03.f",mfd_HeadingToSteer);
        EVE_cmd_text_burst(100,150,0,0,str1);
        // Line ____________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,260));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,260));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,260,29,0,"Rudder");
        snprintf(str1,5,"%02.f",mfd_rudder);
        EVE_cmd_text_burst(EVE_HSIZE-700,285,31,0,str1);
        // Line ______________________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,330));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,330));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,330,29,0,"Speed");
        snprintf(str1,5,"%03.f",mfd_speed);
        EVE_cmd_text_burst(EVE_HSIZE-700,355,31,0,str1);       
        // Line __________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,400));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,400));
        EVE_cmd_dl_burst(DL_END);
        /******************/
        EVE_cmd_text_burst(EVE_HSIZE-795,400,29,0,"Depth");
        snprintf(str1,5,"%02.f",mfd_depth);
        EVE_cmd_text_burst(EVE_HSIZE-700,425,31,0,str1);
        // Line ________________________
        EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,470));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-475,470));
        EVE_cmd_dl_burst(DL_END);

    }


    EVE_cmd_fgcolor_burst(0);
    if(mfd_APStatus==2 || mfd_APMode==0x8 || mfd_APStatus==6 ||mfd_APStatus==12/* || mfd_APMode==NFU*/)EVE_color_rgb_burst(GREY);
    else EVE_color_rgb_burst(RED);

    EVE_cmd_dl(DL_TAG + 100U);
    EVE_cmd_button_burst(EVE_HSIZE-450,150,150,64,31,0,"<<10");
    EVE_cmd_dl(DL_TAG);
    
    if(mfd_APStatus==2 || mfd_APMode==0x8 || mfd_APStatus==6 ||mfd_APStatus==12/*|| mfd_APMode==NFU*/)EVE_color_rgb_burst(GREY);
    else EVE_color_rgb_burst(GREEN); 

    EVE_cmd_dl(DL_TAG + 105U);
    EVE_cmd_button_burst(EVE_HSIZE-290,150,150,64,31,0,"10>>");
    EVE_color_rgb_burst(RED);

    EVE_cmd_dl(DL_TAG + 110U);
    EVE_cmd_button_burst(EVE_HSIZE-450,150+70,150,64,31,0,"<1");
    EVE_color_rgb_burst(GREEN);
    EVE_cmd_dl(DL_TAG + 115U);
    EVE_cmd_button_burst(EVE_HSIZE-290,150+70,150,64,31,0,"1>");
    EVE_color_rgb_burst(WHITE);
    EVE_cmd_fgcolor_burst(RED);

    EVE_cmd_dl(DL_TAG + 120U);
    EVE_cmd_button_burst(EVE_HSIZE-450,150+70+70+75,150,100,31,0,"STBY");
    EVE_cmd_fgcolor_burst(0);
    EVE_cmd_dl(DL_TAG + 125U);
    EVE_cmd_button_burst(EVE_HSIZE-290,150+70+70+75,150,100,31,0,"AUTO");
    EVE_cmd_dl(DL_TAG + 130U);
    EVE_cmd_button_burst(EVE_HSIZE-450,150+70+70,310,64,31,0,"MODE");
}