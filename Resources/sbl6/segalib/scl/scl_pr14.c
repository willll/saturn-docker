/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr14.c
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

static	void	SCL_SetLineColReg(Uint32 Surface,Uint16 sw)
{
	switch(Surface){
		case SCL_SPR:
			SCL_SET_SPLCEN(sw);
			break;
		case SCL_RBG0:
			SCL_SET_R0LCEN(sw);
			break;
		case SCL_NBG0:
		case SCL_RBG1:
			SCL_SET_N0LCEN(sw);
			break;
		case SCL_NBG1:
		case SCL_EXBG:
			SCL_SET_N1LCEN(sw);
			break;
		case SCL_NBG2:
			SCL_SET_N2LCEN(sw);
			break;
		case SCL_NBG3:
			SCL_SET_N3LCEN(sw);
			break;
		default:
			break;
	}
}


/* ラインカラー画面の挿入を設定する */
/* 実際に画面上に効果を得るには挿入設定した面をカラー演算する必要がある */
void  SCL_EnableLineCol(Uint32 Surface)
{
	SCL_SetLineColReg(Surface,1);
}

/* ラインカラー画面の挿入を解除する */
void  SCL_DisableLineCol(Uint32 Surface)
{
	SCL_SetLineColReg(Surface,0);
}
