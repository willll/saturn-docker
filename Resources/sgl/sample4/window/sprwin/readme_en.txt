◎About this sample

☆Purpose
  A simple demo using a sprite window

☆Description
  A sprite window uses the shape of a sprite as a window.
  From VDP1, set the MSB on bit and execute the sprite drawing command.
  VDP2 looks at the MSB and processes the sprite window.
  Sprite windows use MSB as well as MSB shadows, so
  Mixing is not possible. (Sprite window has priority.)
  Also, like MSB shadows, the sprite color mode is only palette mode,
  Does not work in mixed mode with RBG.
  The window only looks at the presence or absence of the MSB, so it doesn't matter what the dot color data is.
  There is no problem. (All are treated as windows.)
  ■VDP1 side settings
    In VDP1, it is necessary to specify MSB On for the data on which sprite shadows are applied.
    there is. If you extract that part from the sample,
      SPR_ATTR tails_atr = SPR_ATTRIBUTE( 0, 0 | ( 5 << 12 ) | ( 1 << 11 ),
				   No_Gouraud, CL256Bnk | MSBon,
				   sprNoflip | _ZmCC );
    The fourth argument of the SPR_ATTRIBUTE macro is the command table's
    For the drawing mode word, connect the color mode specification and MSB ON specification with '|'.
    Specify.
  ■VDP2 side settings
    In VDP2, sprite window enable setting and window effective area
    Set the sprite color mode.
    Extracted from the sample source below,
      slSpriteColMode( SPR_PAL );
      slSpriteWinMode( ON );
      slScrWindowModeNbg1( spw_OUT );
      slScrWindowModeNbg2( spw_IN );
    The first line sets the sprite's color mode to palette-only mode,
    The next line turns on the sprite window enable bit.
    The remaining lines specify the effective area of ​​the window.

☆How to use
  Directional keys: Move the character up/down/left/right.
  L button: NBG1 is valid outside the sprite window.
            NBG2 enables the inside of the sprite window.
  R button: NBG2 is valid outside the sprite window.
            NBG1 enables the inside of the sprite window.

  NBG1: Mountain painting
  NBG2: Flower garden painting
