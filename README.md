# NMEA 2000 MFD
 Display for my sailboat. Emulates B&G Triton 2

 Based on the work of [Timo](https://github.com/ttlappalainen#:~:text=Timo%20Lappalainen,ttlappalainen)
 whos library i have used for some time now: Thanks Timo
 Inspired by Hans [Hans](https://github.com/htool#:~:text=Hans,htool), whos ESP Compas i have used for some time, until buying a second hand NAC-2 including an original Precision-9
 The work on my pilot controller would not have been difficult without [text](https://github.com/htool#:~:text=RaymarineAPtoFakeNavicoAutoPilot)
 I also like to thank the folks on [Canboat](https://canboat.github.io/canboat/canboat.html#lookup-MANUFACTURER_CODE:~:text=%C3%97-,Top,-PGN%20list)

 Most of the PGN's handled of the display, is implemented in Timo's library execpt for these

PGN 130850  Autopilot command etc.
PGN 65341 Autopilot Angle
PGN 130821 Ascii data
PGN 65305 Pilot Mode etc.
PGN 130824 B&G key-value data