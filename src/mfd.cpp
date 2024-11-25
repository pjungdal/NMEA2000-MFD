
#include <Arduino.h>
#include <SPI.h>
#include "EVE.h"
#include "tft.h"
#include "touch.h"
//#include <NMEA2000.h>
#define ESP32_CAN_RX_PIN GPIO_NUM_26
#define ESP32_CAN_TX_PIN GPIO_NUM_25
#include <N2kMessages.h>
#include "apmessages.h"
#include <N2kMessagesEnumToStr.h>
#include <NMEA2000_CAN.h>
//#include "state.h"
#include <textparser.h>
#include "Preferences.h"

//#include <sstream>
//#include <string>
//#include "globals.h"
#define DEV_OP10 0

unsigned char Engaged;
extern bool apstate(const char * Command);
double array_calculate_avg(int * buf, int len);

int DEVICE_ID = 65;
const unsigned long TransmitMessagesOP10[] PROGMEM = { 60928L, 59904L, 59392L , 59904L, 130130L};

typedef struct
{
    unsigned long PGN;
    void (*Handler)(const tN2kMsg &N2kMsg);
} tNMEA2000Handler;

void SystemTime(const tN2kMsg &N2kMsg);
void Rudder(const tN2kMsg &N2kMsg);
void Speed(const tN2kMsg &N2kMsg);
void WaterDepth(const tN2kMsg &N2kMsg);
void ApCommand(const tN2kMsg &N2kMsg);
void GNSS(const tN2kMsg &N2kMsg);
void LocalOffset(const tN2kMsg &N2kMsg);
void Attitude(const tN2kMsg &N2kMsg);
void Heading(const tN2kMsg &N2kMsg);
void Humidity(const tN2kMsg &N2kMsg);
void WindSpeed(const tN2kMsg &N2kMsg);
void ApAngle(const tN2kMsg &N2kMsg);
void ApAscii(const tN2kMsg &N2kMsg);
void ApStatusMode(const tN2kMsg &N2kMsg);
void ApHeadTrack(const tN2kMsg &N2kMsg);
void CrossTrackError(const tN2kMsg &N2kMsg);
void NavigationData(const tN2kMsg &N2kMsg);
void BandGKeyValueData(const tN2kMsg &N2kMsg);
void COGSOG(const tN2kMsg &N2kMsg);
void TimeAndDate(const tN2kMsg &N2kMsg);
void PosRapid(const tN2kMsg &N2kMsg);
void CrossTrackError(const tN2kMsg &N2kMsg);
//ParseN2kXTE
tNMEA2000Handler NMEA2000Handlers[] = {
    //{126992L, &SystemTime},
    {127245L, &Rudder},
    {127250L, &Heading},
    {127257L, &Attitude},
    {128259L, &Speed},
    {128267L, &WaterDepth},
    {130306L, &WindSpeed},
    {130850L, &ApCommand},
    {65341L, &ApAngle},
    {130821L, &ApAscii},
    {65305L, &ApStatusMode},
    {127237L, &ApHeadTrack},
    {129283L, &CrossTrackError},
    {129284L, &NavigationData},
    {130824L,&BandGKeyValueData},
    {129026L,&COGSOG},
    {126992L,&TimeAndDate},
    {129025L,&PosRapid},    
    {0, 0}};
#define SERIAL_PORT Serial

Stream *OutputStream;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);

double mfd_heading;
double mfd_commandedheading;
double mfd_depth;
double mfd_speed;
double mfd_rudder;
double mfd_TWA;
double mfd_windangle_rad;
double mfd_AWA;
double mfd_relwindangle_rad;
double mfd_commandedangle;
double mfd_AWS;
double mfd_TWS;
double mfd_TWD;
unsigned char mfd_windref;

