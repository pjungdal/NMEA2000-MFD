


#include "EVE.h"
#include "tft.h"
#include "steering_page.h"
#include "display_state.h"

extern double mfd_heading;
extern double mfd_XTE;
extern double WPTBrg;
extern double Cog;
extern double mfd_commandedheading;
extern double mfd_commandedangle;
extern unsigned char Engaged; 
extern  unsigned int  mfd_APMode;
extern  unsigned char mfd_APStatus;
extern double mfd_HeadingToSteer;
extern double mfd_angle_order;
extern double mfd_SetRudder;

extern  uint8_t Circle[21512] PROGMEM; 
extern  uint8_t sbbar[20856] PROGMEM;
extern const uint8_t green_arrow[1000] PROGMEM;
extern const uint8_t red_arrow[1000] PROGMEM;
extern rg_state_t rg_state;

uint32_t memc_st;
uint32_t memsb_st;
uint32_t memga_st;
uint32_t memra_st;
uint32_t memfree_st;
uint32_t width_st,height_st;
char    str1_st[50];
extern void spinner(void);


bool init_steering_page(void){
        if(rg_state != STEER){
        spinner();
        Serial.println("Loading steering");

        memc_st=0;
                sprintf(str1_st,"Circle Load adr: %ul width: %ul height: %ul \n",memc_st,width_st,height_st); Serial.print(str1_st);
        EVE_cmd_loadimage(memc_st,EVE_OPT_NODL,Circle,sizeof(Circle));
        
                EVE_cmd_getprops(&memga_st,&width_st,&height_st);
                sprintf(str1_st,"Green arrow Load adr: %ul width: %ul height: %ul \n",memga_st,width_st,height_st); Serial.print(str1_st);
        EVE_cmd_loadimage(memga_st,EVE_OPT_NODL,green_arrow,sizeof(green_arrow));
        
                EVE_cmd_getprops(&memra_st,&width_st,&height_st);
                sprintf(str1_st,"Red arrow Load adr: %ul width: %ul height: %ul \n",memra_st,width_st,height_st); Serial.print(str1_st);
        EVE_cmd_loadimage(memra_st,EVE_OPT_NODL,red_arrow,sizeof(red_arrow));
        // Note Something makes the loading of sbbar the next EVE_cmd_getprops return 0. Placing it last in the list of loads solves the problem (temporarily)
                EVE_cmd_getprops(&memsb_st,&width_st,&height_st);
                sprintf(str1_st,"SB bar Load adr: %ul width: %ul height: %ul \n",memsb_st,width_st,height_st); Serial.print(str1_st);
        EVE_cmd_loadimage(memsb_st,EVE_OPT_NODL,sbbar,sizeof(sbbar));
        
                EVE_cmd_getprops(&memfree_st,&width_st,&height_st);
                sprintf(str1_st,"Next free load adr: %ul  Free memory RAM_G %u\n",memfree_st,1048572-memfree_st); Serial.print(str1_st);


        rg_state = STEER;
        return true;
        }             
        return false;  
}
       
void  Display_Steering_Page(void){  //670*480
char str1[20];

EVE_cmd_dl_burst(SCISSOR_SIZE(670,480));
EVE_cmd_dl_burst(CLEAR_COLOR_RGB(255,255,255));
EVE_cmd_dl_burst(CLEAR(1,0,0));

EVE_color_rgb_burst(BLACK);
EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
EVE_cmd_dl(LINE_WIDTH(3U * 16U));
EVE_cmd_dl(VERTEX2F(670/2-40, 10));
EVE_cmd_dl(VERTEX2F(670/2+40, 35));
EVE_color_rgb_burst(LIGHTGREY);
EVE_cmd_dl(VERTEX2F(175, 350));
EVE_cmd_dl(VERTEX2F(453+50, 460));
EVE_cmd_dl(DL_END);

        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memc_st, EVE_ARGB4, 670U, 480U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(670/2,480,-mfd_heading*65356/360,65536*2);
        EVE_cmd_translate_burst(0, 480/2*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        EVE_cmd_dl_burst(VERTEX2F(0,0));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);




EVE_color_rgb_burst(BLACK);
EVE_cmd_dl_burst(DL_BEGIN|EVE_LINES);
EVE_cmd_dl(LINE_WIDTH(2U * 16U));
EVE_cmd_dl_burst(VERTEX2F(50, 200));
EVE_cmd_dl_burst(VERTEX2F(630, 200));
EVE_cmd_dl_burst(VERTEX2F(225, 225));
EVE_cmd_dl_burst(VERTEX2F(225, 325));
EVE_cmd_dl_burst(VERTEX2F(453, 225));
EVE_cmd_dl_burst(VERTEX2F(453, 325));
//      |
EVE_cmd_dl_burst(VERTEX2F(175, 350));
EVE_cmd_dl_burst(VERTEX2F(175, 460));
//                              |
EVE_cmd_dl_burst(VERTEX2F(453+50, 350));
EVE_cmd_dl_burst(VERTEX2F(453+50, 460));
//_______
EVE_cmd_dl_burst(VERTEX2F(50, 340));
EVE_cmd_dl_burst(VERTEX2F(210, 340));
// _________
EVE_cmd_dl_burst(VERTEX2F(240, 340));
EVE_cmd_dl_burst(VERTEX2F(438, 340));
//  _________
EVE_cmd_dl_burst(VERTEX2F(468, 340));
EVE_cmd_dl_burst(VERTEX2F(630, 340));




// |rect top for heading|
EVE_color_rgb_burst(WHITE);
EVE_cmd_dl(LINE_WIDTH(3U * 16U));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,0));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,35));
EVE_cmd_dl_burst(VERTEX2F(670/2+40,0));
EVE_cmd_dl_burst(VERTEX2F(670/2+40,35));

