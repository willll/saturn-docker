/*------------------------------------------------------------------------
 *  FILE:	SCL_Win1.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	ノーマル矩形ウインドウのセット
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

void	SCL_SetWindow(Uint8 win,Uint32 logic,Uint32 enable,Uint32 area,
		Uint16 sx,Uint16 sy,Uint16 ex,Uint16 ey)
{
    Uint16	*sxy,*exy;
    Uint8	*contrl;
    Uint8	en_on,en_off;
    Uint8	ar_on,ar_off;

    switch(win)
    {
	case SCL_W0:
		Scl_w_reg.linewin0_addr = 0;
		sxy = &Scl_w_reg.win0_start[0];
		exy = &Scl_w_reg.win0_end[0];
		en_on  = 0x02;
		en_off = 0xfd;
		ar_on  = 0x01;
		ar_off = 0xfe;
		break;
	case SCL_W1:
		Scl_w_reg.linewin1_addr = 0;
		sxy = &Scl_w_reg.win1_start[0];
		exy = &Scl_w_reg.win1_end[0];
		en_on  = 0x08;
		en_off = 0xf7;
		ar_on  = 0x04;
		ar_off = 0xfb;
		break;
	default:
		return;
		break;
    }

    contrl = (Uint8 *)&Scl_w_reg.wincontrl[0];

    if(SclDisplayX > 352)
    {	/* ハイレゾ */
	sxy[0] = sx;
	exy[0] = ex;
    }else{
        /* ノーマル */
	sxy[0] = sx*2;
	exy[0] = ex*2;
    }
    sxy[1] = sy;
    exy[1] = ey;

    if(logic | enable)	SCL_SetWindowSub(logic,enable,contrl,0x80,0x7f);

    if(area | enable)	SCL_SetWindowSub(area,enable,contrl,ar_on,ar_off);

    SCL_SetWindowSub(enable,0xffffffff,contrl,en_on,en_off);
    if(enable & SCL_RP)
    {
	Scl_r_reg.paramode = 3;
    } else {
	Scl_r_reg.paramode = SclRPMD;
    }

    if(SclProcess == 0)	SclProcess = 1;
}
