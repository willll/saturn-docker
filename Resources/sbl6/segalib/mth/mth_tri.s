!*----------------------------------------------------------------------------
!*  mth_trif.c -- MTH ライブラリ 三角関数 モジュール
!*  Copyright(c) 1994 SEGA
!*  Written by H.E on 1994-04-28 Ver.0.80
!*  Updated by H.E on 1994-04-28 Ver.0.80
!*
!*  このライブラリは数学計算ライブラリの三角関数 モュールで、
!*  以下のルーチンを含む。
!*
!*  MTH_Sin                 -  ｓｉｎ関数
!*  MTH_Cin                 -  ｃｏｓ関数
!*  MTH_Atan                -  ａｔａｎ関数
!*
!*----------------------------------------------------------------------------
!
          .global     _MTH_Sin
          .global     _MTH_Cos
          .global     _MTH_Atan
          .global     _fsin
          .global     _fcos

	  .section .text

!
!
!*****************************************************************************
!*
!* NAME:  MTH_Sin()  - Return Sin Value
!*
!*     Fixed32 MTH_Sin(Fixed32 degree)!
!*
!* PARAMETERS :
!*
!* 　　(1) Fixed32   degree - <i> -180.0 から 180.0 までの角度
!*
!* DESCRIPTION:
!*
!*     指定角度の sin値を返す
!*
!*
!* POSTCONDITIONS:
!*
!*     Fixed32 sin値
!*
!* CAVEATS:
!*
!*****************************************************************************
!

	.align 2

_MTH_Sin:                              ! function: MTH_Sin
          MOV.L       r14,@-r15        ! r4  : degree
          STS.L       pr,@-r15         ! r14 : sign
!
          CMP/pz      r4               ! if(degree >= MTH_FIXED(0))
          BT.S        sin_0            !    sign = 0
          MOV         #0,r14           !    goto sin_0
          MOV         #1,r14           ! sign = 1
          NEG         r4,r4            ! degree  = -degree
sin_0:
          MOV.L       sin_f180,r2      ! r2 = MTH_FIXED(180)
          CMP/ge      r2,r4            ! if(degree < r2)
          BF          sin_1            !    goto sin_1
          BRA         sin_2            ! degree = MTH_FIXED(0)
          MOV         #0,r4            ! goto sin_2
sin_1:
          MOV.L       sin_f90,r3       ! r3 = MTH_FIXED(90)
          CMP/gt      r3,r4            ! if(degree <= r3)
          BF          sin_2            !    goto sin_2
          SUB         r4,r2            !
          MOV         r2,r4            ! degree = r2 - degree
sin_2:
          MOV.L       fsin,r3
          JSR         @r3              ! r0 = fsin(degree)
          NOP
          TST         r14,r14          ! if(sign == 0)
          BT          sin_3            !    goto sin_3
          NEG         r0,r0            ! r0 = -r0
sin_3:
          LDS.L       @r15+,pr
          RTS                          ! return r0
          MOV.L       @r15+,r14
!
	  .align 2

sin_f180: .long     0x00B40000       ! MTH_FIXED(180)
sin_f90:  .long     0x005A0000       ! MTH_FIXED(90)
fsin:     .long     _fsin
!
!
!*****************************************************************************
!*
!* NAME:  MTH_Cos()  - Return Cos Value
!*
!*     Fixed32 MTH_Cos(Fixed32 degree)!
!*
!* PARAMETERS :
!*
!* 　　(1) Fixed32   degree - <i> -180.0 から 180.0 までの角度
!*
!* DESCRIPTION:
!*
!*     指定角度の cos値を返す
!*
!*
!* POSTCONDITIONS:
!*
!*     Fixed32 cos値
!*
!* CAVEATS:
!*
!*****************************************************************************
!
_MTH_Cos:                              ! function: MTH_Cos
          MOV.L       r14,@-r15        ! r4  : degree
          STS.L       pr,@-r15         ! r14 : sign
!
          CMP/pz      r4               ! if(degree >= MTH_FIXED(0))
          BT          cos_0            !    goto cos_0
          NEG         r4,r4            ! degree = -degree
