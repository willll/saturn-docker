◎About this sample

  This sample is a sample that performs extended color calculation of VDP2.

explanation
  Extended color operations allow you to use 2-sided or 3-sided translucent scrolling.
  For more information on Enhanced Color Screen,
    Hardware Manual Vol.2 VDP2 User's Manual
    Extended color operations section in Chapter 12 Color Operations
  Please refer to.
  Depending on the color calculation ratio, scrolling may be performed as if it were a (sprite) window.
  You can also use it as if it were.

Program description
  The screen layout is as follows.
    NBG0: Debug screen priority 7 256 colors
    NBG1: AM2 picture priority 6 256 colors
    NBG2: 2D game style BG priority 4 256 colors
    NBG3: Mountain picture priority 5 256 colors
  Then, apply extended color calculation to NBG1 and NBG3.
  To clarify the difference between extended color operations and normal color operations,
  Use the LR button on the controller to turn the extended color calculation enable bit ON/OFF.
  When the extended color calculation enable bit is turned OFF, the difference between the AM2 picture and the 2D game-like BG is
  Color calculations will no longer be performed.
  In order to perform extended color calculations, the resolution of the scroll surface must be normal.
  It has to be a resolution.
  In the center of the screen,
    "Calc Rate About NBG"
  is displayed, but this value is the value of the color calculation ratio register passed by the slColRate function.
  It is not the actual color calculation ratio.
  The actual color calculation ratio will vary depending on how the screens are stacked.

Method of operation
  start button
    Restore default settings.
  A button + direction key
    Movement of NBG1
  A button + L button
    Decrease in color calculation ratio of NBG1
  A button + R button
    Increase in color calculation ratio of NBG1
  B button + direction key
    NBG2 movement
  C button + direction key
    NBG3 movement
  C button + L button
    Decrease in color calculation ratio of NBG3
  C button + R button
    Increase in color calculation ratio of NBG3
  L button
   Turn on extended color calculation
  R button
   Turn off extended color calculation
