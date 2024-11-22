
#ifndef _apMessages_H_
#define _apMessages_H_

#include "N2kMsg.h"
#include "N2kTypes.h"
#include <string.h>
#include <stdint.h>





void SetN2kPGN130850(tN2kMsg &N2kMsg, const char* Command);


inline void SetN2kAp(tN2kMsg &N2kMsg, const char* Command) {
  SetN2kPGN130850(N2kMsg,Command);
}


bool ParseN2kPGN130850(const tN2kMsg &N2kMsg,char *Command);
bool ParseN2kPGN65341(const tN2kMsg &N2kMsg,unsigned char &mode,double &angle);
bool ParseN2kPGN130821(const tN2kMsg &N2kMsg,unsigned int &MfCode,char * str);
bool ParseN2kPGN65305(const tN2kMsg &N2kMsg,unsigned int &MfCode,unsigned char &Model,unsigned char &Dr,unsigned int &Mode);
bool ParseN2kPGN130824(const tN2kMsg &N2kMsg, unsigned int &MfCode, int keyval[50][2]);

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