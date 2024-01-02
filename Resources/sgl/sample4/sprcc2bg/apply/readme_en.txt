1997-04-08
Sega Technical Support


The included sample program is
・Color calculation between NBG and line color
・Color calculation between sprite and BG
is.
The operation method etc. is documented at the beginning of MAIN.C.
Please refer to that.


The following is a simple FAQ when using the color calculation function.


Q: How to use color calculation for BG plane?
A: There are several modes for the color calculation function, but the easiest method is
    It will be as follows.

Example 1) When performing color calculations on all images under NBG0 at a rate of 50%
       slColRate( scnNBG0 , CLRate16_16 );
       slColorCalc( CC_RATE |CC_TOP| NBG0ON ); ← Specify the calculation ratio on the top image side

Example 2) When performing color calculations at different calculation rates for each image under NBG0
       slColRate(NBG1ON , Uint16 rate); ←For the required screen,
       slColRate(NBG2ON , Uint16 rate); ←Please set individually.
                   : ←
       slColRate(SPRON , Uint16 rate); ←
       slColorCalc( CC_RATE |CC_2ND| NBG0ON );
                               ↑
                             Specify the calculation ratio on the second image side

<How to use> void slColorCalc(Uint16 flag)
  Function: Set parameters for color calculation.
    flag : CC_RATE | CC_ADD : Calculation method
            CC_TOP | CC_2ND: Calculation ratio specification screen
            CC_EXT : Extended color operation
            NBG0ON | NBG1ON | NBG2ON | NBG3ON | RBG0ON | LNCLON | SPRON: Registration surface
        Please specify the above values ​​by connecting them with "|" according to the function to be used.

<How to use> void slColRate(Uint16 scrn , Uint16 rate)
  Function: Specify the color calculation ratio for each screen.
    scrn: scnNBG0, scnNBG1, scnNBG2, scnNBG3, scnRBG0, scnLNCL, scnBACK
           scnSPR0 , scnSPR1 , scnSPR2 , scnSPR3 , scnSPR4 , scnSPR5 , scnSPR6
            , scnSPR7
       Specify one of the following.
    rate: Specify the calculation rate. (32 steps) CLRate31_1 ~ CLRate0_32

For details about each function or functions not described in the REFERENCE manual,
  [SGL\DOC\SCROLL.TXT]
Please refer to it and contact technical support if you have any questions.
