◎About this sample

☆Purpose
  This sample shows how to use normal shadows for sprites.

☆Description
  Normal shadow uses a certain color code as shadow data.
  Assign.
  Shadows are only possible when the sprite is in palette mode and have been passed to VDP2,
  In the frame buffer, only the lowest 1 bit of the dot color code is 0 and the rest
  Data with a bit of 1 is treated as shadow data.
  Therefore, in this sample, sprite type 5 is specified, so the sprite
  The color bank is set to 0x7ff0 and the bit arrangement is as shown above.
  Also, in order to match the palette code with the bit arrangement, the palette code is 0EH.
  It will be.
  This color data is treated as a transparent color for surfaces other than those on which the shadow is applied.
  In other words, even if you assign a color to this location, that color will not be displayed.
  There is no such thing, so please be careful.
  Normal shadows, unlike MSB shadows, prepare data for shadows.
  is needed.
  Normal shadow sprite data is SGL\SAMPLE4\SPRITE\SHADOW.C
  It is located in

☆How to use the sample
  Arrow keys Move the character up/down/left/right.
