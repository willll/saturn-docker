/*------------------------------------------------------------------------
 *  FILE:	SCL_Ro10.c
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
 *	Written by K.M on 1994-06-30 Ver.1.00
 *	Updated by K.M on 1994-06-30 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 

extern	Uint32	SclCurSclNum;

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;

/*******************************************************
 *  回転マトリックスパラメータテーブルバッファ         *
 *******************************************************/
extern	SclRotreg	SclRotregBuff[2];

extern	Sint32	SclRbgKtbOffset[];
extern	Uint16	SclRotXySw[];

/* 回転パラメータ用角度データ */
extern	Fixed32	SclRotateXy[];
extern	Fixed32	SclRotateZ[];
extern	Fixed32	SclRotateDisp[];
extern	Fixed32	SclRotateMoveZ[];

extern	void   SCL_Rotate(Fixed32 xy,Fixed32 z,Fixed32 disp);

/* Scale */
void   SCL_SetRotateZoom(Fixed32 x,Fixed32 y)
{
	Uint16	TbNum;
	Sint32	xFlag,yFlag;

	switch(SclCurSclNum)
	{
		case SCL_RBG_TB_A:
			TbNum = 0;
			break;
		case SCL_RBG_TB_B:
			TbNum = 1;
			break;
		default:
			return;
			break;
	}

	if(SclRotregBuff[TbNum].zoom.x >= 0)	xFlag =  1;
	else					xFlag = -1;
	if(SclRotregBuff[TbNum].zoom.y >= 0)	yFlag =  1;
	else					yFlag = -1;

	SclRotregBuff[TbNum].zoom.x  -= x * xFlag;
	SclRotregBuff[TbNum].zoom.y  -= y * yFlag;

	if(    (SclRotregBuff[TbNum].zoom.x * xFlag) < FIXED(0)
		|| (SclRotregBuff[TbNum].zoom.x * xFlag) > FIXED(7)
		|| (SclRotregBuff[TbNum].zoom.y * yFlag) < FIXED(0)
		|| (SclRotregBuff[TbNum].zoom.y * yFlag) > FIXED(7) )
	{
		SclRotregBuff[TbNum].zoom.x += x * xFlag;
	    	SclRotregBuff[TbNum].zoom.y += y * yFlag;
	}

	if( (TbNum==0 && (Scl_r_reg.k_contrl & 0x00ff))
	     || (TbNum==1 && (Scl_r_reg.k_contrl & 0xff00)) )
	{
		SCL_Rotate(0,0,0);
	}
}
