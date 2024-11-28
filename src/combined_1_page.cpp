#include "EVE.h"
#include "tft.h"
#include "combined1_page.h"
#include "display_state.h"

extern double mfd_heading;

extern  uint8_t CircleCombined1[12800] PROGMEM; // 460800
extern  uint8_t PortArc[1504] PROGMEM; // 11124
extern  uint8_t StarbordArc[1552] PROGMEM; // 11132
extern const uint8_t Inner_circle[11888] PROGMEM;//460800
extern const uint8_t blue_t[1024] PROGMEM;//5408
extern rg_state_t rg_state;

extern double mfd_heading;
extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_TWA;
extern double mfd_AWA;
extern double mfd_TWS;
extern double mfd_TWD;
uint32_t memc_1;
uint32_t mempa_1;
uint32_t memsa_1;
uint32_t membt_1;
uint32_t memba_1;
uint32_t memic_1;
uint32_t memfree;
uint32_t width,height;
char    str1[50];
void init_combined1_page(void){
        if(rg_state != COMB1){
        memc_1=0;
        EVE_cmd_loadimage(memc_1,EVE_OPT_NODL,CircleCombined1,sizeof(CircleCombined1));
        EVE_cmd_getprops(&mempa_1,&width,&height);
        sprintf(str1,"Load adr: %ul width: %ul height: %ul \n",mempa_1,width,height);
        Serial.print(str1);
        EVE_cmd_loadimage(mempa_1,EVE_OPT_NODL,PortArc,sizeof(PortArc));
        EVE_cmd_getprops(&memsa_1,&width,&height);
        sprintf(str1,"Load adr: %ul width: %ul height: %ul \n",memsa_1,width,height);
        Serial.print(str1);
        EVE_cmd_loadimage(memsa_1,EVE_OPT_NODL,StarbordArc,sizeof(StarbordArc));
        EVE_cmd_getprops(&membt_1,&width,&height);
        sprintf(str1,"Load adr: %ul width: %ul height: %ul \n",membt_1,width,height);
        Serial.print(str1);
        EVE_cmd_loadimage(membt_1,EVE_OPT_NODL,blue_t,sizeof(blue_t));
        EVE_cmd_getprops(&memic_1,&width,&height);
        sprintf(str1,"Load adr: %ul width: %ul height: %ul \n",memic_1,width,height);
        Serial.print(str1);
        EVE_cmd_loadimage(memic_1,EVE_OPT_NODL,Inner_circle,sizeof(Inner_circle));
        EVE_cmd_getprops(&memfree,&width,&height);
        sprintf(str1,"Next free load adr: %ul  Free memory RAM_G %u\n",memfree,1048572-memfree);
        Serial.print(str1);


  


  
        rg_state = COMB1;
        }      
}
       
void  Display_combined1_Page(void){ 
//EVE_cmd_dl_burst(CMD_DLSTART);
EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(255,255,255));
EVE_cmd_dl_burst(CLEAR(1,0,0));
        


EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(memc_1, EVE_ARGB4, 480U, 480U);
//EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
//EVE_cmd_dl_burst(CMD_LOADIDENTITY);
//EVE_cmd_rotatearound_burst(670/2,480,-mfd_heading*65356/360,65536*2);
//EVE_cmd_translate_burst(0, 480/2*65536);
//EVE_cmd_dl_burst(CMD_SETMATRIX);
EVE_cmd_dl_burst(VERTEX2F(670-480,0));
//EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
EVE_cmd_dl_burst(DL_END);

EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(memic_1, EVE_ARGB4, 480U, 480U);
EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
EVE_cmd_dl_burst(CMD_LOADIDENTITY);
EVE_cmd_rotatearound_burst(480/2,480/2,-mfd_heading*65356/360,65536*1);
//EVE_cmd_translate_burst(0, 480/2*65536);
EVE_cmd_dl_burst(CMD_SETMATRIX);
EVE_cmd_dl_burst(VERTEX2F(670-480,0));
EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
EVE_cmd_dl_burst(DL_END);

EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(memsa_1, EVE_ARGB4, 104U, 54U);
//EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
//EVE_cmd_dl_burst(CMD_LOADIDENTITY);
//EVE_cmd_rotatearound_burst(670/2,480,-mfd_heading*65356/360,65536*2);
//EVE_cmd_translate_burst(0, 480/2*65536);
//EVE_cmd_dl_burst(CMD_SETMATRIX);
EVE_cmd_dl_burst(VERTEX2F(432,28));
//EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
EVE_cmd_dl_burst(DL_END);
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(mempa_1, EVE_ARGB4, 103U, 54U);
//EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
//EVE_cmd_dl_burst(CMD_LOADIDENTITY);
//EVE_cmd_rotatearound_burst(670/2,480,-mfd_heading*65356/360,65536*2);
//EVE_cmd_translate_burst(0, 480/2*65536);
//EVE_cmd_dl_burst(CMD_SETMATRIX);
EVE_cmd_dl_burst(VERTEX2F(324,28));
//EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
EVE_cmd_dl_burst(DL_END);

// Blue triangle True wind        
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(membt_1, EVE_ARGB4, 80U, 80U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(40,40,mfd_TWD*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xpos=190+480/2+ 480/2*sin((mfd_TWD*PI/180))-80/2-(80/2*(sin(mfd_TWD*PI/180)));
        long ypos=480/2+480/2 *-cos(mfd_TWD*PI/180)-80/2+(80/2*(cos(mfd_TWD*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xpos,ypos));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);

EVE_color_rgb_burst(BLACK);
char str1[20];
EVE_cmd_romfont_burst(0,32);
EVE_cmd_romfont_burst(1,34);
EVE_cmd_text_burst(5,10,29,0,"BSPD    kn");
snprintf(str1,5,"%02.1f",mfd_speed);
EVE_cmd_text_burst(10,40,1,0,str1);

//Mid left

snprintf(str1,5,"%02.1f",mfd_depth);
EVE_cmd_text_burst(10,200,1,0,str1);
EVE_cmd_text_burst(5,170,29,0,"Depth   m");

//Lower left
snprintf(str1,5,"%02.1f",mfd_TWS);
EVE_cmd_text_burst(10,370,1,0,str1);
EVE_cmd_text_burst(5,330,29,0,"TWS    m/s");


EVE_color_rgb_burst(BLACK);
EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
EVE_cmd_dl(LINE_WIDTH(2U * 16U));

//       Line from                                      Line to
EVE_cmd_dl_burst(VERTEX2F(190+480/2-40,80-4)); EVE_cmd_dl_burst(VERTEX2F(190+480/2-40,130-4));
EVE_cmd_dl_burst(VERTEX2F(190+480/2+40,80-4)); EVE_cmd_dl_burst(VERTEX2F(190+480/2+40,130-4));
EVE_cmd_dl_burst(VERTEX2F(190+480/2-40,80-4)); EVE_cmd_dl_burst(VERTEX2F(190+480/2+40,80-4));
EVE_cmd_dl_burst(VERTEX2F(190+480/2-40,130-4)); EVE_cmd_dl_burst(VERTEX2F(190+480/2+40,130-4));
EVE_cmd_dl_burst(VERTEX2F(0,160)); EVE_cmd_dl_burst(VERTEX2F(150,160));
EVE_cmd_dl_burst(VERTEX2F(0,320)); EVE_cmd_dl_burst(VERTEX2F(150,320));
EVE_cmd_dl_burst(DL_END);

EVE_cmd_dl(DL_BEGIN|EVE_RECTS);
EVE_color_rgb_burst(WHITE);
EVE_cmd_dl(LINE_WIDTH(1U * 16U));
EVE_cmd_dl(VERTEX2F(190+480/2-38,82-4));
EVE_cmd_dl(VERTEX2F(190+480/2+38,128-4));
EVE_cmd_dl_burst(DL_END);
EVE_color_rgb_burst(BLACK);
EVE_cmd_number_burst(190+480/2,82,31,EVE_OPT_CENTERX,mfd_heading);
    } 