cos_0:
          MOV.L       cos_f180,r2      ! r2 = MTH_FIXED(180)
          CMP/ge      r2,r4            ! sin = 1
          BF.s        cos_1            ! if(degree < MTH_FIXED(180))
          MOV         #1,r14           !    goto cos_1
          BRA         cos_4            ! degree = MTH_FIXED(0)
          MOV         #0,r4            ! goto cos_4
cos_1:
          MOV.L       cos_f90,r2       ! r2 = MTH_FIXED(90)
          CMP/gt      r2,r4            ! if(degree > MTH_FIXED(90))
          BF          cos_2            !    goto cos_2
          MOV.L       cos_f180,r2      ! r2 = MTH_FIXED(180)
          SUB         r4,r2            ! r2 -= degree
          BRA         cos_4            ! degree = r2
          MOV         r2,r4            ! goto cos_4
cos_2:
          MOV         #0,r14           ! sign = 0
cos_4:
          MOV.L       fcos,r3
          JSR         @r3              ! r0 = fcos(degree)
          NOP
          TST         r14,r14          ! if(sign == 0)
          BT          cos_5            !    goto cos_5
          NEG         r0,r0            ! r0 = -r0
cos_5:
          LDS.L       @r15+,pr
          RTS                          ! return r0
          MOV.L       @r15+,r14
!
	  .align 2

cos_f180: .long     0x00B40000       ! MTH_FIXED(180)
cos_f90:  .long     0x005A0000       ! MTH_FIXED(90)
fcos:     .long     _fcos
!
!
!*****************************************************************************
!*
!*      Cosine(a) Subroutine
!*      Cosine(a) = Sine(90-a)
!*      In :a		(r4)
!*      Out:Cosine(a)   (r0)
!*
!*****************************************************************************
_fcos:
	MOV.L	NINTY,r2	!90-a ->r4
	NEG	r4,r4
	ADD	r2,r4
!
!
!*****************************************************************************
!*
!*      Sine (a) Subroutine
!*      In:a = angle (0 - 89)(r4)
!*      Out:Sine(a)          (r0)
!*
!*****************************************************************************
!
_fsin:
	MOVA	SINTBL,r0	!sine table start address ->r1
	STS	macl,r3

	SWAP.W	r4,r1
	EXTU.W	r1,r1
	SHLL2	r1		!r0<-Entry of The Table
	MOV.W	@(r0,r1),r2	!sin(a) GRAD->r2

	ADD	#2,r0
	MULU	r4,r2
	MOV.W	@(r0,r1),r0	!sin(a) value->r0
	STS	macl,r2
	EXTU.W	r0,r0
	LDS	r3,macl
	SWAP.W	r2,r2
	EXTU.W	r2,r2
	RTS
	ADD	r2,r0
!
!	Constant Data definiton

	.align 2

NINTY:	.long	0x005A0000	!90

