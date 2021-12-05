!----------------------------------------------------------------------------
!  mth_norm.src -- MTH ライブラリ 法線ベクタ計算 モジュール
!  Copyright(c) 1994 SEGA
!  Written by H.E on 1994-06-22 Ver.1.00
!  Updated by H.E on 1994-06-22 Ver.1.00
!
!  このライブラリは法線ベクタ計算モジュールで、
!  以下のルーチンを含む。
!
!  _MTH_ComputeNormalVect       -  法線ベクタ計算関数
!
!----------------------------------------------------------------------------
          .global     _MTH_ComputeNormVect
!
.if 1
.equ SAVE_r14,0
.equ NORMAL_P,4

.equ X,0
.equ Y,4
.equ Z,8
.endif

.if 0
          .SECTION    CALL_STACK,DUMMY
SAVE_r14:  .long	    0
NORMAL_P:  .long      0
!
          .SECTION    MthXyz,DUMMY
X:         .long      0
Y:         .long      0
Z:         .long      0
.endif


          .section .text

!
!-----------------------------------------------------------------------------
!
! NAME:  MTH_ComputeNormVect()  - Compute Normal Vector
!
!  void MTH_ComputeNormVect(Fixed32 surfNormK,
!			 MthXyz *p0, MthXyz *p1, MthXyz *p2, MthXyz *normal)
!
! PARAMETERS :
!
! 　　(1) Fixed32  surfNormK   - <i>  法線ベクトルの計算補正値
! 　　(2) MthXyz   *p0         - <i>  頂点０
! 　　(3) MthXyz   *p1         - <i>  頂点１
! 　　(4) MthXyz   *p2         - <i>  頂点２
! 　　(5) MthXyz   *normal     - <o>  法線
!
! DESCRIPTION:
!
!     指定３頂点座標より法線ベクトル（単位ベクトル）を計算する
!
!
! POSTCONDITIONS:
!
!
! CAVEATS:
!
!-----------------------------------------------------------------------------
!
	  .align 2

_MTH_ComputeNormVect:
          MOV.L       r14,@-r15         ! save register
          MOV         r15,r14
          MOV.L       r11,@-r15
          MOV.L       r10,@-r15
          MOV.L       r9,@-r15
          MOV.L       r8,@-r15
          STS.L       pr,@-r15
          STS.L       macl,@-r15
          STS.L       mach,@-r15
!
          CMP/pz      r4                ! r4 : surfNormK
          BT.S        cnv_00            ! if(surfNormK >= 0) goto cnv00
          MOV         #1,r8             ! r8 : sign = 1
          MOV         #-1,r8            !      sign = -1
          NEG         r4,r4             ! surfNormK *= -1
cnv_00:                                 ! r5 : p0, r6 : p1, r7 : p2
!
          MOV.L       @(Z,r6),r0        ! b0z = (p1->z - p0->z) * surfNormK
          MOV.L       @(Z,r5),r1
          SUB         r1,r0
	  DMULS.L     r0,r4
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@-r15          ! push b0z
!
          MOV.L       @(Y,r6),r0        ! b0y = (p1->y - p0->y) * surfNormK
          MOV.L       @(Y,r5),r1
          SUB         r1,r0
	  DMULS.L     r0,r4
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@-r15          ! push b0y
!
          MOV.L       @(X,r6),r0        ! b0x = (p1->x - p0->x) * surfNormK
          MOV.L       @(X,r5),r1
          SUB         r1,r0
	  DMULS.L     r0,r4
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@-r15          ! push b0x
          MOV         r15,r2            ! @r2 = (b0x, b0y, b0z)
!
          MOV.L       @(Z,r7),r0        ! b1z = (p2->z - p1->z) * surfNormK
          MOV.L       @(Z,r6),r1
          SUB         r1,r0
	  DMULS.L     r0,r4
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@-r15          ! push b1z
!
          MOV.L       @(Y,r7),r0        ! b1y = (p2->y - p1->y) * surfNormK
          MOV.L       @(Y,r6),r1
          SUB         r1,r0
	  DMULS.L     r0,r4
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@-r15          ! push b1y
!
          MOV.L       @(X,r7),r0        ! b1x = (p2->x - p1->x) * surfNormK
          MOV.L       @(X,r6),r1
          SUB         r1,r0
	  DMULS.L     r0,r4
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@-r15          ! push b1x
          MOV         r15,r3            ! @r3 = (b1x, b1y, b1z)
!
          MOV.L       @(Z,r2),r0        ! nx = b0z*b1y - b0y*b1z
          MOV.L       @(Y,r3),r1        ! r9     r9        r4
	  DMULS.L     r0,r1
 	  STS         mach,r1
 	  STS	      macl,r9
 	  XTRCT	      r1,r9
          MOV.L       @(Y,r2),r0
          MOV.L       @(Z,r3),r1
	  DMULS.L     r0,r1
 	  STS         mach,r1
 	  STS	      macl,r4
 	  XTRCT	      r1,r4
          SUB         r4,r9
