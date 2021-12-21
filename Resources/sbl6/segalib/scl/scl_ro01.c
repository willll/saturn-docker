/*------------------------------------------------------------------------
 *  FILE:	SCL_Ro01.c
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


void   SCL_SetRotateViewPoint(Uint16 x,Uint16 y,Uint16 z)
{
    Uint16	TbNum;

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

    SclRotregBuff[TbNum].viewp.x     = x;
    SclRotregBuff[TbNum].viewp.y     = y;
    SclRotregBuff[TbNum].viewp.z     = z;
}