unsigned char mfd_DeviceReport;
unsigned int  mfd_APMode;
unsigned int  mfd_PrevAPMode;
unsigned char mfd_APStatus;
double mfd_HeadingToSteer;
double mfd_angle_order;
double mfd_SetRudder;
double WPTDist;
double LLDist;
double LLTime;
double TTW;
double SETA;
double NLB;
double WPTBrg;
double Cog;
double Sog;
double TimeLoc;
double PosLat;
double PosLon;
double mfd_XTE;
static uint32_t update_timer = 0;
Preferences preferences;
void setup()
{
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);
    Serial.begin(115200);
    preferences.begin("pju-mfd",false);
    Selectedpage = preferences.getUInt("Selected",1);
    preferences.end();

#if defined (ESP32)
#if defined (EVE_USE_ESP_IDF) /* not using the Arduino SPI class in order to use DMA */
    EVE_init_spi();
    
#else /* using the Arduino SPI class to be compatible with other devices */
    SPI.begin(EVE_SCK, EVE_MISO, EVE_MOSI);
#endif
/* not using the Arduino SPI class in order to use DMA */
#elif defined (ARDUINO_NUCLEO_F446RE) || defined (ARDUINO_RASPBERRY_PI_PICO)
    EVE_init_spi();
#else
    SPI.begin(); /* sets up the SPI to run in Mode 0 and 1 MHz */
    SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));
#endif

    TFT_init();

#if defined (__AVR__)
    SPI.endTransaction();
    SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));
#elif defined(ESP8266)
    SPI.setFrequency(16UL * 1000000UL);
#elif defined (ARDUINO_HLK_w80x)
    SPI.endTransaction();
    SPI.beginTransaction(SPISettings(20UL * 1000000UL, MSBFIRST, SPI_MODE0));
#else
    SPI.endTransaction();
    SPI.beginTransaction(SPISettings(16UL * 1000000UL, MSBFIRST, SPI_MODE0));
#endif
TFT_buttons();
OutputStream = &Serial;

NMEA2000.SetN2kCANSendFrameBufSize(250);
  NMEA2000.SetN2kCANReceiveFrameBufSize(250);
  
   NMEA2000.SetDeviceCount(1);
   NMEA2000.SetInstallationDescription1("");
   NMEA2000.SetInstallationDescription2("");


  NMEA2000.SetProductInformation("107011234", // Manufacturer's Model serial code
                                 13233, // Manufacturer's product code
                                 "Triton2 Pilot Keypad",  // Manufacturer's Model ID
                                 "2.0.3-0",  // Manufacturer's Software version code
                                 "", // Manufacturer's Model version
                                 1,  // load equivalency *50ma
                                 0xffff, // NMEA 2000 version - use default
                                 0xff, // Sertification level - use default
                                 DEV_OP10
                                );
 
  // Set device information
  

  NMEA2000.SetDeviceInformation(30006, // Unique number. Use e.g. Serial number.
                                140, // Device function=Temperature See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                40, // Device class=Sensor Communication Interface. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                381, // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                                4,
                                DEV_OP10
                               );
     
    // Do not forward bus messages at all
    NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode,DEVICE_ID);
    NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
    NMEA2000.SetForwardStream(OutputStream);
    // Set false below, if you do not want to see messages parsed to HEX withing library
    NMEA2000.EnableForward(false);
    NMEA2000.ExtendTransmitMessages(TransmitMessagesOP10, DEV_OP10);
    NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
    //  NMEA2000.SetN2kCANMsgBufSize(2);
    NMEA2000.Open();

}
//*****************************************************************************
template <typename T>
void PrintLabelValWithConversionCheckUnDef(const char *label, T val, double (*ConvFunc)(double val) = 0, bool AddLf = false, int8_t Desim = -1)
{
    OutputStream->print(label);
    if (!N2kIsNA(val))
    {
        if (Desim < 0)
        {
            if (ConvFunc)
            {
                OutputStream->print(ConvFunc(val));
            }
            else
            {
                OutputStream->print(val);
            }
        }
        else
        {
            if (ConvFunc)
            {
                OutputStream->print(ConvFunc(val), Desim);
            }
            else
            {
                OutputStream->print(val, Desim);
            }
        }
    }
    else
        OutputStream->print("not available");
    if (AddLf)
        OutputStream->println();
}

//*****************************************************************************


