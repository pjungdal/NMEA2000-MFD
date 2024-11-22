

#include "apmessages.h"

//*****************************************************************************

void SetN2kPGN130850(tN2kMsg &N2kMsg, const char* Command) {
    N2kMsg.SetPGN(130850L);
    N2kMsg.Priority=2;
    N2kMsg.AddByte(0x41);N2kMsg.AddByte(0x9F);N2kMsg.AddByte(0x02);N2kMsg.AddByte(0x0FF);N2kMsg.AddByte(0x0FF);//N2kMsg.AddByte(0x0A);
    for (int i =0;i<6;i++)
    N2kMsg.AddByte(Command[i]);
    N2kMsg.AddByte(0xff);
  #ifdef MSG_PRINT_SEND
  Serial.println("Sender");
  Serial.print(N2kMsg.PGN);Serial.print(" ");Serial.print("Source: ");Serial.print(N2kMsg.Source);Serial.print(" Pri: ");
  Serial.print(N2kMsg.Priority);Serial.print(" Len: ");Serial.print(N2kMsg.DataLen);Serial.print(" Data: ");
  for (int i=0;i<N2kMsg.DataLen;i++) {Serial.print(N2kMsg.Data[i],16);Serial.print(" ");}
  Serial.println();
  #endif
}

bool ParseN2kPGN130850(const tN2kMsg &N2kMsg, char *Command) {
  if (N2kMsg.PGN!=130850L) return false;
    if(N2kMsg.DataLen==12){
      for (int i=0;i<6;i++) Command[i]=N2kMsg.Data[i+5];
    }
  
  return true;
}
bool ParseN2kPGN65341(const tN2kMsg &N2kMsg, unsigned char &mode, double &angle) {
  int Index=4;
  if (N2kMsg.PGN!=65341L) return false;
    if(N2kMsg.DataLen==8){
      mode = N2kMsg.GetByte(Index);Index++;
      angle = N2kMsg.Get2ByteUDouble(0.0001,Index);
    }
  
  return true;
}
bool ParseN2kPGN65305(const tN2kMsg &N2kMsg,unsigned int &MfCode,unsigned char &Model,unsigned char &Dr,unsigned int &Mode) {
  int Index=0;
  if (N2kMsg.PGN!=65305L) return false;
    if(N2kMsg.DataLen==8){
      MfCode = N2kMsg.Get2ByteUInt(Index)& 0x7FF;
      Model = N2kMsg.GetByte(Index);
      Dr = N2kMsg.GetByte(Index);
      Mode =N2kMsg.Get2ByteUInt(Index);
    }
  
  return true;
}
bool ParseN2kPGN130821(const tN2kMsg &N2kMsg, unsigned int &MfCode, char * str) {
  int Index=0;
  if (N2kMsg.PGN!=130821L) return false;
  MfCode= N2kMsg.Get2ByteUInt(Index)& 0x7FF;
  Index++;Index++;Index++;
N2kMsg.GetStr( str,N2kMsg.DataLen-5, Index);
 //Serial.print("Datalen: ")  ; Serial.print(N2kMsg.DataLen)  ; Serial.print(" Index: ")  ; Serial.println(Index)  ;  Serial.print(" Mfcode: ")  ; Serial.println(MfCode)  ;
 #ifdef MSG_DEBUG_130821
 Serial.println(str);
 #endif
  return true;
}
bool ParseN2kPGN130824(const tN2kMsg &N2kMsg, unsigned int &MfCode, int keyval[50][2]) {
  int Index=0;
  uint16_t keylen;
  uint16_t key;
  double value;
  unsigned int fieldlen;
  if (N2kMsg.PGN!=130824L) return false;
  MfCode= N2kMsg.Get2ByteUInt(Index)& 0x7FF;
 //Serial.print("Datalen: ")  ; Serial.print(N2kMsg.DataLen)  ; Serial.print(" Index: ")  ; Serial.print(Index)  ;  Serial.print(" Mfcode: ")  ; Serial.print(MfCode)  ;
//  Index++;Index++;Index++;
int i = 0;
while (Index<N2kMsg.DataLen){
//Serial.print(" Index før: ")  ; 
//Serial.print(Index)  ; 
keylen=N2kMsg.Get2ByteUInt(Index);
key=keylen & 0xfff; 
fieldlen=(keylen&0xf000) >> 12;
if(fieldlen==4) value = N2kMsg.Get4ByteDouble(0.001,Index) ;
else
if(fieldlen==2) value = N2kMsg.Get2ByteDouble(0.001,Index) ;
keyval[i][0] = key;
keyval[i][1] = value;
i++;

/*Serial.print(" keylen: ")  ; 
Serial.print(keylen,HEX)  ;
Serial.print(" Fieldlen: ")  ; 
Serial.print(fieldlen) ;
Serial.print(" Key: ")  ; 
Serial.print(key)  ;
Serial.print(" Value: ")  ; 
Serial.println(value)  ;*/
}
/*Serial.print(" Index: ")  ; 
Serial.print(Index)  ; 
Serial.print(" Index: ")  ; 
Serial.print(Index)  ; 
Serial.print(" keylen: ")  ; 
Serial.print(keylen)  ;
Serial.print(" Len: ")  ; 
Serial.print(N2kMsg.DataLen)  ; 
Serial.print(" Index: ")  ; 
Serial.print(Index)  ; 
Serial.print(" MfCode: ")  ; 
Serial.print(MfCode)  ;
Serial.print(" Key: ")  ; 
Serial.print(key)  ;
Serial.print(" Fieldlen: ")  ; 
Serial.print(fieldlen)  ;
Serial.print(" Value: ")  ; 
Serial.println(value)  ;*/
  return true;
}