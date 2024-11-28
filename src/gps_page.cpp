#include "EVE.h"
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




void Display_gps_page(void){
char timestr[50] = "";
int hours;
int mins;
int secs;
int deg;

EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
EVE_cmd_dl_burst(VERTEX2F(50, 200));
EVE_cmd_dl_burst(VERTEX2F(630, 200));
EVE_cmd_dl_burst(VERTEX2F(225, 225));
EVE_cmd_dl_burst(VERTEX2F(225, 325));
EVE_cmd_dl_burst(VERTEX2F(453, 225));
EVE_cmd_dl_burst(VERTEX2F(453, 325));
EVE_cmd_dl_burst(VERTEX2F(225, 350));
EVE_cmd_dl_burst(VERTEX2F(225, 460));
EVE_cmd_dl_burst(VERTEX2F(453, 350));
EVE_cmd_dl_burst(VERTEX2F(453, 460));

EVE_cmd_dl_burst(VERTEX2F(50, 340));
EVE_cmd_dl_burst(VERTEX2F(210, 340));

EVE_cmd_dl_burst(VERTEX2F(240, 340));
EVE_cmd_dl_burst(VERTEX2F(438, 340));

EVE_cmd_dl_burst(VERTEX2F(468, 340));
EVE_cmd_dl_burst(VERTEX2F(630, 340));
EVE_cmd_dl_burst(DL_END);
EVE_cmd_romfont(1,32);



secs=(int)PosLat*3600;
deg=secs/3600;
secs=secs%3600;
mins=secs/60;
secs%=60;
sprintf(timestr,"N %.2u %.2u.%.3u'",deg,mins,secs);
EVE_cmd_text_burst(210,40,1,0,timestr);
secs=(int)PosLon*3600;
deg=secs/3600;
secs=secs%3600;
mins=secs/60;
secs%=60;
sprintf(timestr,"E %.2u %.2u.%.3u'",deg,mins,secs);
//Serial.println(timestr);
EVE_cmd_text_burst(210,120,1,0,timestr);
//Print deg char in pos string
sprintf(timestr,"%c",120);
EVE_cmd_text_burst(315,132,19,0,timestr);
EVE_cmd_text_burst(323,52,19,0,timestr);

EVE_cmd_text_burst(50, 20, 29, 0, "Pos");
EVE_cmd_text_burst(50, 200, 29, 0, "Cog");
EVE_cmd_text_burst(50, 340, 29, 0, "Wpt brg");
EVE_cmd_text_burst(291, 200, 29, 0, "Time Loc");
EVE_cmd_text_burst(463, 200, 29, 0, "Sog");
EVE_cmd_text_burst(319, 340, 29, 0, "Etw");
EVE_cmd_text_burst(463, 340, 29, 0, "Wpt dist");

sprintf(timestr,"%.2f ",WPTDist/1000);
EVE_cmd_text_burst(495,385,1,0,timestr);
if(TTW <0) TTW=0;
hours=TTW/3600;
hours=hours+2; if(hours>24)hours=0;
secs=(int)TTW % 3600;

mins=secs/60 ;
secs = secs%60;
sprintf(timestr,"%.2u:%.2u:%.2u",hours,mins,secs);
//Serial.println(timestr);
EVE_cmd_text_burst(230,385,1,0,timestr);
EVE_cmd_number_burst(70,385,1,0,WPTBrg);
EVE_cmd_number_burst(70,245,1,0,Cog);
sprintf(timestr,"%.2f ",Sog/10);
EVE_cmd_text_burst(495,245,1,0,timestr);

//EVE_cmd_number_burst(495,245,1,0,Sog);

hours=TimeLoc/3600;
hours=hours+2; if(hours>24)hours=0;
secs=(int)TimeLoc % 3600;

mins=secs/60 ;
secs = secs%60;
sprintf(timestr,"%.2u:%.2u:%.2u",hours,mins,secs);
EVE_cmd_text_burst(230,245,1,0,timestr);
}