void PosRapid(const tN2kMsg &N2kMsg){
    double Latitude;
    double Longitude;
        ParseN2kPositionRapid(N2kMsg, Latitude,Longitude ) ;
        PosLat=Latitude;
        PosLon=Longitude;
}
void BandGKeyValueData(const tN2kMsg &N2kMsg){
    unsigned int MfCode;
    int keyval[50][2];
char timestr[50] = "";
int hours;
int mins;
int secs;
int deg;
    ParseBGKeyValue( N2kMsg,MfCode,keyval);
    for (int i=0;i<N2kMsg.DataLen/6;i++){
        switch (keyval[i][0])
        {
        case 256:
           LLTime = keyval[i][1];
//Serial.print("256: ");Serial.println(LLTime);
            break;
        case 257:
            LLTime = keyval[i][1];
//Serial.print("257: ");Serial.println(LLTime);
            break;        
        case 258:
            LLDist = keyval[i][1];
//Serial.print("258: ");Serial.println(LLTime);
            break;
        case 259:
            LLDist = keyval[i][1];
//Serial.print("259: ");Serial.println(LLDist);
            break;
        case 260:
            TTW = keyval[i][1];

        break;
        case 261:
//            SETA = keyval[i][1];
//Serial.print("261: ");Serial.println((keyval[i][1])/1.852);
//Serial.print("???: "); Serial.print(keyval[i][0]); Serial.print(" : ");Serial.println(keyval[i][1]);
            break;
        case 262:
            SETA = keyval[i][1];
//Serial.print("262: ");Serial.println(SETA);
            break;
        case 309:
            NLB = keyval[i][1];
//Serial.print("309: ");Serial.println(NLB);
            break;
        default:
Serial.print("???: "); Serial.print(keyval[i][0]); Serial.print(" : ");Serial.println(keyval[i][1]);
            break;
        }
    }
}
void ApHeadTrack(const tN2kMsg &N2kMsg){


  tN2kOnOff RudderLimitExceeded[2] = {N2kOnOff_On,N2kOnOff_On};
  tN2kOnOff OffHeadingLimitExceeded[2] = {N2kOnOff_Off,N2kOnOff_Off};
  tN2kOnOff OffTrackLimitExceeded[2] = {N2kOnOff_On,N2kOnOff_On};
  tN2kOnOff Override[2] = {N2kOnOff_Off,N2kOnOff_Off};
  tN2kSteeringMode SteeringMode[2] = {N2kSM_FollowUpDevice,N2kSM_FollowUpDevice};
  tN2kTurnMode TurnMode[2] = {N2kTM_RadiusControlled,N2kTM_RadiusControlled};
  tN2kHeadingReference HeadingReference[2] = {N2khr_Unavailable,N2khr_Unavailable};
  tN2kRudderDirectionOrder CommandedRudderDirection[2] = {N2kRDO_Unavailable,N2kRDO_Unavailable};
  double CommandedRudderAngle[2] = {0.1,0.0};
  double HeadingToSteerCourse[2] = {0.2,0.0};
  double Track[2] = {0.3,0.0};
  double RudderLimit[2] = {0.4,0.0};
  double OffHeadingLimit[2] = {0.5,0.0};
  double RadiusOfTurnOrder[2] = {10,0.0};
  double RateOfTurnOrder[2] = {0.7,0.0};
  double OffTrackLimit[2] = {4,0.0};
  double VesselHeading[2] = {0.9,0.0};

 
  ParseN2kHeadingTrackControl(N2kMsg,
    RudderLimitExceeded[1],
    OffHeadingLimitExceeded[1],
    OffTrackLimitExceeded[1],
    Override[1],
    SteeringMode[1],
    TurnMode[1],
    HeadingReference[1],
    CommandedRudderDirection[1],
    CommandedRudderAngle[1],
    HeadingToSteerCourse[1],
    Track[1],
    RudderLimit[1],
    OffHeadingLimit[1],
    RadiusOfTurnOrder[1],
    RateOfTurnOrder[1],
    OffTrackLimit[1],
    VesselHeading[1]);

    mfd_HeadingToSteer=RadToDeg(HeadingToSteerCourse[1]);
/*Serial.println("Heading/Track control");
Serial.print("Heading to steer: ");Serial.print(RadToDeg( HeadingToSteerCourse[1]));
Serial.print(" Off heading: ");Serial.print(RadToDeg(OffHeadingLimit[1]));
Serial.print(" Off track: ");Serial.print(OffTrackLimit[1]);
Serial.print(" Vessel heading: ");Serial.println(RadToDeg(VesselHeading[1]));*/
}


   void CrossTrackError(const tN2kMsg &N2kMsg){
    unsigned char SID;
    tN2kXTEMode XTEMode; 
    bool NavigationTerminated; 
    double XTE;
        ParseN2kXTE(N2kMsg,SID,XTEMode,NavigationTerminated,XTE);
        //Serial.println(XTE);
        mfd_XTE=XTE;
   }





