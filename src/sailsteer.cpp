#include "EVE.h"
#include "tft.h"
#include "sailsteer.h"
#include "imagemap.h"
#include "display_state.h"

// Sailsteer is page number one i list of pages
extern  uint8_t Circle[21512] PROGMEM; 
extern  uint8_t sbbar[20856] PROGMEM;
extern  uint8_t SmallBlueArrow[5000] PROGMEM;
extern uint8_t BlueArrowAparent[20000] PROGMEM;
extern rg_state_t rg_state;
extern double mfd_heading;

extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_TWA;
extern double mfd_AWA;
extern double mfd_TWS;
extern double mfd_TWD;




void init_sailsteer(void){

        if(rg_state != SAILST){
        EVE_cmd_loadimage(MEM_SBBAR_SS,EVE_OPT_NODL,sbbar,sizeof(sbbar));               
        EVE_cmd_loadimage(MEM_CIRCLE_SS,EVE_OPT_NODL,Circle,sizeof(Circle));
        EVE_cmd_loadimage(MEM_SBA_SS,EVE_OPT_NODL,SmallBlueArrow,sizeof(SmallBlueArrow));
        EVE_cmd_loadimage(MEM_BAA_SS,EVE_OPT_NODL,BlueArrowAparent,sizeof(BlueArrowAparent));
        rg_state = SAILST;     
        }   
}

void  Display_sailsteer(void){  //670*480


EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(255,255,255));
EVE_cmd_dl_burst(CLEAR(1,0,0));
// Outer circle
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(MEM_CIRCLE_SS, EVE_ARGB4, 670U, 480U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        EVE_cmd_dl_burst(VERTEX2F(0,0));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);





// Red green bar on outer circle
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(MEM_SBBAR_SS, EVE_ARGB4, 237U, 44U);
EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
EVE_cmd_dl_burst(CMD_LOADIDENTITY);
EVE_cmd_dl_burst(BITMAP_TRANSFORM_A(0,256));
EVE_cmd_dl_burst(BITMAP_TRANSFORM_E(0,256));
EVE_cmd_dl_burst(BITMAP_SIZE(EVE_NEAREST,EVE_BORDER,EVE_BORDER,237,44));
EVE_cmd_dl_burst(VERTEX2F(670/2-237/2,4));
EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
EVE_cmd_dl_burst(DL_END);
#define MAGXY 1
        // Inner circle
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(MEM_CIRCLE_SS, EVE_ARGB4, 670U, 480U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(670/2,480/2,-mfd_heading*65356/360,0.85*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        EVE_cmd_dl_burst(VERTEX2F((670-670/MAGXY)/2,(480-480/MAGXY)/2));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);
        // Blue arror True wind
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(MEM_SBA_SS, EVE_ARGB4, 50U, 50U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(25,25,mfd_TWA*65356/360,1.5*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xpos=670/2+ 480/2*sin((mfd_TWA*PI/180))-50/2-(50/2*(sin(mfd_TWA*PI/180)));
        long ypos=480/2+480/2 *-cos(mfd_TWA*PI/180)-50/2+(50/2*(cos(mfd_TWA*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xpos,ypos));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);
// temp line from center to insert point
/*EVE_color_rgb_burst(BLACK);
EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
EVE_cmd_dl(LINE_WIDTH(1U * 16U));
EVE_cmd_dl_burst(VERTEX2F(670/2,480/2)); EVE_cmd_dl_burst(VERTEX2F(xpos,ypos));
EVE_cmd_dl_burst(DL_END);*/
// temp end
        // |rect top for heading|

        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(MEM_BAA_SS, EVE_ARGB4, 100U, 100U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(50,50,mfd_AWA*65356/360,1.2*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xposa=670/2+ 480/2*sin((mfd_AWA*PI/180))-100/2-(100/2*(sin(mfd_AWA*PI/180)));
        long yposa=480/2+480/2 *-cos(mfd_AWA*PI/180)-100/2+(100/2*(cos(mfd_AWA*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xposa,yposa));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);

EVE_color_rgb_burst(BLACK);
EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
EVE_cmd_dl(LINE_WIDTH(3U * 16U));

//       Line from                                      Line to
EVE_cmd_dl_burst(VERTEX2F(670/2-40,70)); EVE_cmd_dl_burst(VERTEX2F(670/2-40,120));
EVE_cmd_dl_burst(VERTEX2F(670/2+40,70)); EVE_cmd_dl_burst(VERTEX2F(670/2+40,120));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,70)); EVE_cmd_dl_burst(VERTEX2F(670/2+40,70));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,120)); EVE_cmd_dl_burst(VERTEX2F(670/2+40,120));
EVE_cmd_dl_burst(DL_END);
EVE_cmd_number_burst(670/2,75,30,EVE_OPT_CENTERX,mfd_heading);
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