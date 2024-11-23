The bitmaps for EVE 4 needs to be converted by Bridgetek EAB
Seems not always necessary, but do it anyway.

The size of the bitmap to be loaded in the display can be found when it is loaded in Bridgetek EVE Screen Editor

Bitmap is then converted to hex  
cat Sailsteer\ outer\ 670x480_670x480_ARGB1555_Converted.png |hexdump  -ve '16/1 "0x%02x, ""\n"' > sailsteer-outer-640x480.hex