void NavigationData(const tN2kMsg &N2kMsg){
    unsigned char SID; 
    double DistanceToWaypoint; 
    tN2kHeadingReference BearingReference;
    bool PerpendicularCrossed; 
    bool ArrivalCircleEntered; 
    tN2kDistanceCalculationType CalculationType;
    double ETATime; 
    int16_t ETADate; 
    double BearingOriginToDestinationWaypoint; 
    double BearingPositionToDestinationWaypoint;
    uint32_t OriginWaypointNumber; 
    uint32_t DestinationWaypointNumber;
    double DestinationLatitude; 
    double DestinationLongitude; 
    double WaypointClosingVelocity;
    ParseN2kNavigationInfo(N2kMsg,SID, DistanceToWaypoint, BearingReference, PerpendicularCrossed, ArrivalCircleEntered, CalculationType,
                            ETATime, ETADate, BearingOriginToDestinationWaypoint, BearingPositionToDestinationWaypoint,
                            OriginWaypointNumber, DestinationWaypointNumber, DestinationLatitude, DestinationLongitude, WaypointClosingVelocity);
//Serial.print("WPT Dist: ");Serial.println(DistanceToWaypoint);
WPTDist=DistanceToWaypoint/1.852;
WPTBrg = RadToDeg(BearingPositionToDestinationWaypoint);
}
void COGSOG(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double COG;
    double SOG;
    
    if (ParseN2kCOGSOGRapid(N2kMsg,SID,HeadingReference,COG,SOG) ) {
    /*OutputStream->println("COG/SOG:");
    PrintLabelValWithConversionCheckUnDef("  SID: ",SID,0,true);
    OutputStream->print("  reference: "); PrintN2kEnumType(HeadingReference,OutputStream);
    PrintLabelValWithConversionCheckUnDef("  COG (deg): ",COG,&RadToDeg,true);
    PrintLabelValWithConversionCheckUnDef("  SOG (m/s): ",SOG,0,true);*/
      Cog=RadToDeg(COG);
      Sog=SOG*19.43385;
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}
void TimeAndDate(const tN2kMsg &N2kMsg){
unsigned char SID;
uint16_t SystemDate;
double SystemTime;
tN2kTimeSource TimeSource;
    //Serial.println("TimeHandler");
    if(ParseN2kSystemTime(N2kMsg,SID,SystemDate,SystemTime,TimeSource)){
    TimeLoc=SystemTime;
    /*Serial.println();
    Serial.print("SID: ");Serial.print(SID);Serial.print(" Time source: ");Serial.print(TimeSource);
    Serial.print(" Time: ");Serial.println(TimeLoc);*/}
    else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
}
}
void ApStatusMode(const tN2kMsg &N2kMsg){
    unsigned int MfCode;
    unsigned char Model;
    unsigned char Dr;
    unsigned int Mode;
    ParseApStatusMode(N2kMsg,MfCode,Model,Dr,Mode);
    mfd_DeviceReport = Dr;
    if(Dr == 10){ if(mfd_APStatus==16) mfd_APMode=Mode&0xffff;};
    if(Dr == 2){mfd_APStatus=Mode&0xff;if(mfd_APStatus==16)mfd_PrevAPMode=mfd_APMode;};
    if(mfd_APStatus==6||mfd_APStatus==12) {mfd_APMode=0x20;if(mfd_PrevAPMode==0) mfd_PrevAPMode=0x10;}

    //mfd_APMode = Mode;


//    if(Dr==2) {Serial.print(" Status: ");Serial.println(mfd_APStatus,BIN);}
//    if(Dr==10) {Serial.print(" Mode: ");Serial.println(mfd_APMode,BIN);}
//Serial.print("Device report: ");Serial.print(Dr);Serial.print(" Mode: ");Serial.print(mfd_APMode);Serial.print(" Prev Mode: ");Serial.print(mfd_PrevAPMode);Serial.print(" Status: ");Serial.println(mfd_APStatus);

}
void ApAngle(const tN2kMsg &N2kMsg){
    unsigned char mode;
    double Angle;
    ParseN2kApAngle(N2kMsg,mode,Angle);
    Angle=RadToDeg(Angle);
switch (mode)
{

case 2:
    //if (Angle>180) Angle=-1*(360-Angle);
    break;
case 3:
   if (Angle>180) Angle=-1*(360-Angle);
   mfd_commandedangle = Angle;
    break;
case 10:
   //if (Angle>180) Angle=-1*(360-Angle);
    break;
case 11:
   //if (Angle>180) Angle=-1*(360-Angle);
    break;
default:
    break;
}

    #ifdef MSG_PRINT_A
    Serial.print("Mode: ");Serial.print(mode);
    Serial.print(" Angle: ");Serial.println(Angle);
    #endif
}
void ApAscii(const tN2kMsg &N2kMsg){
    unsigned int MfCode;
    char str[400];
    short Arr[200];
    TextParser parser (",");

    ParseN2kApAscii(N2kMsg,MfCode,str);
    parser.parseLine(str,Arr);
    mfd_commandedheading=Arr[7];
    mfd_SetRudder=Arr[13];
    #ifdef MSG_PRINT_SRC3
    Serial.print("MfCode: ");Serial.print(MfCode);
    Serial.print(" Datastring: ");Serial.println(str);
    Serial.println(Arr[7]);
    #endif
}
void ApCommand(const tN2kMsg &N2kMsg)
{
 /*   char Command[8];
    if (ParseN2kAp(N2kMsg,Command))
    {
        apstate(Command);
    }
    else
    {
#ifdef MSG_PRINT_REC
        OutputStream->print("Failed to parse PGN: ");
        OutputStream->println(N2kMsg.PGN);
#endif
    }*/
#ifdef MSG_PRINT_SRC2
   if(N2kMsg.Data[2]==2){OutputStream->print("Event Command: ");OutputStream->print(" Controlling device: ");OutputStream->print(N2kMsg.Data[5]);OutputStream->print(" Event: ");OutputStream->print(N2kMsg.Data[6]);
   OutputStream->println();}
   else
   {OutputStream->print("Command: ");OutputStream->println(N2kMsg.Data[2]);}
   //if(N2kMsg.Data[2]==0xff){OutputStream->print("Event Command: ");OutputStream->print(" Controlling device: ");OutputStream->print(N2kMsg.Data[5]);OutputStream->print(" Event: ");OutputStream->print(N2kMsg.Data[6]);
   //OutputStream->println();}
#endif
}


