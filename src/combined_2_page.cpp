#include "EVE.h"
#include "tft.h"
#include "combined2_page.h"
#include "display_state.h"

extern double mfd_heading;

extern  uint8_t CircleCombined1[12800] PROGMEM; // 460800
extern  uint8_t PortArc[1504] PROGMEM; // 11124
extern  uint8_t StarbordArc[1552] PROGMEM; // 11132
extern const uint8_t blue_t[1024] PROGMEM;
extern const uint8_t blue_a[3552] PROGMEM;
extern double mfd_heading;
extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_TWA;
extern double mfd_AWA;
extern double mfd_AWS;
extern double mfd_TWS;
extern double mfd_TWD;
uint32_t memc_2;
uint32_t mempa_2;
uint32_t memsa_2;
uint32_t membt_2;
uint32_t memba_2;
uint32_t memfree_2;
uint32_t width_2,height_2;
char    str1_2[50];
extern void spinner(void);
bool init_combined2_page(void){
        if(rg_state != COMB2){
                spinner();
        Serial.println("Loading combined 2");

        memc_2=0;
        EVE_cmd_loadimage(memc_2,EVE_OPT_NODL,CircleCombined1,sizeof(CircleCombined1));
        EVE_cmd_getprops(&mempa_2,&width_2,&height_2);
        sprintf(str1_2,"Load adr: %ul width: %ul height: %ul \n",mempa_2,width_2,height_2);
        Serial.print(str1_2);
        EVE_cmd_loadimage(mempa_2,EVE_OPT_NODL,PortArc,sizeof(PortArc));
        EVE_cmd_getprops(&memsa_2,&width_2,&height_2);
        sprintf(str1_2,"Load adr: %ul width: %ul height: %ul \n",memsa_2,width_2,height_2);
        Serial.print(str1_2);
        EVE_cmd_loadimage(memsa_2,EVE_OPT_NODL,StarbordArc,sizeof(StarbordArc));
        EVE_cmd_getprops(&membt_2,&width_2,&height_2);
        sprintf(str1_2,"Load adr: %ul width: %ul height: %ul \n",membt_2,width_2,height_2);
        Serial.print(str1_2);
        EVE_cmd_loadimage(membt_2,EVE_OPT_NODL,blue_t,sizeof(blue_t));
        EVE_cmd_getprops(&memba_2,&width_2,&height_2);
        sprintf(str1_2,"Load adr: %ul width: %ul height: %ul \n",memba_2,width_2,height_2);
        Serial.print(str1_2);
        EVE_cmd_loadimage(memba_2,EVE_OPT_NODL,blue_a,sizeof(blue_a));
        EVE_cmd_getprops(&memfree_2,&width_2,&height_2);
        sprintf(str1_2,"Next free load adr: %ul  Free memory RAM_G %u\n",memfree_2,1048572-memfree_2);
        Serial.print(str1_2);

        rg_state = COMB2;
        return true;
        } 
        return false;  
}
       
void  Display_combined2_Page(void){ 
EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(255,255,255));
EVE_cmd_dl_burst(CLEAR(1,0,0));
        


EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(memc_2, EVE_ARGB4, 480U, 480U);

EVE_cmd_dl_burst(VERTEX2F(670-480,0));
EVE_cmd_dl_burst(DL_END);


EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(memsa_2, EVE_ARGB4, 104U, 54U);

EVE_cmd_dl_burst(VERTEX2F(432,28));

EVE_cmd_dl_burst(DL_END);
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(mempa_2, EVE_ARGB4, 103U, 54U);

EVE_cmd_dl_burst(VERTEX2F(324,28));
EVE_cmd_dl_burst(DL_END);
// Blue triangle True wind        
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(membt_2, EVE_ARGB4, 80U, 80U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(40,40,mfd_TWD*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xpos=190+480/2+ 480/2*sin((mfd_TWD*PI/180))-80/2-(80/2*(sin(mfd_TWD*PI/180)));
        long ypos=480/2+480/2 *-cos(mfd_TWD*PI/180)-80/2+(80/2*(cos(mfd_TWD*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xpos,ypos));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);
// Blue triangle Apparent wind        
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memba_2, EVE_ARGB4, 260U, 260U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(260/2,260/2,mfd_TWA*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xposa=190+480/2+ 480/2*sin((mfd_TWA*PI/180))-260/2-(260/2*(sin(mfd_TWA*PI/180)));
        long yposa=480/2+480/2 *-cos(mfd_TWA*PI/180)-260/2+(260/2*(cos(mfd_TWA*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xposa,yposa));
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
snprintf(str1,5,"%02.1f",mfd_AWS);
EVE_cmd_text_burst(10,370,1,0,str1);
EVE_cmd_text_burst(5,330,29,0,"AWS    m/s");


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


EVE_cmd_number_burst(190+480/2,82,31,EVE_OPT_CENTERX,mfd_heading);
    } 



