/*------------------------------------------------------------------------
 *  FILE:	SCL_Ro03.c
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

extern	void	SCL_Rotate(Fixed32 xy,Fixed32 z,Fixed32 disp);
extern	void	SCL_RotateZD(Fixed32 dAngle);

/*------------------------------------------------------------------------
 *
 * NAME : SCL_RotateTo
 *
 * PARAMETERS
 *	param1 - XorY Angle
 *	param2 - Z Angle
 *	param3 - Display(Z) Angle
 *	param4 - X or Y Rotate ?
 *
 * DESCRIPTION
 *
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 *
 * CAVEATS
 * 
 *------------------------------------------------------------------------
 */
void   SCL_RotateTo(Fixed32 xy,Fixed32 z,Fixed32 disp,Uint16 mode)
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

    switch(mode)
    {
	case SCL_X_AXIS:
		SclRotXySw[TbNum] = 0;
		break;
	case SCL_Y_AXIS:
		if(SclRbgKtbOffset[TbNum]) {
			SclRotXySw[TbNum] = 0;
			return;
		}
		SclRotXySw[TbNum] = 1;
		break;
	default:
		return;
		break;
    }

    switch(SclCurSclNum)
    {
	case SCL_RBG_TB_A:
		Scl_r_reg.k_contrl &= 0xff00;
		break;
	case SCL_RBG_TB_B:
		Scl_r_reg.k_contrl &= 0x00ff;
		break;
    }

    SclRotateXy[TbNum]   = 0;
    SclRotateZ[TbNum]    = 0;
    SclRotateDisp[TbNum] = 0;

    if(!disp)	SCL_RotateZD(disp);

    SclRotregBuff[TbNum].matrix_a    = FIXED(1);
    SclRotregBuff[TbNum].matrix_b    = FIXED(0);
    SclRotregBuff[TbNum].matrix_c    = FIXED(0);
    SclRotregBuff[TbNum].matrix_d    = FIXED(0);
    SclRotregBuff[TbNum].matrix_e    = FIXED(1);
    SclRotregBuff[TbNum].matrix_f    = FIXED(0);

    SCL_Rotate(xy,z,disp);
}