void WindSpeed(const tN2kMsg &N2kMsg){
    unsigned char SID;
    double WindSpeed;
    double WindAngle;
    tN2kWindReference WindRef;
    double BSP = mfd_speed*1852/3600;

if(ParseN2kWindSpeed(N2kMsg,SID,WindSpeed,WindAngle,WindRef)){
    mfd_AWS = WindSpeed;
    mfd_AWA = RadToDeg(WindAngle);
    mfd_TWS=sqrt(pow(BSP,2)+pow(mfd_AWS,2)-(2*BSP*mfd_AWS*cos(WindAngle)));

    if(BSP > 0 ) mfd_TWA= 180 - RadToDeg(acos((pow(mfd_TWS,2)+pow(BSP,2)-pow(mfd_AWS,2))/(2*mfd_TWS*BSP)));
    else mfd_TWA=  mfd_AWA;
    mfd_relwindangle_rad = WindAngle;


    if (mfd_AWA > 180? mfd_AWA = mfd_AWA-360:mfd_AWA = mfd_AWA);
    mfd_TWD=((int)mfd_heading+(int)mfd_TWA) % 360;

#ifdef MSG_PRINT_WIND
Serial.print("AWS: ");Serial.print(mfd_AWS); Serial.print(" TWS: ");Serial.print(mfd_TWS);Serial.print(" BSP: ");Serial.print(BSP);
Serial.print(" AWA: ");Serial.print(mfd_AWA);Serial.print(" TWA: ");Serial.print(mfd_TWA);Serial.print(" Windangle: ");Serial.print(WindAngle);
Serial.print(" WindReference: ");Serial.println(mfd_windref);
#endif


    mfd_windref = WindRef;
}
}
void SystemTime(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    uint16_t SystemDate;
    double SystemTime;
    tN2kTimeSource TimeSource;

    if (ParseN2kSystemTime(N2kMsg, SID, SystemDate, SystemTime, TimeSource))
    {
       #ifdef MSG_PRINT 
       OutputStream->println("System time:");
       
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        PrintLabelValWithConversionCheckUnDef("  days since 1.1.1970: ", SystemDate, 0, true);
        PrintLabelValWithConversionCheckUnDef("  seconds since midnight: ", SystemTime, 0, true);
        
        OutputStream->print("  time source: ");
        PrintN2kEnumType(TimeSource, OutputStream);
        #endif
    }
    else
    {
#ifdef MSG_PRINT
        OutputStream->print("Failed to parse PGN: ");
        OutputStream->println(N2kMsg.PGN);
#endif
    }
}
void Rudder(const tN2kMsg &N2kMsg)
{
    unsigned char Instance;
    tN2kRudderDirectionOrder RudderDirectionOrder;
    double RudderPosition;
    double AngleOrder;

    if (ParseN2kRudder(N2kMsg, RudderPosition, Instance, RudderDirectionOrder, AngleOrder))
    {
       if(N2kMsg.Source==26) {mfd_rudder = RadToDeg(RudderPosition);mfd_angle_order = RadToDeg(AngleOrder);}
#ifdef MSG_PRINT_RUDDER

        PrintLabelValWithConversionCheckUnDef("Rudder: ", Instance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  position (deg): ", RudderPosition, &RadToDeg, true);
        //OutputStream->print("  direction order: ");
        //PrintN2kEnumType(RudderDirectionOrder, OutputStream);
        //PrintLabelValWithConversionCheckUnDef("  angle order (deg): ", AngleOrder, &RadToDeg, true);
#endif
    }
    else
    {
#ifdef MSG_PRINT

        OutputStream->print("Failed to parse PGN: ");
        OutputStream->println(N2kMsg.PGN);
#endif
    }
 
}
void Heading(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    tN2kHeadingReference HeadingReference;
    double Heading;
    double Deviation;
    double Variation;

    if (ParseN2kHeading(N2kMsg, SID, Heading, Deviation, Variation, HeadingReference))
    {
        mfd_heading = RadToDeg(Heading);
#ifdef MSG_PRINT 
        OutputStream->println("Heading:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        OutputStream->print("  reference: ");
        PrintN2kEnumType(HeadingReference, OutputStream);
        PrintLabelValWithConversionCheckUnDef("  Heading (deg): ", Heading, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Deviation (deg): ", Deviation, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Variation (deg): ", Variation, &RadToDeg, true);
#endif
    }
    else
    {
#ifdef MSG_PRINT
        OutputStream->print("Failed to parse PGN: ");
        OutputStream->println(N2kMsg.PGN);
#endif
    }
    
}
void Speed(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    double SOW;
    double SOG;
    tN2kSpeedWaterReferenceType SWRT;

    if (ParseN2kBoatSpeed(N2kMsg, SID, SOW, SOG, SWRT))
    {
#ifdef MSG_PRINT
        OutputStream->print("Boat speed:");
        PrintLabelValWithConversionCheckUnDef(" SOW:", N2kIsNA(SOW) ? SOW : msToKnots(SOW));
        PrintLabelValWithConversionCheckUnDef(", SOG:", N2kIsNA(SOG) ? SOG : msToKnots(SOG));
        OutputStream->print(", ");
        PrintN2kEnumType(SWRT, OutputStream, true);
#endif
        mfd_speed = msToKnots(SOW);
    }
}
void Attitude(const tN2kMsg &N2kMsg)
{
    unsigned char SID;
    double Yaw;
    double Pitch;
    double Roll;

    if (ParseN2kAttitude(N2kMsg, SID, Yaw, Pitch, Roll))
    {
#ifdef MSG_PRINT
        OutputStream->println("Attitude:");
        PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
        PrintLabelValWithConversionCheckUnDef("  Yaw (deg): ", Yaw, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Pitch (deg): ", Pitch, &RadToDeg, true);
        PrintLabelValWithConversionCheckUnDef("  Roll (deg): ", Roll, &RadToDeg, true);
#endif
    }
    else
    {
#ifdef MSG_PRINT
        OutputStream->print("Failed to parse PGN: ");
        OutputStream->println(N2kMsg.PGN);

#endif 
    }
    }
void WaterDepth(const tN2kMsg &N2kMsg)
    {
        unsigned char SID;
        double DepthBelowTransducer;
        double Offset;

        if (ParseN2kWaterDepth(N2kMsg, SID, DepthBelowTransducer, Offset))
        mfd_depth=DepthBelowTransducer; 
        {

    }
}
// NMEA 2000 message handler
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg)
{
    int iHandler;

    // Find handler
    // First a lot of debugging. Can be removed when complete
#ifdef MSG_PRINT_PGN
 //OutputStream->print("In Main Handler: ");
 //if((N2kMsg.Source==2  ))
 //  && (N2kMsg.Source==1 || N2kMsg.Source==4) )
 //||N2kMsg.PGN == 130822
   //||N2kMsg.PGN == 130845||N2kMsg.PGN == 130860||N2kMsg.PGN == 130851
   //if(N2kMsg.Source == 25 || N2kMsg.Source == 50)
   if(N2kMsg.PGN == 130821)
   { 
  
    //OutputStream->print(N2kMsg.PGN);

   OutputStream->print(" Source: ");
   OutputStream->print(N2kMsg.Source);
   /*OutputStream->print(" Pri: ");
   OutputStream->print(N2kMsg.Priority);*/

   OutputStream->print(" Len: ");

   OutputStream->print(N2kMsg.DataLen);
   OutputStream->print(" Data: ");
   /*for (int i = 0;i<N2kMsg.DataLen;i++) {OutputStream->print(N2kMsg.Data[i],16);OutputStream->print(" ");}
   OutputStream->println(""); OutputStream->println("");*/

   /*char str[10]="";
   for (int i = 0;i<N2kMsg.DataLen;i++) {if(N2kMsg.Data[i] >= 0x20&& N2kMsg.Data[i] <=127) {sprintf(str,"%c",N2kMsg.Data[i]);OutputStream->print(str);}
   } 
   OutputStream->println();*/  
   }
#endif    
#ifdef MSG_PRINT_SRC2
 //OutputStream->print("In Main Handler: ");
   if(((N2kMsg.Source==9 /*|| N2kMsg.PGN == 130850/*|| N2kMsg.Source==4*/) /*&& (N2kMsg.PGN == 130850 || N2kMsg.PGN == 130845||N2kMsg.PGN == 130860||N2kMsg.PGN == 130851*/ /*|| N2kMsg.PGN == 130821*/ /*|| N2kMsg.PGN == 130822 || N2kMsg.PGN == 59904 || N2kMsg.PGN == 65341 || N2kMsg.PGN == 65305 )*/))
   //  && (N2kMsg.Source==1 || N2kMsg.Source==4) )
   //||N2kMsg.PGN == 130822
   //||N2kMsg.PGN == 130845||N2kMsg.PGN == 130860||N2kMsg.PGN == 130851
   { 
    //OutputStream->println("Modtager");
    //OutputStream->print(N2kMsg.PGN);

   OutputStream->print(" Source: ");
   OutputStream->print(N2kMsg.Source);
   OutputStream->print(" PGN: ");
   //OutputStream->print(N2kMsg.Priority);
OutputStream->print(N2kMsg.PGN); 
 switch (N2kMsg.PGN)
 {
 case 65305:
    OutputStream->print(" (Status)");
    break;
 case 65341:
    OutputStream->print(" (AP Angle)");
    break;
 case 59904:
    OutputStream->print(" (ISO Req)");
    break;
 case 130850:
    OutputStream->print(" (AP Command)");
    break;
 case 130851:
    OutputStream->print(" (AP Reply)");
    break;
 case 130821:
    OutputStream->print(" (AP Ascii)");

    break;
   case 127237:
    OutputStream->print(" (Head/Track )");

    break;
  case 130860:
    OutputStream->print(" (AP Unknown)");

    break;
 case 127245:
    OutputStream->print(" (AP Rudder)");

    break;

 default:
    break;
 }  
 if(N2kMsg.PGN==130821){   
    char str[500] ; 
    int i;
    //OutputStream->print(" (AP Ascii)");
    OutputStream->print(" Len: ");OutputStream->print(N2kMsg.DataLen);OutputStream->print(" Data: ");
    for ( i=5;i<=N2kMsg.DataLen;i++) str[i-5]=(N2kMsg.Data[i]);
    str[i-4]=0;
    OutputStream->println(str);}
 else
   
    {OutputStream->print(" Len: ");
   OutputStream->print(N2kMsg.DataLen);
   OutputStream->print(" Data: ");
   for (int i = 0;i<N2kMsg.DataLen;i++) {OutputStream->print(N2kMsg.Data[i],16);OutputStream->print(" ");}
    
   OutputStream->println("");
   }
   }
#endif    

#ifdef MSG_PRINT_WIND
 //OutputStream->print("In Main Handler: ");
   if((N2kMsg.PGN == 130306  ||N2kMsg.PGN == 130323||N2kMsg.PGN == 130324))
   //  && (N2kMsg.Source==1 || N2kMsg.Source==4) )
   //||N2kMsg.PGN == 130822
   
   { 

    OutputStream->print(N2kMsg.PGN);

   OutputStream->print(" Source: ");
   OutputStream->print(N2kMsg.Source);
   OutputStream->print(" Pri: ");
   OutputStream->print(N2kMsg.Priority);

   OutputStream->print(" Len: ");

   OutputStream->print(N2kMsg.DataLen);
   OutputStream->print(" Data: ");
   for (int i = 0;i<N2kMsg.DataLen;i++) {OutputStream->print(N2kMsg.Data[i],16);OutputStream->print(" ");
   }
   OutputStream->println("");
   }
#endif 
    for (iHandler = 0; NMEA2000Handlers[iHandler].PGN != 0 && !(N2kMsg.PGN == NMEA2000Handlers[iHandler].PGN); iHandler++);
    if (NMEA2000Handlers[iHandler].PGN != 0)
    {    
        NMEA2000Handlers[iHandler].Handler(N2kMsg);
    }
}

void updatesettings(){
uint8_t temp;
preferences.begin("pju-mfd",false);
temp = preferences.getUInt("Selected",1);
if (temp!= Selectedpage) preferences.putUInt("Selected",Selectedpage);
update_timer=0;
preferences.end();
}

void loop()
{

    uint32_t current_millis;
    static uint32_t previous_millis = 0;


    static uint8_t display_delay = 0;
    uint32_t micros_start, micros_end;
    current_millis = millis();
    NMEA2000.ParseMessages();
    //OutputStream->print("Parser: ");
    if ((current_millis - previous_millis) > 10) /* execute the code every 10 milli-seconds */
    {
        previous_millis = current_millis;
        update_timer+=current_millis;
        if(update_timer>=5000) updatesettings();
        micros_start = micros();
        TFT_touch();
        micros_end = micros();

        display_delay++;
        if (display_delay > 3) /* refresh the display every 20ms */
        {
            display_delay = 0;
            micros_start = micros();
            TFT_display();
            micros_end = micros();
        }
    }
}
