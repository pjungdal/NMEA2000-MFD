    
    #include "EVE.h"
    #include "tft.h"

    void spinner(void){

EVE_cmd_dl_burst(CMD_DLSTART);
EVE_cmd_dl_burst(DL_CLEAR_COLOR_RGB | BLACK);
EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_color_rgb_burst(WHITE);
    EVE_cmd_spinner_burst(670/2,480/2,0,0);
    EVE_end_cmd_burst();
    }