!Sine Table Data Area(AREA SIZE = 91 x 2 x 2 = 364BYTES
!	 ALIGN=4
SINTBL:
!	         delta, value
	.word	0x0477,0x0000		!00deg
	.word	0x0478,0x0477		!01deg
	.word	0x0476,0x08EF		!02deg
	.word	0x0476,0x0D65		!03deg
	.word	0x0474,0x11DB		!04deg
	.word	0x0473,0x164F		!05deg
	.word	0x0470,0x1AC2		!06deg
	.word	0x046E,0x1F32		!07deg
	.word	0x046C,0x23A0		!08deg
	.word	0x0468,0x280C		!09deg
	.word	0x0464,0x2C74		!10deg
	.word	0x0461,0x30D8		!11deg
	.word	0x045D,0x3539		!12deg
	.word	0x0458,0x3996		!13deg
	.word	0x0453,0x3DEE		!14deg
	.word	0x044F,0x4241		!15deg
	.word	0x0448,0x4690		!16deg
	.word	0x0443,0x4AD8		!17deg
	.word	0x043D,0x4F1B		!18deg
	.word	0x0436,0x5358		!19deg
	.word	0x0430,0x578E		!20deg
	.word	0x0428,0x5BBE		!21deg
	.word	0x0420,0x5FE6		!22deg
	.word	0x0419,0x6406		!23deg
	.word	0x0411,0x681F		!24deg
	.word	0x0409,0x6C30		!25deg
	.word	0x03FF,0x7039		!26deg
	.word	0x03F7,0x7438		!27deg
	.word	0x03ED,0x782F		!28deg
	.word	0x03E3,0x7C1C		!29deg
	.word	0x03DA,0x7FFF		!30deg
	.word	0x03CF,0x83D9		!31deg
	.word	0x03C5,0x87A8		!32deg
	.word	0x03BA,0x8B6D		!33deg
	.word	0x03AE,0x8F27		!34deg
	.word	0x03A4,0x92D5		!35deg
	.word	0x0397,0x9679		!36deg
	.word	0x038B,0x9A10		!37deg
	.word	0x0380,0x9D9B		!38deg
	.word	0x0372,0xA11B		!39deg
	.word	0x0366,0xA48D		!40deg
	.word	0x0359,0xA7F3		!41deg
	.word	0x034B,0xAB4C		!42deg
	.word	0x033E,0xAE97		!43deg
	.word	0x032F,0xB1D5		!44deg
	.word	0x0322,0xB504		!45deg
	.word	0x0313,0xB826		!46deg
	.word	0x0305,0xBB39		!47deg
	.word	0x02F6,0xBE3E		!48deg
	.word	0x02E7,0xC134		!49deg
	.word	0x02D8,0xC41B		!50deg
	.word	0x02C8,0xC6F3		!51deg
	.word	0x02B8,0xC9BB		!52deg
	.word	0x02A8,0xCC73		!53deg
	.word	0x0298,0xCF1B		!54deg
	.word	0x0288,0xD1B3		!55deg
	.word	0x0278,0xD43B		!56deg
	.word	0x0266,0xD6B3		!57deg
	.word	0x0256,0xD919		!58deg
	.word	0x0244,0xDB6F		!59deg
	.word	0x0234,0xDDB3		!60deg
	.word	0x0221,0xDFE7		!61deg
	.word	0x0211,0xE208		!62deg
	.word	0x01FE,0xE419		!63deg
	.word	0x01EC,0xE617		!64deg
	.word	0x01DB,0xE803		!65deg
	.word	0x01C8,0xE9DE		!66deg
	.word	0x01B5,0xEBA6		!67deg
	.word	0x01A4,0xED5B		!68deg
	.word	0x0190,0xEEFF		!69deg
	.word	0x017E,0xF08F		!70deg
	.word	0x016B,0xF20D		!71deg
	.word	0x0158,0xF378		!72deg
	.word	0x0145,0xF4D0		!73deg
	.word	0x0131,0xF615		!74deg
	.word	0x011F,0xF746		!75deg
	.word	0x010B,0xF865		!76deg
	.word	0x00F7,0xF970		!77deg
	.word	0x00E4,0xFA67		!78deg
	.word	0x00D1,0xFB4B		!79deg
	.word	0x00BD,0xFC1C		!80deg
	.word	0x00A9,0xFCD9		!81deg
	.word	0x0095,0xFD82		!82deg
	.word	0x0081,0xFE17		!83deg
	.word	0x006E,0xFE98		!84deg
	.word	0x005A,0xFF06		!85deg
	.word	0x0046,0xFF60		!86deg
	.word	0x0032,0xFFA6		!87deg
	.word	0x001E,0xFFD8		!88deg
	.word	0x000A,0xFFF6		!89deg
	.word	0x0000,0xFFFF		!90deg
!
!
!*****************************************************************************
!*
!* NAME:  MTH_Atan()  - Return Atan Value
!*
!*     Fixed32 MTH_Atan(Fixed32 y, Fixed32 x)!
!*
!* PARAMETERS :
!*
!* 　　(1) Fixed32   y     - <i> -1.0 から 1.0 までの高さ
!* 　　(2) Fixed32   x     - <i> -1.0 から 1.0 までの底辺
!*
!* DESCRIPTION:
!*
!*     指定 x, y 値から角度を返す
!*
!*
!* POSTCONDITIONS:
!*
!*     Fixed32 -180.0 から 180.0 までの角度
!*
!* CAVEATS:
!*
!*****************************************************************************
!
_MTH_Atan:                       ! function: MTH_Atan
          MOV.L       r14,@-r15
          MOV.L       r13,@-r15
          MOV.L       r12,@-r15
          MOV.L       r11,@-r15
          MOV.L       r10,@-r15
          MOV.L       r9,@-r15
 	  			STS.L	      mach,@-r15
 	  			STS.L	      macl,@-r15
          MOV         r5,r10           ! r10 = x
          STS.L       pr,@-r15
!
          MOV.L       rTanTbl,r11      ! r11 = tanTbl pointer

          CMP/pz      r10              ! r12 = y
          BT.s        atan_0           ! if(x >= 0)
          MOV         r4,r12           !    goto atan_0
          NEG         r10,r5           ! r5 : ax = -x
atan_0:
          CMP/pz      r12              ! if(y >= 0)
          BT          atan_2           !    goto atan_2
          NEG         r12,r4           ! r4 : ay = -y
atan_2:
          MOV         #65,r2           ! r2 = MTH_FIXED(0.001)
          CMP/ge      r2,r5            ! if(ax >= r2)
          BT          atan_4           !    goto atan_4
          MOV.L       atan_fmax,r13    ! r13 : w =  0x7fff0000
          BRA         atan_5           ! goto atan_5
          NOP
atan_4:
	  MOV.L	      A_DIV,r0
	  SWAP.W      r4,r2	       ! make r2:r4 64bit pair
	  MOV.L	      r5,@r0
	  EXTS.W      r2,r2            !  0  r4H   r4L  0
	  MOV.L	      r2,@(0x10,r0)
	  SHLL16      r4
	  MOV.L	      r4,@(0x14,r0)
 	  MOV.L	      @(0x14,r0),r13   ! r13 : w = ay / ax
atan_5:
          MOV         #0,r4            ! r4 : left  = 0
          MOV         #90,r5           ! r5 : right = 90
atan_6:
          MOV         r4,r14
          ADD         r5,r14
          SHAR        r14              ! r14 : mid = (left + right) >> 1
          MOV         r14,r0
          SHLL2       r0
          SHLL        r0               ! r0 = mid * 8
          MOV.L       @(r0,r11),r2     ! r2 = tanTbl[mid].val
          CMP/gt      r13,r2           ! if(r2 > w)
          BT          atan_7           !    goto atan_7
          MOV         r14,r4
          ADD         #1,r4            ! r4 : left = mid + 1
          MOV         r4,r0
          SHLL2       r0
          SHLL        r0               ! r0 = left * 8
          MOV.L       @(r0,r11),r3     ! r2 = tanTbl[left].val
          CMP/ge      r3,r13           ! if(w >= r3)
          BT          atan_8           !    goto atan_8
          BRA         atan_9           ! goto atan_9 (break)
          NOP
atan_7:
          MOV         r14,r5           ! r5 : right = mid
atan_8:
          CMP/ge      r5,r4            ! if(left < right)
          BF          atan_6           !   goto atan_6
atan_9:
          MOV         r14,r0           ! r0 : mid
          MOV         r13,r4           ! r4 : w
          SHLL2       r0
          SHLL        r0               ! r0 = mid * 8
          MOV.L       @(r0,r11),r3     ! r3 = tanTbl[mid].val
          SUB         r3,r4            ! r4 : w - r3
          ADD         #4,r0
          MOV.L       @(r0,r11),r5     ! r5 : tanTbl[mid].k
	  			DMULS.L     r4,r5
 	  			STS	      mach,r3
 	  			STS	      macl,r4
 	  			XTRCT	      r3,r4	       ! r4 : degree = MTH_Mul(r4,r5)
          MOV.L       atan_f1,r2       ! r2 = MTH_FIXED(1.0)
          CMP/gt      r2,r4            ! if(degree <= MTH_FIXED(1.0))
          BF          atan_10          !    goto atan_10
          MOV.L       atan_f1,r4       ! r4 : degree = MTH_FIXED(1.0)
atan_10:
	  			MOV  	      r14,r0
	  			SHLL16      r0               ! r0 = MTH_IntToFixed(mid)

          CMP/pz      r12              ! r4 : degree + = r0
          BT.S        atan_11          ! if(y >= MTH_FIXED(0))
          ADD         r0,r4            !    goto atan_11
          NEG         r4,r4            ! r4 : degree = -degree
atan_11:
          CMP/pz      r10              ! if(x >= MTH_FIXED(0))
          BT          atan_14          !    goto atan_14
          CMP/pz      r12              ! if(y < MTH_FIXED(0))
          BF          atan_12          !    goto atan_12
          MOV.L       atan_f180,r2     ! r2 = MTH_FIXED(180.0)
          BRA         atan_13          ! goto atan_13
          NOP
atan_12:
          MOV.L       atan_fm180,r2    ! r2 = MTH_FIXED(-180.0)
atan_13:
          SUB         r4,r2
          MOV         r2,r4            ! r4 = r2 - degree
atan_14:
          MOV         r4,r0            ! r0 = r4
          LDS.L       @r15+,pr
 	  			LDS.L      	@r15+,macl
 	  			LDS.L      	@r15+,mach
          MOV.L       @r15+,r9
          MOV.L       @r15+,r10
          MOV.L       @r15+,r11
          MOV.L       @r15+,r12
          MOV.L       @r15+,r13
          RTS                          ! return r0
          MOV.L       @r15+,r14
!
	.align 2

A_DIV:
	  			.long     0xFFFFFF00
rTanTbl:
          .long     tanTbl
atan_fmax:
          .long     0x7FFF0000
atan_f1:
          .long     0x00010000
atan_f180:
          .long     0x00B40000
atan_fm180:
          .long     0xFF4C0000
tanTbl:                            ! static: tanTbl
          .long      0x00000000,0x00394A3A    !/*   0 */
          .long      0x00000477,0x0039414B    !/*   1 */
          .long      0x000008F0,0x00392F70    !/*   2 */
          .long      0x00000D6A,0x003914AD    !/*   3 */
          .long      0x000011E6,0x0038F10C    !/*   4 */
          .long      0x00001665,0x0038C498    !/*   5 */
          .long      0x00001AE8,0x00388F5F    !/*   6 */
          .long      0x00001F6E,0x00385170    !/*   7 */
          .long      0x000023FA,0x00380AE0    !/*   8 */
          .long      0x0000288B,0x0037BBC4    !/*   9 */
          .long      0x00002D23,0x00376435    !/*  10 */
          .long      0x000031C2,0x0037044F    !/*  11 */
          .long      0x0000366A,0x00369C2F    !/*  12 */
          .long      0x00003B1A,0x00362BF6    !/*  13 */
          .long      0x00003FD3,0x0035B3C7    !/*  14 */
          .long      0x00004498,0x003533C7    !/*  15 */
          .long      0x00004968,0x0034AC1E    !/*  16 */
          .long      0x00004E44,0x00341CF7    !/*  17 */
          .long      0x0000532D,0x0033867E    !/*  18 */
          .long      0x00005825,0x0032E8E2    !/*  19 */
          .long      0x00005D2D,0x00324454    !/*  20 */
          .long      0x00006244,0x00319908    !/*  21 */
          .long      0x0000676E,0x0030E733    !/*  22 */
          .long      0x00006CAA,0x00302F0C    !/*  23 */
          .long      0x000071FA,0x002F70CD    !/*  24 */
          .long      0x0000775F,0x002EACB2    !/*  25 */
          .long      0x00007CDC,0x002DE2F7    !/*  26 */
          .long      0x00008270,0x002D13DB    !/*  27 */
          .long      0x0000881E,0x002C3F9F    !/*  28 */
          .long      0x00008DE7,0x002B6686    !/*  29 */
          .long      0x000093CD,0x002A88D2    !/*  30 */
          .long      0x000099D2,0x0029A6C9    !/*  31 */
          .long      0x00009FF7,0x0028C0B2    !/*  32 */
          .long      0x0000A63F,0x0027D6D3    !/*  33 */
          .long      0x0000ACAC,0x0026E978    !/*  34 */
          .long      0x0000B340,0x0025F8E8    !/*  35 */
          .long      0x0000B9FE,0x0025056F    !/*  36 */
          .long      0x0000C0E8,0x00240F5A    !/*  37 */
          .long      0x0000C802,0x002316F5    !/*  38 */
          .long      0x0000CF4E,0x00221C8D    !/*  39 */
          .long      0x0000D6CF,0x00212071    !/*  40 */
          .long      0x0000DE89,0x002022EE    !/*  41 */
          .long      0x0000E680,0x001F2455    !/*  42 */
          .long      0x0000EEB9,0x001E24F5    !/*  43 */
          .long      0x0000F737,0x001D251D    !/*  44 */
          .long      0x00010000,0x001C251D    !/*  45 */
          .long      0x00010918,0x001B2545    !/*  46 */
          .long      0x00011286,0x001A25E5    !/*  47 */
          .long      0x00011C51,0x0019274C    !/*  48 */
          .long      0x0001267E,0x001829C9    !/*  49 */
          .long      0x00013116,0x00172DAD    !/*  50 */
          .long      0x00013C22,0x00163345    !/*  51 */
          .long      0x000147AA,0x00153AE0    !/*  52 */
          .long      0x000153B9,0x001444CB    !/*  53 */
          .long      0x0001605A,0x00135152    !/*  54 */
          .long      0x00016D9B,0x001260C2    !/*  55 */
          .long      0x00017B89,0x00117366    !/*  56 */
          .long      0x00018A34,0x00108988    !/*  57 */
          .long      0x000199AF,0x000FA371    !/*  58 */
          .long      0x0001AA0E,0x000EC168    !/*  59 */
          .long      0x0001BB67,0x000DE3B4    !/*  60 */
          .long      0x0001CDD6,0x000D0A9B    !/*  61 */
          .long      0x0001E177,0x000C365F    !/*  62 */
          .long      0x0001F66D,0x000B6743    !/*  63 */
          .long      0x00020CE0,0x000A9D88    !/*  64 */
          .long      0x000224FE,0x0009D96D    !/*  65 */
          .long      0x00023EFC,0x00091B2E    !/*  66 */
          .long      0x00025B19,0x00086307    !/*  67 */
          .long      0x0002799F,0x0007B132    !/*  68 */
          .long      0x00029AE7,0x000705E6    !/*  69 */
          .long      0x0002BF5A,0x00066158    !/*  70 */
          .long      0x0002E77A,0x0005C3BC    !/*  71 */
          .long      0x000313E3,0x00052D43    !/*  72 */
          .long      0x00034556,0x00049E1C    !/*  73 */
          .long      0x00037CC7,0x00041673    !/*  74 */
          .long      0x0003BB67,0x00039673    !/*  75 */
          .long      0x000402C2,0x00031E44    !/*  76 */
          .long      0x000454DB,0x0002AE0B    !/*  77 */
          .long      0x0004B462,0x000245EB    !/*  78 */
          .long      0x00052501,0x0001E605    !/*  79 */
          .long      0x0005ABD9,0x00018E76    !/*  80 */
          .long      0x00065052,0x00013F5A    !/*  81 */
          .long      0x00071D88,0x0000F8CA    !/*  82 */
          .long      0x000824F3,0x0000BADB    !/*  83 */
          .long      0x000983AD,0x000085A2    !/*  84 */
          .long      0x000B6E17,0x0000592D    !/*  85 */
          .long      0x000E4CF8,0x0000358D    !/*  86 */
          .long      0x001314C5,0x00001ACA    !/*  87 */
          .long      0x001CA2E1,0x000008EF    !/*  88 */
          .long      0x00394A3A,0x00000105    !/*  89 */
          .long      0x7FFFFFFF,0x00000000    !/*  90 */

					.END
