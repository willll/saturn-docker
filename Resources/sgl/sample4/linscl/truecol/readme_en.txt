◎About this sample

  This sample uses vertical cell scrolling/line scrolling.
This is a sample of full screen size display of 16 million color bitmap.

Topics
  This sample uses two techniques:
  ・Vertical cell/line scroll
  ・Technique for displaying a 16 million color bitmap screen

explanation
  We will explain how to use each topic listed above.
  1) Vertical cell/line scroll
      About vertical cell scrolling
        Vertical cell scrolling requires setting a cycle pattern.
        The cycle pattern of vertical cells cannot be set using SGL's slAutoDisp function.
        Therefore, if you want to use vertical cell scrolling, use the slScrCycleSet function
        and configure it yourself.
        One cycle is required to read the vertical cell scroll table, but it is relatively short.
        Since the setting conditions are strict, you must be very careful when setting.
        Setting cycle patterns itself is difficult for first-time users to understand, so it is easy for users to
        To set the cycle pattern on the side, set the slScrAutoDisp limit in advance.
        Call slScrAutoDisp with the settings excluding the information part.
        When slScrAutoDisp completes the settings successfully, the system variable area,
        The cycle pattern at that time is set after VDP2_CYCA0L. that data
        Configure settings from the original.
      About line scrolling
        SATURN's line scrolling functionality includes horizontal and vertical movement,
        There is a function for scaling in line units, and each has a fixed decimal (FIXED)
        It is equipped with a wide range of highly functional items.
        In this sample, we use horizontal and vertical movement, but zoom
        If you use this function together, you can create a screen with depth using only one scroll surface.
        You can.
        Line scrolling uses a cycle pattern like vertical cell scrolling.
        Basically, it is stored in a specified register using a scroll table.
        You can easily use it by simply registering it.
  2) About bitmap display
      This sample program uses the
      I am using a technique to display a bitmap.
      In the case of SATURN, only if the original picture size is 512 or 1024 horizontally, continuous
      Data can be taken, but in other cases, if the data exceeds the horizontal size of the picture.
      It is necessary to transfer data by skipping parts, which increases the processing load.
      I'll put it away.
      STN-14 uses line scrolling and vertical cell scrolling to create continuous
      Explains the display when storing original data on VRM.
      The data used in this sample is 320 x 224 16 million color bitmap data.
      This is displayed using in-scroll and vertical cell scroll.
      Note that bitmap data is placed across multiple banks.
      Therefore, for all banks with bitmap data, the bitmap data
      This means that you must set up a cycle pattern for access.
      This is not set by the slScrAutoDisp function like vertical cell scrolling, so
      It is set using the slScrCycleSet function.
      However, the lScrAutoDisp function is also used to set the scroll screen display enable bit.
      I'm using. The first thing you need to be careful about is:
      If you set vertical cell scrolling, slScrAutoDisp will return an error.
      Since nothing is processed, the settings you originally wanted will not be made.
