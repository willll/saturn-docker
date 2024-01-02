◎About this sample
                                                    Sega Technical Support

☆Purpose of this sample
  This sample is a simple demonstration of VDP2's color offset feature.
  is. Color offset is like applying a filter to the original color.
  It can give an effect.

☆Sample description
  You can use either A or B for color offset.
  Deciding which surface to assign to which color offset is
  In this sample, with the slColOffsetAUse and slColOffsetBUse functions,
  I use it as follows.
    slColOffsetAUse( NBG1ON );
  Color offset is for each normal BG surface and rotating BG line color screen,
  It can be applied to the sprite surface (frame buffer).
  Also, to set the offset value, use the slColOffsetA and slColOffsetB functions.
  This sample uses only A, so
    slColOffsetA( i, - i, i / 2 );
  This is how I use it.
