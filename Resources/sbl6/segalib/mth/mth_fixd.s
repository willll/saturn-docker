!-----------------------------------------------------------------------------
!   mth_fixd.src -- MTH ���C�u���� FIXD ���W���[��
!   Copyright(c) 1994 SEGA
!   Written by T.S on 1994-01-24 Ver.0.80
!   Updated by H.E on 1994-02-15 Ver.0.80
!
!   ���̃��C�u�����͂r�g�ɂ������R�Q�r�b�g�Œ菬���_�̉��Z�ƁA�b�����̊����Z
!   ���[�`���̍����ł��܂ށB
!
!   _MTH_Mul               -  �Œ菬���_�̏��Z
!   _MTH_Div               -  �Œ菬���_�̏��Z
!   _MTH_IntToFixed        -  �������Œ菬���_�ɕϊ�
!   _MTH_FixedToInt        -  �Œ菬���_�𐮐��ɕϊ�
!   _MTH_Product           -  �R���Ϙa���Z
!
!   ���̃��C�u�������g�p�����ɂ͎��̃C���N���[�h�t�@�C�������`�����K�v�������B
!
!   #include "sega_xpt.h"
!   #include "sega_spr.h"
!
!-----------------------------------------------------------------------------
!
	.global _MTH_Mul
	.global _MTH_Div
	.global	_MTH_IntToFixed
	.global	_MTH_FixedToInt
	.global	_MTH_Product
!	.global	__divls

	.section .text


!
!---------------------------------------------------------------------
!  SH2 fixed point multiplication routine                   '94.01.24
!---------------------------------------------------------------------
!	C Langage Interface:
!
!	Fixed32 MTH_Mul( Fixed32 a, Fixed32 b )!
!
!	  Fixed point multiplication:
!	  H16.L16 x H16.L16 -> e16 H16.L16 o16
!	                           ~~~~~~~
!	    R4    x   R5    ->       R0
!
!       REGs USED:
!	       R0, R3..R5
!

	.align 2

_MTH_Mul:
 	STS	mach,r1
 	STS	macl,r2
	DMULS.L r4,r5
 	STS	mach,r3
 	STS	macl,r0
 	LDS	r1,mach
 	LDS	r2,macl
	RTS		! return
 	XTRCT	r3,r0	! xtract for Fixed32
!
!---------------------------------------------------------------------
!  SH2 fixed point division routine                         '94.01.24
!---------------------------------------------------------------------
!	C Langage Interface:
!
!	Fixed32 MTH_Div( Fixed32 a, Fixed32 b )!
!
!	   Fixed point division:
!	   e16 H16.L16 o16/ H16.L16 -> H16.L16
!	       ~~~~~~~
!	          R4      /   R5    ->   R0
!
!       REGs USED:
!	       R0, R4, R5
!
	.align 2

_MTH_Div:
	MOV.L	A_DIV,r0
	SWAP.W	r4,r2	! make R2:R4 64bit pair
	MOV.L	r5,@r0
	EXTS.W	r2,r2	!  0  R4H   R4L  0
	MOV.L	r2,@(0x10,r0)
	SHLL16	r4
	MOV.L	r4,@(0x14,r0)
	RTS
 	MOV.L	@(0x14,r0),r0
!
!---------------------------------------------------------------------
!  SH2 int to fixed point convert routine                   '94.01.24
!---------------------------------------------------------------------
!	C Langage Interface:
!
!	Fixed32 MTH_IntToFixed( int a )!
!
!	R4  << 16    ->   R0
!
!       REGs USED:
!	       nothing
!
	.align 2

_MTH_IntToFixed:
	MOV  	r4,r0
	RTS
	SHLL16	r0
!
!---------------------------------------------------------------------
!  SH2 fixed to int point convert routine                   '94.01.24
!---------------------------------------------------------------------
!	C Langage Interface:
!
!	Fixed32 MTH_FixedToInt( Fixed32 a )!
!
!	R4  >> 16    ->   R0
!
!       REGs USED:
!	       nothing
!
	.align 2

_MTH_FixedToInt:
	MOV  	r4,r0
	SHLR16	r0
	RTS
	EXTS.W	r0,r0
!
!---------------------------------------------------------------------
!  SH2   3 item multiple and add process                     '94.01.24
!---------------------------------------------------------------------
!	C Langage Interface:
!
!	Fixed32 MTH_Product( Fixed32 a[3], Fixed32 b[3] )!
!
!	a[0]*b[0] + a[1]*b[1] + a[2]*b[2] -> R0
!
!       REGs USED:
!	       R0-R7
!
	.align 2

_MTH_Product:                    ! function: MTH_Product
 	STS	mach,r1
 	STS	macl,r2
	MOV	#0,r0
	LDS	r0,mach
	LDS	r0,macl
	MAC.L   @r4+,@r5+
	MAC.L   @r4+,@r5+
	MAC.L   @r4+,@r5+
 	STS	mach,r3
 	STS	macl,r0
 	LDS	r1,mach
 	LDS	r2,macl
	RTS		! return
 	XTRCT	r3,r0	! xtract for Fixed32
!

	.align 2

__divls:
	MOV.L	r3,@-r15
	MOV	#0xF0,r3
	STC.L	sr,@-r15
	EXTU.B	r3,r3
	LDC	r3,sr
	MOV.L	A_DIV,r3
	MOV.L	r0,@r3		! SH2 DIV Unit
	NOP
	MOV.L	r1,@(0x4,r3)
	NOP
 	MOV.L	@(0x14,r3),r0	! get DIV result
	NOP
	LDC.L	@r15+,sr
	MOV.L	@r15+,r3
	RTS
	MOV	r0,r1

	.ALIGN 2

A_DIV:	.long	0xFFFFFF00
!
!---------------------------------------------------------------------
	.END
!---------------------------------------------------------------------
!   end of file
