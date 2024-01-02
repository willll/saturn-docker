◎About this sample

  This sample displays 16-color palette mode, blurs, and color operations.
This is a sample. Blur creates a beautiful effect when applied to a realistic picture with a small number of colors.
It seems to be visible.

☆Points to be careful of
  Blur is only effective in color mode 0, that is,
  slColRAMMode( CRM16_1024 );
  I have to do it.
  Also, if the blur operation enable bit is set to OFF, the color operation will be blurred.
  The calculation will be performed with the back screen, not with the specified screen.
  About 16 color palette mode
  VDP2's 16-color mode has a slightly special arrangement of pattern name data, so
  As a sample, I have given an example where it is placed at the end of the color RAM.

☆How to operate
  L Blur enable bit ON
  R Blur enable bit OFF
  Right Decrease calculation ratio
  Left Increase calculation rate
