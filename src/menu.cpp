#include "EVE.h"
#include "tft.h"
#include "touch.h"
#include "Preferences.h"

extern bool pmquit,pmstore;
extern uint8_t menuselect;
extern uint8_t Selectedpage;
extern uint8_t pmpage,pmfunction;
extern uint8_t pages[10];
extern void readsettings();
extern void writesettings();

void display_pagemenu(void){



if (pmquit) {readsettings();pmquit=false; menuselect=0; Selectedpage =0; return;}
if (pmstore){{(pmpage<9) ? pages[pmpage+1]=0:pmpage++;}writesettings();pmquit=false; menuselect=0; Selectedpage =0;return; }
EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(125,125,125));
EVE_cmd_dl_burst(CLEAR(1,0,0));


    EVE_color_rgb_burst(DARKGREY);
    EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
    EVE_cmd_dl_burst(VERTEX2F(50,60));
    EVE_cmd_dl_burst(VERTEX2F(80,90));
    EVE_cmd_dl_burst(VERTEX2F(240,60));
    EVE_cmd_dl_burst(VERTEX2F(450,90));
    EVE_cmd_dl_burst(DL_END); 

    EVE_color_rgb_burst(WHITE);
        EVE_cmd_number(64, 75, 28, EVE_OPT_CENTER, pmpage); // Page number


    pmfunction = pages[pmpage];

        char pmtext[20];
        switch (pmfunction)
        {
        case 1:
            sprintf(pmtext,"Sailsteer");
            break;
        case 2:
            sprintf(pmtext,"Digital 1");
            break;
        case 3:
            sprintf(pmtext,"Navi Autopilot");
            break;
        case 4:
            sprintf(pmtext,"Digital 2");
            break;
        case 5:
            sprintf(pmtext,"Combined 1");
            break;
        case 6:
            sprintf(pmtext,"Navigate GPS");
            break;
        case 7:
            sprintf(pmtext,"Combined 2");
            break;
        case 8:
            sprintf(pmtext,"Pilot controller");
            break;
        case 9:
            sprintf(pmtext,"Empty");
            break;

        
        default:
            break;
        }
        EVE_cmd_text(346, 75, 28, EVE_OPT_CENTER, pmtext);
        EVE_cmd_dl(DL_TAG + 165U);
        EVE_cmd_button(500, 46, 120, 50, 27, 0, "Store");
        EVE_cmd_dl(DL_TAG + 170U);
        EVE_cmd_button(500, 120, 120, 50, 27, 0, "Quit");
        EVE_cmd_text(290, 18, 28, EVE_OPT_CENTER, "Function");
        EVE_cmd_text(50, 18, 28, 0, "Page");
        EVE_cmd_dl(DL_TAG + 160U);
        EVE_cmd_button(0, 120, 100, 50, 27, 0, "Prev");
        EVE_cmd_dl(DL_TAG + 161U);
        EVE_cmd_button(110, 120, 100, 50, 27, 0, "Next");
        EVE_cmd_dl(DL_TAG + 162U);
        EVE_cmd_button(240, 120, 100, 50, 27, 0, "Prev");
        EVE_cmd_dl(DL_TAG + 163U);
        EVE_cmd_button(350, 120, 100, 50, 27, 0, " Next");  
    EVE_color_rgb_burst(LIGHTGREY);


}