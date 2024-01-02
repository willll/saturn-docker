◎About this sample
                                           Sega Technical Support

☆Purpose of this sample
  This sample is a line color sample.
  In this sample, the line color for normal scroll is
  You can do the same for rotating scrolls as well. to rotating scroll
  For the line color sample, see SAMPLE4\ROT_BG\LINECOL
  Please refer to.

☆Sample description
  ◇Line color table and palette
    To create a line color screen, VDP2 first uses the line color table to create a line color screen.
    Let's go read the data. This data is an offset from the beginning of the color RAM,
    It is specified in units of colors from the beginning.
    Therefore, in order to multiply the line color, the design of the color RAM must also be
    need to be done.
    In this sample, 256 colors of RGB data are stored in color RAM for better visibility.
    I have defined it, but it can be made simpler.
  ◇Line color and color calculation
    Line color screens always influence others in the form of color calculations.
    In this sample, NBG1 is multiplied by the line color.
    The part of the sample that corresponds to the process below is the setting part.
      slColorCalc( CC_ADD | CC_2ND | NBG1ON );
      slColorCalcOn( NBG1ON );
      slLineColTable( ( void * )LINE_COLOR_TBL );
      slLineColDisp( NBG1ON );
      slColRateLNCL( CLRate20_12 );
  ◇Color calculation mode
    When performing color calculations, there are two types of calculations.
    One is addition mode
    The other is percentage mode.
    When this sample starts, it is running in addition mode.
    In this case, for each pixel value of the top image on the screen that multiplies the color operation,
    Add the pixel values ​​of the screen to be inserted (in this case the line color screen).
    Therefore, the color varies from the original picture to white.
    In ratio mode, each pixel value of the top image on the screen to which the color calculation is applied is
    On the other hand, since the screen to be inserted (in this case the line color screen) is viewed as a percentage value,
    Colors vary from the original painting to black.

☆How to use the sample
  Up/down/left/right keys: Move the mountain picture up/down/left/right.
  L button: Sets color calculation mode to addition mode.
  R button: Sets color calculation mode to ratio mode.
