# NMEA 2000 MFD

 Display for my sailboat. Emulates B&G Triton 2

 Based on the work of [Timo](https://github.com/ttlappalainen#:~:text=Timo%20Lappalainen,ttlappalainen)
 whos library i have used for some time now: Thanks Timo\
 Inspired by Hans [Hans](https://github.com/htool#:~:text=Hans,htool), whos ESP Compas i have used for some time, until buying a second hand NAC-2 including an original Precision-9  
 The work on my pilot controller would not have been difficult without [Hans](https://github.com/htool#:~:text=RaymarineAPtoFakeNavicoAutoPilot)  
 I also like to thank the folks on [Canboat](https://canboat.github.io/canboat/canboat.html#lookup-MANUFACTURER_CODE:~:text=%C3%97-,Top,-PGN%20list)  
 Please observe that this project is not completed, and will be <B>Work In Progress</B> for a long time  

Most of the PGN's handled of the display, is implemented in Timo's library execpt for these  

These 5 PGNs are in apmessages, and are subject to rapid changes  
PGN 130850  Autopilot command etc.  
PGN 65341 Autopilot Angle  
PGN 130821 Ascii data  
PGN 65305 Pilot Mode etc.  
PGN 130824 B&G key-value data  

Controller
Based on ESP32-wroom-32, with built in CAN bus controller

Display
As display I have chosen the 5" high brightness [RVT50](https://riverdi.com/product/eve4-intelligent-display-rvt50hqbnwc00-b-5-inch-projected-capacitive-touch-panel-optical-bonding-uxtouch)  

This EVE4 display has Capacitive touch, Optical bonded ( waterproof ?), and a BT817Q GPU  

Bitmaps for buttons moved to flash memory, to save some space in RAM_G

Fixed address memory map for bitmaps in RAM_G changed to use CMD_GETPROPS

Selection of pages displayed in menu. pages button more than 2 secs.
List stored in ESP32 flash
