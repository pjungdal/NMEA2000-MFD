#include "EVE.h"
#include "tft.h"
#include "sailsteer.h"
#include "display_state.h"

// Sailsteer is page number one i list of pages
extern  uint8_t Sailsteer_Outer[23536] PROGMEM; 
extern  uint8_t Sailsteer_Inner[29504] PROGMEM; 

extern  uint8_t sbbar[20856] PROGMEM;
extern const uint8_t blue_t[1024] PROGMEM;
extern const uint8_t blue_a[3552] PROGMEM;
extern const uint8_t Sailsteer_Ship[2816] PROGMEM;
extern rg_state_t rg_state;
extern double mfd_heading;

extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_TWA;
extern double mfd_AWA;
extern double mfd_TWS;
extern double mfd_TWD;

uint32_t memco_ss;
uint32_t memci_ss;
uint32_t memsh_ss;
uint32_t membt_ss;
uint32_t memba_ss;
uint32_t memfree_ss;
uint32_t width_ss,height_ss;
char    str1_ss[50];
extern void spinner(void);

bool init_sailsteer(void){

        if(rg_state != SAILST){
               
        spinner();
        Serial.println("Loading sailsteer");
        memco_ss=0;
        EVE_cmd_loadimage(memco_ss,EVE_OPT_NODL,Sailsteer_Outer,sizeof(Sailsteer_Outer));
        EVE_cmd_getprops(&memci_ss,&width_ss,&height_ss);
        sprintf(str1_ss,"Load adr: %ul width: %ul height: %ul \n",memci_ss,width_ss,height_ss);
        Serial.print(str1_ss);
        EVE_cmd_loadimage(memci_ss,EVE_OPT_NODL,Sailsteer_Inner,sizeof(Sailsteer_Inner));
        EVE_cmd_getprops(&membt_ss,&width_ss,&height_ss);
        sprintf(str1_ss,"Load adr: %ul width: %ul height: %ul \n",membt_ss,width_ss,height_ss);
        Serial.print(str1_ss);
        EVE_cmd_loadimage(membt_ss,EVE_OPT_NODL,blue_t,sizeof(blue_t));
        EVE_cmd_getprops(&memba_ss,&width_ss,&height_ss);
        sprintf(str1_ss,"Load adr: %ul width: %ul height: %ul \n",memba_ss,width_ss,height_ss);
        Serial.print(str1_ss);
        EVE_cmd_loadimage(memba_ss,EVE_OPT_NODL,blue_a,sizeof(blue_a));
        EVE_cmd_getprops(&memsh_ss,&width_ss,&height_ss);
        sprintf(str1_ss,"Load adr: %ul width: %ul height: %ul \n",memsh_ss,width_ss,height_ss);
        Serial.print(str1_ss);
        EVE_cmd_loadimage(memsh_ss,EVE_OPT_NODL,Sailsteer_Ship,sizeof(Sailsteer_Ship));
        EVE_cmd_getprops(&memfree_ss,&width_ss,&height_ss);
        sprintf(str1_ss,"Next free load adr: %ul  Free memory RAM_G %u\n",memfree_ss,1048572-memfree_ss);
        Serial.print(str1_ss);
        rg_state = SAILST;
        return true;     
        }
        return false; 
}

void  Display_sailsteer(void){  


EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(255,255,255));
EVE_cmd_dl_burst(CLEAR(1,0,0));
        // Outer circle
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memco_ss, EVE_ARGB4, 456U, 456U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        EVE_cmd_dl_burst(VERTEX2F(670/2-456/2,12));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);

        // Inner circle
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memci_ss, EVE_ARGB4, 346, 344U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(346/2,344/2,-mfd_heading*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        EVE_cmd_dl_burst(VERTEX2F(670/2-346/2,480/2-344/2));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);
        // Blue arror True wind
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(membt_ss, EVE_ARGB4, 80U, 80U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(40,40,mfd_TWA*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xpos=670/2+ 480/2*sin((mfd_TWA*PI/180))-80/2-(80/2*(sin(mfd_TWA*PI/180)));
        long ypos=480/2+480/2 *-cos(mfd_TWA*PI/180)-80/2+(80/2*(cos(mfd_TWA*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xpos,ypos));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);

        // Aparent wind direction
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memba_ss, EVE_ARGB4, 260U, 260U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(130,130,mfd_AWA*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xposa=670/2+ 480/2*sin((mfd_AWA*PI/180))-260/2-(260/2*(sin(mfd_AWA*PI/180)));
        long yposa=480/2+480/2 *-cos(mfd_AWA*PI/180)-260/2+(260/2*(cos(mfd_AWA*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xposa,yposa));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);

        // Ship Icon in middle
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memsh_ss, EVE_ARGB4, 100U, 240U);
        EVE_cmd_dl_burst(VERTEX2F(670/2 - 50,480/2-120));
        EVE_cmd_dl_burst(DL_END);

EVE_color_rgb_burst(BLACK);
EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
EVE_cmd_dl(LINE_WIDTH(3U * 16U));
// Square for heading
//       Line from                                      Line to
EVE_cmd_dl_burst(VERTEX2F(670/2-40,60)); EVE_cmd_dl_burst(VERTEX2F(670/2-40,110));
EVE_cmd_dl_burst(VERTEX2F(670/2+40,60)); EVE_cmd_dl_burst(VERTEX2F(670/2+40,110));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,60)); EVE_cmd_dl_burst(VERTEX2F(670/2+40,60));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,110)); EVE_cmd_dl_burst(VERTEX2F(670/2+40,110));
EVE_cmd_dl_burst(DL_END);
EVE_color_rgb_burst(BWHITE);
EVE_cmd_dl_burst(DL_BEGIN|EVE_RECTS);
EVE_cmd_dl_burst(VERTEX2F(670/2-40+3,60+3)); EVE_cmd_dl_burst(VERTEX2F(670/2+40-3,110-3));
EVE_cmd_dl_burst(DL_END);


EVE_color_rgb_burst(BLACK);
EVE_cmd_number_burst(670/2,60,31,EVE_OPT_CENTERX,mfd_heading);
// All corners of screen has an indicator
// Upper left
EVE_cmd_romfont_burst(0,33);
char str1[20];

snprintf(str1,5,"%2.1f",mfd_speed);

EVE_cmd_text_burst(5,0,0,0,str1);
EVE_cmd_text_burst(5,70,29,0,"BSPD");
//Lower left
snprintf(str1,5,"%01u",(int)mfd_TWD);
EVE_cmd_text_burst(5,480-80,0,0,str1);
EVE_cmd_text_burst(5,480-100,29,0,"TWD");
// Upper right
snprintf(str1,5,"%1u",(int)mfd_TWA);
EVE_cmd_text_burst(670-15,0,0,EVE_OPT_RIGHTX,str1);
EVE_cmd_text_burst(670-70,70,29,0,"TWA");
//Lower right
snprintf(str1,5,"%02.1f",mfd_TWS);
EVE_cmd_text_burst(670-15,480-80,0,EVE_OPT_RIGHTX,str1);
EVE_cmd_text_burst(670-70,480-100,29,0,"TWS");

}