!
          MOV.L       @(X,r2),r0        ! ny = b0x*b1z - b0z*b1x
          MOV.L       @(Z,r3),r1        ! r10    r10       r4
	  DMULS.L     r0,r1
 	  STS         mach,r1
 	  STS	      macl,r10
 	  XTRCT	      r1,r10
          MOV.L       @(Z,r2),r0
          MOV.L       @(X,r3),r1
	  DMULS.L     r0,r1
 	  STS         mach,r1
 	  STS	      macl,r4
 	  XTRCT	      r1,r4
          SUB         r4,r10
!
          MOV.L       @(Y,r2),r0        ! nz = b0y*b1x - b0x*b1y
          MOV.L       @(X,r3),r1        ! r11    r11       r4
	  DMULS.L     r0,r1
 	  STS         mach,r1
 	  STS	      macl,r11
 	  XTRCT	      r1,r11
          MOV.L       @(X,r2),r0
          MOV.L       @(Y,r3),r1
	  DMULS.L     r0,r1
 	  STS         mach,r1
 	  STS	      macl,r4
 	  XTRCT	      r1,r4
          SUB         r4,r11
!
	  DMULS.L     r9,r9             ! r4 : w = nx*nx + ny*ny + nz*nz
 	  STS         mach,r1           !            r9     r10     r11
 	  STS	      macl,r4
 	  XTRCT	      r1,r4
	  DMULS.L     r10,r10
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          ADD         r0,r4
	  DMULS.L     r11,r11
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          ADD         r0,r4
!
          MOV         #65,r2           ! r2 = 0.001
          CMP/ge      r2,r4            ! if(w >= 0.001)
          BT          cnv_01           !    goto cnv_01
          MOV         #65,r4           ! w = 0.001
cnv_01:
          MOV.L       FISQRT,r3
          JSR         @r3
          NOP
          MUL.L       r8,r0
          STS         macl,r4          ! w = FISQRT(w) * sign
!
          MOV.L       @(NORMAL_P,r14),r7     ! @r7 : normal
!
	  DMULS.L     r9,r4            ! normal->x = nx * w
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@(X,r7)
!
	  DMULS.L     r10,r4           ! normal->y = ny * w
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@(Y,r7)
!
	  DMULS.L     r11,r4            ! normal->z = nz * w
 	  STS         mach,r1
 	  STS	      macl,r0
 	  XTRCT	      r1,r0
          MOV.L       r0,@(Z,r7)
!
          ADD         #24,r15           ! restore register
          LDS.L       @r15+,mach
          LDS.L       @r15+,macl
          LDS.L       @r15+,pr
          MOV.L       @r15+,r8
          MOV.L       @r15+,r9
          MOV.L       @r15+,r10
          MOV.L       @r15+,r11
          RTS
          MOV.L       @r15+,r14
!
	  .align 2

FISQRT:	  .long    _FISQRT
!
!
! C interface:
!	  Fixed32 FISQRT(Fixed32 a)!
!
! return:
!	  1/sqrt(a)
!
! method:
!	  Newton-Raphson
!         xn+1 = xn*(3-a*xn*xn)/2
!
! note:   macl, mach not saved
!
! pgm:	  1994.05.14 by T. Suzuki (SEGA C/S Hard dpt.)
!
!
_FISQRT:
	XOR	r0,r0
	CMP/pl	r4		! if (a<=0) return 0!
	BF	RETURN
	MOV.L	C_1,r0
	XOR	r6,r6
	MOV.L	C_4,r7
NORMLZ:				! normalize 1/4 <= a < 1
	CMP/ge	r0,r4
	BF	NORMLZ2
	ADD	#1,r6
	BRA	NORMLZ
	SHLr2	r4
NORMLZ2:
	CMP/ge	r4,r7
	BF	NEWTON
	ADD	#-1,r6
	BRA	NORMLZ2
	SHLL2	r4
NEWTON:
	MOV	#3,r5		! only 3 times do Newton
REPEAT:
	DMULS.L	r0,r0
	MOV.L	C_3,r1
	STS	macl,r2
	STS	mach,r3
	XTRCT	r3,r2
	DMULS.L	r4,r2
	STS	macl,r2
	STS	mach,r3
	XTRCT	r3,r2
	SUB	r2,r1
	DMULS.L	r0,r1
	STS	macl,r0
	STS	mach,r3
	XTRCT	r3,r0
	SHAR	r0
	DT	r5
	BF	REPEAT
DENORMLZ:
	TST	r6,r6
	BT	RETURN
	CMP/pl	r6
	BF	DENORML2
	ADD	#-1,r6
	BRA	DENORMLZ
	SHLR	r0
DENORML2:
	ADD	#1,r6
	BRA	DENORMLZ
	SHLL	r0
RETURN:
	RTS
	NOP
!
	.align 2

C_1:	.long	0x00010000	! const 1
C_3:	.long	0x00030000	! const 3
C_4:	.long	0x00004000	! const 1/4
!
        .END
