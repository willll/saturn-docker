/*------------------------------------------------------------------------
 *  FILE:	SCL_Win0.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	ウインドウ関数共通サブルーチン
 *
 *  AUTHOR(S):
 *	K.M
 *
 *  MOD HISTORY:
 *	Written by K.M on 1994-06-21 Ver.1.00
 *	Updated by K.M on 1994-06-21 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 

extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;
extern	Uint16		SclProcess;

extern	Uint16	SclDisplayX;
extern	Uint16	SclDisplayY;

extern	Uint16	SclRPMD;	/* 回転パラメータモード */

void SCL_SetWindowSub(Uint32 surfaces,Uint32 enable,Uint8 *contrl,
				Uint8 on,Uint8 off)
{
    if( (enable & SCL_NBG0) || (enable & SCL_RBG1) )
    {
	if( (surfaces & SCL_NBG0) || (surfaces & SCL_RBG1) )
				contrl[1] |= on;
	else			contrl[1] &= off;
    }

    if( (enable & SCL_NBG1) || (enable & SCL_EXBG) )
    {
	if( (surfaces & SCL_NBG1) || (surfaces & SCL_EXBG) )
				contrl[0] |= on;
	else			contrl[0] &= off;
    }

    if(enable & SCL_NBG2)
    {
	if(surfaces & SCL_NBG2)	contrl[3] |= on;
	else			contrl[3] &= off;
    }

    if(enable & SCL_NBG3)
    {
	if(surfaces & SCL_NBG3)	contrl[2] |= on;
	else			contrl[2] &= off;
    }

    if(enable & SCL_RBG0)
    {
	if(surfaces & SCL_RBG0)	contrl[5] |= on;
	else			contrl[5] &= off;
    }

    if(enable & SCL_SPR)
    {
	if(surfaces & SCL_SPR)	contrl[4] |= on;
	else			contrl[4] &= off;
    }

    if(enable & SCL_RP)
    {
	if(surfaces & SCL_RP)	contrl[7] |= on;
	else			contrl[7] &= off;
    }

    if(enable & SCL_CC)
    {
	if(surfaces & SCL_CC)	contrl[6] |= on;
	else			contrl[6] &= off;
    }
}
