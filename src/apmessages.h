
#ifndef _apMessages_H_
#define _apMessages_H_

#include "N2kMsg.h"
#include "N2kTypes.h"
#include <string.h>
#include <stdint.h>




/************************************************************************//**
 * \brief Setting up PGN127245 Message "Rudder"
 * \ingroup group_msgSetUp
 * 
 * Rudder order command in direction or angle with current rudder angle reading.
 * 
 * \param N2kMsg                Reference to a N2kMsg Object, 
 *                              Output: NMEA2000 message ready to be send.
 * \param RudderPosition        Current rudder postion in radians.
 * \param Instance              Rudder instance.
 * \param RudderDirectionOrder  Direction, where rudder should be turned.
 * \param AngleOrder            In radians angle where rudder should be turned.
 * 
 * \sa tN2kRudderDirectionOrder
 */
void SetN2kPGN130850(tN2kMsg &N2kMsg, const char* Command);

/************************************************************************//**
 * \brief Setting up Message "Rudder" - PGN 127245
 * \ingroup group_msgSetUp
 * 
 * Alias of PGN 127245. This alias was introduced to improve the readability
 * of the source code. See parameter details on \ref SetN2kPGN127245
 * 
 * 
"Auto
130850 Source: 1 Len: 12 Data: 41 9F 2 FF FF A 9 0 FF FF FF FF 
"STBY
130850 Source: 1 Len: 12 Data: 41 9F 2 FF FF A 6 0 FF FF FF FF
"1>
130850 Source: 1 Len: 12 Data: 41 9F 2 FF FF A 1A 0 3 AE 0 FF
"<1
130850 Source: 1 Len: 12 Data: 41 9F 2 FF FF A 1A 0 2 AE 0 FF 
"10>>"
130850 Source: 4 Len: 12 Data: 41 9F 2 FF FF A 1A 0 3 D1 6 FF 
"<<10"
130850 Source: 4 Len: 12 Data: 41 9F 2 FF FF A 1A 0 2 D1 6 FF
 */
inline void SetN2kAp(tN2kMsg &N2kMsg, const char* Command) {
  SetN2kPGN130850(N2kMsg,Command);
}

/************************************************************************//**
 * \brief Parsing the content of Message PGN127245 "Rudder"
 * \ingroup group_msgParsers
 * 
 * Rudder order command in direction or angle with current rudder angle reading.
 * 
 * \param N2kMsg                Reference to a N2kMsg Object, 
 *                              Output: NMEA2000 message ready to be send.
 * \param RudderPosition        Current rudder postion in radians.
 * \param Instance              Rudder instance.
 * \param RudderDirectionOrder  Direction, where rudder should be turned.
 * \param AngleOrder            In radians angle where rudder should be turned.
 * 
 * \return true     Parsing of PGN Message successful
 * \return false    Parsing of PGN Message aborted
 * 
 * \sa tN2kRudderDirectionOrder
 */
bool ParseN2kPGN130850(const tN2kMsg &N2kMsg,char *Command);
bool ParseN2kPGN65341(const tN2kMsg &N2kMsg,unsigned char &mode,double &angle);
bool ParseN2kPGN130821(const tN2kMsg &N2kMsg,unsigned int &MfCode,char * str);
bool ParseN2kPGN65305(const tN2kMsg &N2kMsg,unsigned int &MfCode,unsigned char &Model,unsigned char &Dr,unsigned int &Mode);
bool ParseN2kPGN130824(const tN2kMsg &N2kMsg, unsigned int &MfCode, int keyval[50][2]);
/************************************************************************//**
 * \brief Parsing the content of a "Rudder" 
 *        message - PGN 127245
 * \ingroup group_msgParsers
 * 
 * Alias of PGN 127245. This alias was introduced to improve the readability
 * of the source code. See parameter details on \ref ParseN2kPGN127245 
 */
inline bool ParseN2kAp(const tN2kMsg &N2kMsg,char *Command) {

  return ParseN2kPGN130850(N2kMsg,Command);
}

inline bool ParseN2kApAngle(const tN2kMsg &N2kMsg,unsigned char &mode,double &angle){
  return ParseN2kPGN65341(N2kMsg,mode,angle);
}
inline bool ParseN2kApAscii(const tN2kMsg &N2kMsg,unsigned int &MfCode, char * str){
  return ParseN2kPGN130821(N2kMsg,MfCode,str);
}
#endif

inline bool ParseApStatusMode(const tN2kMsg &N2kMsg,unsigned int &MfCode,unsigned char &Model,unsigned char &Dr,unsigned int &Mode){
  return ParseN2kPGN65305(N2kMsg,MfCode,Model,Dr,Mode);
}
inline bool ParseBGKeyValue(const tN2kMsg &N2kMsg,unsigned int &MfCode,int keyval[50][2]){
  return ParseN2kPGN130824(N2kMsg,MfCode,keyval);
}