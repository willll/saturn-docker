/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr13.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	Scroll Operation function program
 *
 *  AUTHOR(S):
 *	K.M
 *		
 *  MOD HISTORY:
 *	Written by K.M on 1994-07-01 Ver.1.00
 *	Updated by K.M on 1994-07-01 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#include <sega_scl.h> 

/* ボカシ演算を有効にする（拡張カラー演算を無効にする） */
/* 画面上にぼかし効果をだすには、ぼかし有効にした画面をカラー演算すること */
void  SCL_EnableBlur(Uint32 Surface)
{
	SCL_SET_EXCCEN(0);
	SCL_SET_BOKEN(1);
	switch(Surface){
		case SCL_SPR:
			SCL_SET_BOKN(0);
			break;
		case SCL_RBG0:
			SCL_SET_BOKN(1);
			break;
		case SCL_NBG0:
		case SCL_RBG1:
			SCL_SET_BOKN(2);
			break;
		case SCL_NBG1:
		case SCL_EXBG:
			SCL_SET_BOKN(4);
			break;
		case SCL_NBG2:
			SCL_SET_BOKN(5);
			break;
		case SCL_NBG3:
			SCL_SET_BOKN(6);
			break;
		default:
			break;
	}
}

/* ボカシ演算を無効にする */
void  SCL_DisableBlur(void)
{
	SCL_SET_BOKEN(0);
}