EVE_color_rgb_burst(RED);
EVE_cmd_dl(LINE_WIDTH(4U * 16U));
EVE_cmd_dl(VERTEX2F(670/2, 350));
EVE_cmd_dl(VERTEX2F(670/2, 460));
EVE_cmd_dl_burst(DL_END);
EVE_color_rgb_burst(BLACK);

EVE_cmd_romfont(1,33);

if (Cog >=0 && Cog <=360) EVE_cmd_number_burst(550,230,1,EVE_OPT_CENTERX,Cog); else EVE_cmd_text_burst(550, 230, 1, EVE_OPT_CENTERX, "---");
EVE_cmd_text_burst(463, 200, 29, 0, "COG");
if (WPTBrg >=0 && WPTBrg <=360) EVE_cmd_number_burst(130,230,1,EVE_OPT_CENTERX,WPTBrg); else EVE_cmd_text_burst(110, 230, 1, EVE_OPT_CENTERX, "---");
EVE_cmd_text_burst(50, 200, 29, 0, "WPT BRG");
char lr =' ' ;
if(mfd_XTE <0 ) lr='H'; else lr='V';
if (mfd_XTE >=-10000 && WPTBrg <=10000){
snprintf(str1,5,"%2.1f%c",abs(mfd_XTE),lr);
EVE_cmd_text_burst(350,230,1,EVE_OPT_CENTERX,str1);
}
else EVE_cmd_text_burst(300, 230, 1, 0, "---");
EVE_cmd_text_burst(291, 200, 29, EVE_OPT_CENTERX, "XTE    m");
EVE_color_rgb_burst(WHITE);

//Red green labels
EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
EVE_cmd_setbitmap_burst(memsb_st, EVE_ARGB4, 237U, 44U);
EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
EVE_cmd_dl_burst(CMD_LOADIDENTITY);
EVE_cmd_dl_burst(BITMAP_TRANSFORM_A(0,128));
EVE_cmd_dl_burst(BITMAP_TRANSFORM_E(0,128));
EVE_cmd_dl_burst(BITMAP_SIZE(EVE_NEAREST,EVE_BORDER,EVE_BORDER,237*2,44*2));
EVE_cmd_dl_burst(VERTEX2F(670/2 - 237,4));
EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);


EVE_color_rgb_burst(BLACK);
EVE_cmd_dl(DL_BEGIN | EVE_RECTS);
EVE_cmd_dl(LINE_WIDTH(3U * 16U));
EVE_cmd_dl(VERTEX2F(670/2-40, 6));
EVE_cmd_dl(VERTEX2F(670/2+40, 35));
// |rect top for heading|
EVE_color_rgb_burst(WHITE);
EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
EVE_cmd_dl(LINE_WIDTH(3U * 16U));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,0));
EVE_cmd_dl_burst(VERTEX2F(670/2-40,35));
EVE_cmd_dl_burst(VERTEX2F(670/2+40,0));
EVE_cmd_dl_burst(VERTEX2F(670/2+40,35));
EVE_cmd_dl_burst(DL_END);
EVE_cmd_number_burst(670/2,5,30,EVE_OPT_CENTERX,mfd_heading);
//Serial.print("C: "); Serial.print(mfd_commandedheading); Serial.print(" H: "); Serial.println(mfd_heading); 
if((mfd_commandedheading - mfd_heading)>-0.01){
// green triangle right 
//Serial.print("R: "); Serial.println(mfd_commandedheading-mfd_heading) ; Serial.print("+180: ");Serial.println(mfd_commandedheading-mfd_heading+180) ; 

        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memga_st, EVE_ARGB4, 80U, 80U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(40,40,(mfd_commandedheading - mfd_heading + 180)*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xpos=670/2+460*sin(((mfd_commandedheading - mfd_heading)*PI/180))-80/2-(80/2*(sin((mfd_commandedheading - mfd_heading)*PI/180)));
        long ypos=480+460 *-cos((mfd_commandedheading - mfd_heading)*PI/180)-80/2+(80/2*(cos((mfd_commandedheading - mfd_heading)*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xpos,ypos));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);
}
else {
// red triangle left     
//Serial.print("L: ");      Serial.println(mfd_commandedheading-mfd_heading+360) ; Serial.print("+180: ");      Serial.println(mfd_commandedheading-mfd_heading+180) ; 
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(memra_st, EVE_ARGB4, 80U, 80U);
        EVE_cmd_dl_burst(DL_SAVE_CONTEXT);
        EVE_cmd_dl_burst(CMD_LOADIDENTITY);
        EVE_cmd_rotatearound_burst(40,40,(mfd_commandedheading - mfd_heading+180)*65356/360,1*65536);
        EVE_cmd_dl_burst(CMD_SETMATRIX);
        long xposa=670/2+460 * sin(((mfd_commandedheading-mfd_heading+360)*PI/180))-80/2-(80/2*(sin((mfd_commandedheading-mfd_heading+360)*PI/180)));
        long yposa=480+460 *-cos((mfd_commandedheading-mfd_heading+360)*PI/180)  -80/2+(80/2*(cos((mfd_commandedheading-mfd_heading+360)*PI/180)));
        EVE_cmd_dl_burst(VERTEX2F(xposa,yposa));
        EVE_cmd_dl_burst(DL_RESTORE_CONTEXT);
        EVE_cmd_dl_burst(DL_END);
}

}