



#include "N2kMsg.h"
#include "state.h"


ap_mode_t mode=A;
ap_mode_t previous_mode;
ap_state_t state=S;
extern double mfd_heading;
extern double mfd_speed;
extern double mfd_depth;
extern double mfd_rudder;
extern double mfd_AWA;
extern double mfd_commandedheading;
extern double mfd_commandedangle;


bool comparecommand(const char *Command,const char *Want,unsigned char len){
for (int i=0;i<len;i++) if(Command[i]!=Want[i]) return false;
return true;
}
bool apstate(const char * Command){
char str[8];
    switch (state)
    {
    case S: 
    
    if(comparecommand(Command,"\x02\x0E\x00\xFF\xFF\xFF",6)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage Fu "); 
        #endif
            state=E;mode=FU;}
         else if(comparecommand(Command,"\x02\x1A\x00\x03\xAE\x00",6)){ 
         #ifdef MSG_PRINT_REC
            Serial.println("Fu > "); 
         #endif
            state=E;mode=FU;}
          else if(comparecommand(Command,"\x02\x1A\x00\x02\xAE\x00",6)){ 
          #ifdef MSG_PRINT_REC
            Serial.println("Fu < "); 
          #endif
            state=E;mode=FU;}
           else if(comparecommand(Command,"\x0A\x0F\x00",3)){ 
            mfd_commandedangle=mfd_AWA;
           #ifdef MSG_PRINT_REC
            Serial.println("Engage Wind ");
           #endif
            state=E;mode=W; previous_mode=mode;}
            else if(comparecommand(Command,"\x0A\x06\x00",3)){ 
            #ifdef MSG_PRINT_REC
             Serial.println("STBY  ");
            #endif
            state=S;mode=previous_mode; }
             else if(comparecommand(Command,"\x0A\x10\x00",3)){ 

              mfd_commandedangle=mfd_AWA;
              mfd_commandedheading=mfd_heading;
             #ifdef MSG_PRINT_REC
                Serial.println("Engage  ");
             #endif
             }
              else if(comparecommand(Command,"\x0A\x09\x00",3)){
                mfd_commandedheading=mfd_heading; 
              #ifdef MSG_PRINT_REC
                Serial.println("Engage A ");
              #endif
              state=E;mode=A; previous_mode=mode; }
                else if(comparecommand(Command,"\x0A\x0A\x00",3)){ 
                #ifdef MSG_PRINT_REC
                Serial.println("Engage NAV ");
                #endif
                state=E;mode=NAV;}
                    else if(comparecommand(Command,"\x0A\x0C\x00",3)){ 
                    #ifdef MSG_PRINT_REC
                    Serial.println("Engage Nd "); 
                    #endif
                    state=E;mode=ND;previous_mode=mode;}
                        else if(comparecommand(Command,"\x02\x0D\x00\x05",4)){ 
                        #ifdef MSG_PRINT_REC
                        Serial.println("Engage Nfu > ");
                         #endif
                        state=E;mode=NFU;}
                            else if(comparecommand(Command,"\x02\x0D\x00\x04",4)){ 
                                #ifdef MSG_PRINT_REC
                                Serial.println("Engage Nfu < "); 
                                #endif
                            state=E;mode=NFU;}
                                else if(comparecommand(Command,"\x02\x0D\x00\x0FF",4)){
                                    #ifdef MSG_PRINT_REC 
                                    Serial.println("Nfu stop command "); 
                                    #endif
                                state=E;mode=NFU;}
                                    else if(comparecommand(Command,"\x00A\x1A\x00\x03\xAE\x00",6)){ 
                                        #ifdef MSG_PRINT_REC
                                        Serial.println("Nfu 1> "); 
                                        #endif
                                    state=E;mode=NFU;}
                                        else if(comparecommand(Command,"\x00A\x1A\x00\x03\xD1\x06",6)){ 
                                            #ifdef MSG_PRINT_REC
                                            Serial.println("Nfu 10>> ");
                                            #endif
                                        state=E;mode=NFU;}
                                            else if(comparecommand(Command,"\x00A\x1A\x00\x02\xAE\x00",6)){ 
                                                #ifdef MSG_PRINT_REC
                                                    Serial.println("Nfu <1 "); 
                                                #endif
                                            state=E;mode=NFU;}
                                                else if(comparecommand(Command,"\x00A\x1A\x00\x02\xD1\x06",6)){ 
                                                    #ifdef MSG_PRINT_REC
                                                        Serial.println("Nfu <<10 "); 
                                                    #endif
                                                state=E;mode=NFU;}
                                                    else {
                                                        Serial.print("Unknown: ");for (int i=0;i<8;i++) {Serial.print(Command[i],HEX);Serial.print(" ");};Serial.println();
                                                    }
#ifdef MSG_PRINT_REC
Serial.print("Command decoder: state="); Serial.print(state);Serial.print(" mode=");Serial.println(mode);
#endif

        break;
    case E:

     if(comparecommand(Command,"\x02\x0E\x00\xFF\xFF\xFF",6)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage Fu "); 
        #endif
            state=E;mode=FU;}
        else if(comparecommand(Command,"\x02\x1A\x00\x03\xAE\x00",6)){ 
            #ifdef MSG_PRINT_REC
                Serial.println("Fu > "); 
            #endif
        state=E;mode=FU;}
            else if(comparecommand(Command,"\x02\x1A\x00\x02\xAE\x00",6)){ 
                #ifdef MSG_PRINT_REC
                    Serial.println("Fu < "); 
                #endif
            state=E;mode=FU;}
                else if(comparecommand(Command,"\x0A\x0F\x00",3)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage Wind ");
        #endif

            state=E;mode=W; previous_mode=mode;}
                    else if(comparecommand(Command,"\x0A\x06\x00",3)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("STBY  ");
        #endif

            state=S; mode=previous_mode;}
                        else if(comparecommand(Command,"\x0A\x10\x00",3)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage  ");
        #endif

             }
                            else if(comparecommand(Command,"\x0A\x09\x00",3)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage A ");
        #endif

            state=E;mode=A; previous_mode=mode;}
                                else if(comparecommand(Command,"\x0A\x0A\x00",3)){
        #ifdef MSG_PRINT_REC 
            Serial.println("Engage NAV ");
        #endif
 
            state=E;mode=NAV;}
                                    else if(comparecommand(Command,"\x0A\x0C\x00",3)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage Nd "); 
        #endif

            state=E;mode=ND;previous_mode=mode;}
                                        else if(comparecommand(Command,"\x02\x0D\x00\x05",4)){
        #ifdef MSG_PRINT_REC 
            Serial.println("Engage Nfu > "); 
        #endif

            state=E;mode=NFU;}
                                            else if(comparecommand(Command,"\x02\x0D\x00\x04",4)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Engage Nfu < "); 
        #endif

            state=E;mode=NFU;}
                                                else if(comparecommand(Command,"\x02\x0D\x00\x0FF",4)){ 
        #ifdef MSG_PRINT_REC
            Serial.println("Nfu stop command "); 
        #endif

            state=E;mode=NFU;}
                                                    else if(comparecommand(Command,"\x00A\x1A\x00\x03\xAE\x00",6)){ 

            mfd_commandedangle += 1;
            mfd_commandedheading += 1;
            mfd_commandedheading=(mfd_commandedheading >360) ?  mfd_commandedheading = mfd_commandedheading - 360 :mfd_commandedheading ;
            mfd_commandedangle=(mfd_commandedangle >180) ?  mfd_commandedangle = mfd_commandedangle - 360 :mfd_commandedangle ; 
        #ifdef MSG_PRINT_REC
                    Serial.println("1> "); 
        #endif

            state=E;}
                                                        else if(comparecommand(Command,"\x00A\x1A\x00\x03\xD1\x06",6)){ 
            mfd_commandedangle += 10;
            mfd_commandedheading += 10;
            mfd_commandedheading=(mfd_commandedheading >360) ?  mfd_commandedheading = mfd_commandedheading - 360 :mfd_commandedheading ;
            mfd_commandedangle=(mfd_commandedangle >180) ?  mfd_commandedangle = mfd_commandedangle - 360 :mfd_commandedangle ; 
        #ifdef MSG_PRINT_REC
            Serial.println("10>> "); 
        #endif

            state=E;}
                                                            else if(comparecommand(Command,"\x00A\x1A\x00\x02\xAE\x00",6)){ 
            mfd_commandedangle -= 1;
            mfd_commandedheading -= 1;
            mfd_commandedheading=(mfd_commandedheading <0) ?  mfd_commandedheading = mfd_commandedheading + 360 :mfd_commandedheading ;
            mfd_commandedangle=(mfd_commandedangle < -180) ?  mfd_commandedangle = mfd_commandedangle + 360 :mfd_commandedangle ; 
        #ifdef MSG_PRINT_REC
             Serial.println("<1 "); 
        #endif

            state=E;}
                                                                else if(comparecommand(Command,"\x00A\x1A\x00\x02\xD1\x06",6)){ 
            mfd_commandedangle -= 10;
            mfd_commandedheading -= 10;
            mfd_commandedheading=(mfd_commandedheading <0) ?  mfd_commandedheading = mfd_commandedheading + 360 :mfd_commandedheading ;
            mfd_commandedangle=(mfd_commandedangle < -180) ?  mfd_commandedangle = mfd_commandedangle + 360 :mfd_commandedangle ; 
        #ifdef MSG_PRINT_REC
            Serial.println("<<10 "); 
        #endif

            state=E;}
                                                                else {
                                                                        Serial.print("Unknown: ");for (int i=0;i<8;i++) {Serial.print(Command[i],HEX);Serial.print(" ");};Serial.println();
                                                                    }
    break;
    default:
        break;
    }



    return true;
}