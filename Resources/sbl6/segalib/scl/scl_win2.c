/*------------------------------------------------------------------------
 *  FILE:	SCL_Win2.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	ノーマルラインウインドウのセット
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

void  SCL_SetLineWindow(Uint8 win,Uint32 logic,Uint32 enable,Uint32 area,
		Uint32 addr,Uint32 sy,Uint32 tbSize,SclLineWindowTb *tp)
{
    Uint16	*sxy,*exy;
    Uint8	*contrl;
    Uint8	en_on,en_off;
    Uint8	ar_on,ar_off;
    Uint16	i,*addr16;

    switch(win)
    {
	case SCL_W0:
		Scl_w_reg.linewin0_addr = 0x80000000 | ((addr-SCL_VDP2_VRAM)/2 & 0x0007fffe);
		sxy = &Scl_w_reg.win0_start[0];
		exy = &Scl_w_reg.win0_end[0];
		en_on  = 0x02;
		en_off = 0xfd;
		ar_on  = 0x01;
		ar_off = 0xfe;
		break;
	case SCL_W1:
		Scl_w_reg.linewin1_addr = 0x80000000 | ((addr-SCL_VDP2_VRAM)/2 & 0x0007fffe);
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

    sxy[1] = (Uint16 )sy;
    exy[1] = (Uint16 )(sy+tbSize-1);

    contrl = (Uint8 *)&Scl_w_reg.wincontrl[0];

    /* VRAM へのデータ転送 */
    if(tbSize)
    {
	addr16 = (Uint16 *)(addr+sy*4);
	if(SclDisplayX > 352)
	{	/* ハイレゾ */
		for(i=0;i<tbSize;i++)
		{
			*addr16 = tp->start;
			addr16++;
			*addr16 = tp->end;
			addr16++;
			tp++;
		}
	}else{
	        /* ノーマル */
		for(i=0;i<tbSize;i++)
		{
			*addr16 = tp->start*2;
			addr16++;
			*addr16 = tp->end*2;
			addr16++;
			tp++;
		}
	}
    }

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
