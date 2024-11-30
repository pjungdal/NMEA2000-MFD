

#include "EVE.h"
#include "tft.h"
#include "speeddepth.h"
#include "display_state.h"


extern rg_state_t rg_state;
extern double mfd_heading;

extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_TWA;
extern double mfd_AWA;
extern double mfd_TWS;
extern double mfd_TWD;

bool init_digital(void){        return false;}

void display_digital(void){
EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(255,255,255));
EVE_cmd_dl_burst(CLEAR(1,0,0));

        EVE_color_rgb_burst(BLACK);
        EVE_cmd_romfont(0,34);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
        EVE_cmd_dl_burst(VERTEX2F(0,160));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-120,160));
        EVE_cmd_dl_burst(VERTEX2F(0,320));
        EVE_cmd_dl_burst(VERTEX2F(EVE_HSIZE-120,320));
        EVE_cmd_dl_burst(DL_END);


        EVE_cmd_text_burst(1, 0, 30, 0, "BOAT SPD");
        EVE_cmd_text_burst(600, 0, 30, 0, "kn");
        EVE_cmd_text_burst(0, 160, 30, 0, "DEPTH");
        EVE_cmd_text_burst(600, 160, 30, 0, "m");
        EVE_cmd_text_burst(0, 320, 30, 0, "HEADING");
        EVE_cmd_text_burst(600, 320, 30, 0, "mag");


        EVE_cmd_romfont(0, 34);
char str1[20];
snprintf(str1,5,"%1.1f",mfd_speed);
        EVE_cmd_text_burst((EVE_HSIZE-120)/2, EVE_VSIZE/3 -130, 0, EVE_OPT_CENTERX, str1);
snprintf(str1,5,"%1.1f",mfd_depth);
        EVE_cmd_text_burst((EVE_HSIZE-120)/2, EVE_VSIZE*2/3 -130, 0, EVE_OPT_CENTERX, str1);
snprintf(str1,5,"%0.f",mfd_heading);
        EVE_cmd_text_burst((EVE_HSIZE-120)/2, EVE_VSIZE -130, 0, EVE_OPT_CENTERX, str1);
        
}