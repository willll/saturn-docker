◎About this sample

☆Purpose
  The samples under this directory are samples of special color operations.
  The basics are the same as the special priority, and the special color calculation bit of the map data.
  Performs color calculations on standing cells and dots.

☆Create data
  To perform special color operations, you need to
  1 must be set.
  Setting this bit cannot be performed using Map Editor for Windows95/NT.
  To set this bit, use the Macintosh Map/Screen Editor.
  Must be used.

☆Sample description
  This directory contains samples of special color operations on a dot basis and on a cell basis.
  Two samples are provided that perform special color calculations.
  Special color calculations for each cell can be performed using the special color calculation bits in the map data.
  Multiplies the color operation on the standing cells.
  On the other hand, special color calculation in units of dots uses special color calculation bits of map data.
  Dot color that matches the contents of the special function register among the cell data where is set.
  Performs color operations on the code.
  The color calculation method is the same as general color calculation.

  ■Special color calculation for each cell
    Special color calculations for each cell are performed using the special color calculation bits in the map data.
    Multiplies existing cell data by special color calculation.
    The procedure for multiplying special color operations is as follows in the sample:
      slSpecialCCalcModeNbg1( spPRI_Char );
    It is done like this.
    This line declares a special color operation on a cell-by-cell basis.
  ■Special color calculation in dot units
    Special color calculations in dot units are performed when the special color calculation bit in the map data is set.
    For the dot color data that matches the special function register among the cells in
    Performs color calculations. Taking this sample as an example,
      slSpecialCCalcModeNbg1( spPRI_Dot );
      slSpecialFuncCodeA( sfCOL_ef );
      slSpecialFuncSelectB( 0 );
    The first line declares a special dot-wise color operation.
    The next line sets the special function registers. In this example, the color code
    Performs special color calculations on color codes whose lower 4 bits are EH and FH.
    It will be.
    The last line specifies which of the two special function registers to use.
    Normally, this declaration is not necessary when using A, but in this sample, it is explicitly
    I am declaring. If you use B,
      slSpecialFuncSelectB( 1 );
    will do.

*Many of these features are similar to special priorities.
  Please also see the explanation of special priority (SATURN\SGL\SAMPLE4\SPPRIO\README).
  I think it will be more helpful if you receive it.
