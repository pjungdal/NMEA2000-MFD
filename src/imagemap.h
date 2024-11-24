#define MEM_PAGE  0
#define MEM_MENU  24000

#define MEM_START 120000
// Page specific
//Steering page
#define MEM_CIRCLE 48000 // 643200
#define MEM_SBBAR 691200
#define MEM_GREENA 712056
#define MEM_REDA MEM_GREENA+12800
// Compass page
//Sailsteer page
#define MEM_OUTER_SS MEM_START
#define MEM_INNER_SS MEM_OUTER_SS+SIZE_MEM_OUTER_SS
#define MEM_SBA_SS MEM_INNER_SS+SIZE_MEM_INNER_SS
#define MEM_BAA_SS MEM_SBA_SS+SIZE_MEM_SBA_SS

//Combined_1
#define MEM_CIRCLE_C1 48000 // size 460800
#define MEM_SBARC MEM_CIRCLE_C1+460800 //11132
#define MEM_PORTARC MEM_SBARC+11132 //11124
#define MEM_INNER_C MEM_PORTARC+11124 //460800
#define MEM_BLUET MEM_INNER_C + 460800
#define MEM_BLUEA MEM_BLUET +12800 //135200 260x260
// Sum 37284 over limit
//Combined_2
#define MEM_CIRCLE_C2 48000 // size 460800
#define MEM_SBARC_C2 MEM_CIRCLE_C2+460800 //11132
#define MEM_PORTARC_C2 MEM_SBARC_C2+11132 //11124
#define MEM_BLUET_C2 MEM_PORTARC_C2+11132
#define MEM_BLUEA_C2 MEM_BLUET_C2 +12800 //135200 260x260


// Size definitions for bitmaps
// Max memory 1048572( 1024kb ) upper 42kb is used for decoding PNG formatted bitmaps
// Maybe CMD_GETPROPS can be used to optimize memory consumption
#define SIZE_MEM_PAGE       24000
#define SIZE_MEM_UP         24000
#define SIZE_MEM_RERTURN    24000
#define SIZE_MEM_DOWN       24000
#define SIZE_MEM_MENU       24000
//First free memory location after static MENU maps = 120000


//Sailsteer page
#define SIZE_MEM_OUTER_SS  415872 // 456*456 ARGB4
#define SIZE_MEM_INNER_SS  2*346*344 //346*344 238048
#define SIZE_MEM_SBA_SS     5000
#define SIZE_MEM_BAA_SS     20000
// Memory 415072+238048+5000+20000+120000 = 798120

//Steering (Autopilot)
#define SIZE_MEM_REDA      12800
#define SIZE_MEM_GREENA      12800
#define SIZE_MEM_CIRCLE     643200
#define SIZE_MEM_BARROW     "NA"
#define SIZE_MEM_SBBAR      20856
//Combined_1
#define SIZE_MEM_CIRCLE_C1  460800
#define SIZE_MEM_SBARC      11132
#define SIZE_MEM_PORTARC    11124
#define SIZE_MEM_INNER_C    406800
#define SIZE_MEM_BLUET      12800
#define SIZE_MEM_BLUEA      135200



#define MAX_LIMIT 1048572
