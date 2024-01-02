◎About these samples

☆Purpose
  The sample under this directory uses VDP2 special priority.
  This is a sample program.

☆How to create data
  To perform special priority, certain bits of the map data are
  1 must be set.
  Setting this bit cannot be performed using Map Editor for Windows95/NT.
  To set this bit, use the Macintosh Map/Screen Editor.
  Must be used.

☆Sample description
  This directory contains samples of special priorities in units of dots and
  Two samples are provided that perform special priorities.
  The special priority of each cell is the special priority bit of the map data.
  and perform special priority operations on all cells in which bits are set.
  I will do it.
  The special priority in dot units is the special priority bit set.
  For cell data, the pixel with the color code specified in the special function register
  A special priority calculation is performed on the
  Lower 1 bit of the priority number of the cell or dot that performs special priority
  Set 1 to .
  Therefore, depending on the priority number, there may be no effect at all.
  Caution is required.

  ■Special priority for each cell
    The special priority for each cell is the special priority in the map data.
    Multiply cell data with a set bit by a special priority.
    The procedure for multiplying special priorities is as follows in the sample:
      slSpecialPrioModeNbg1( spPRI_Char );
      slSpecialPrioBitNbg1( ON );
    It is done like this.
    The first line is a declaration of special priority use on a per-cell basis.
    The next line is the special priority bit in the pattern name control register.
    stand up. Normally this bit is only set when the map data is in 1 Word mode.
    will be used.
  ■Special priority per dot
    The special priority in dot units is the special priority in the map data.
    The lower part of the color code in the pixel data of the cell data where the bit is set
    Special priority for those whose 4 bits match the special function register settings
    will be processed. If you actually look at the sample description,
      slSpecialPrioModeNbg1( spPRI_Dot );
      slSpecialFuncCodeA( sfCOL_cd );
      slSpecialFuncSelectB( 0 );
    The first line declares the use of special priority in dots.
    The next line sets the special function registers.
    With this setting, when the lower 4 bits of the color code are 0EH or 0FH, a special
    Performs priority processing. In this sample, the mode is 256 colors, so
    0C, 0D, 1C, 1D...FC, FD
    A special priority will be applied to the color code.
    The last line shows that there can be two special function registers, A and B, so which one should be selected?
    Specify whether to use it.

*When using special priority, it may be very difficult to design pallets and cells.
  I understand. However, if properly designed, it can be quite effective.
  Is possible.
