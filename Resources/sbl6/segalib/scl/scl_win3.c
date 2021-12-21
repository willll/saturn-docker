/*------------------------------------------------------------------------
 *  FILE:	SCL_Win3.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	スプライトウインドウのセット
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

extern	void SCL_SetWindowSub(Uint32 surfaces,Uint32 enable,Uint8 *contrl,
				Uint8 on,Uint8 off);

void  SCL_SetSpriteWindow(Uint32 logic,Uint32 enable,Uint32 area)
{
    Uint8	*contrl;

    contrl = (Uint8 *)&Scl_w_reg.wincontrl[0];

    if(logic | enable)	SCL_SetWindowSub(logic,enable,contrl,0x80,0x7f);

    if(area | enable)	SCL_SetWindowSub(area,enable,contrl,0x10,0xef);

    SCL_SetWindowSub(enable,SCL_RP_R,contrl,0x20,0xdf);

    if(enable)
    {
	SCL_SET_SPWINEN(ON);
    } else {
	SCL_SET_SPWINEN(OFF);
    }

    if(SclProcess == 0)	SclProcess = 1;
}
