RBG0/coefficient table sample

                                                   1997/7/14
                                             Software Technology Development Department

●Summary
The RBG0 coefficient table is specified for each dot and a lens effect is applied to the screen.
Move the screen by pressing the pad up/down/left/right, and rotate by pressing the R/L button.
●File contents
map directory: Map data for display. The pattern name table is 2 words,
                   Characters are in 256 colors.
main.c: Main file
map.c: for map data include
Makefile: Makefile
objects: Includes for makefiles

●Explanation
There are four modes for using the coefficient table.
  Mode 0: Use as scaling factor kx, ky
  Mode 1: Use as scaling factor kx
  Mode 2: Use as scaling factor ky
  Mode 3: Use as viewpoint coordinates Xp after rotation transformation
That's what it means. In this sample, we use mode 0 to place part of the screen on the lens.
We are expanding it.
Transfer of pattern name data and character data to VRAM, screen initialization procedure
This is done in the same way as other samples.
The coefficient table is initialized at line 75 in the source file.
  MakeKtable ((void*)RBG0_KTABLE_ADR,(void*)RBG0_PAR_ADR);
  slKtableRA ((void*)RBG0_KTABLE_ADR, K_ON|K_1WORD|K_MODE0|K_DOT|K_FIX);
This is done in
Also, the actual coefficient table data is created within the function MakeKtable.
In this sample, the lens is a simple circle, so by calculation within this function,
I have created a table. To use a more complex coefficient table, put
You should probably keep the coefficient data.
When preparing a coefficient table for each dot, 1 word with a resolution of 320*224
Even if you use coefficient table mode, you will end up with 140 kilobytes of data.
You will need.
In this case, the coefficient table cannot be placed in one bank in VRAM, so in this sample,
The coefficient table is expanded twice horizontally and used.
That process is on line 120,
  *(Uint16*)(RBG0_PAR_ADR+0x58)=160; //320;
This will be the part. This process must be performed every Sync.
By expanding the coefficient table and using it in this way, it is possible to
It is possible to have a large size coefficient table. If within a certain range,
It would also be possible to scroll the coefficient table itself.
