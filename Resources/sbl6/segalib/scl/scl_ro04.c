/*------------------------------------------------------------------------
 *  FILE:	SCL_Ro04.c
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
#define	DMAOFF

#include <sega_scl.h> 
#include <sega_dma.h> 

extern	void	SCL_Memcpyw(void *dest,void *src,Uint32 tcnt);

extern	Uint32	SclCurSclNum;

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;


/* 回転パラメータ用角度データ */
extern	Fixed32	SclRotateXy[];
extern	Fixed32	SclRotateZ[];
extern	Fixed32	SclRotateDisp[];
extern	Fixed32	SclRotateMoveZ[];

extern	Uint32	SclRbgKtbAddr[];

extern	Uint16	SclProcess;

/*******************************************************
 *  回転マトリックスパラメータテーブルバッファ         *
 *******************************************************/
extern	SclRotreg	SclRotregBuff[2];


void  SCL_SetCoefficientData(Uint32 sclnum,Uint16 *data,Uint16 x,Uint16 y)
{
    Uint16	SclProcessW;
    Uint16	TbNum;
    Fixed32	xw;

    SclProcessW = SclProcess;
    SclProcess  = 0;

    switch(sclnum)
    {
	case SCL_RBG_TB_A:
		TbNum = 0;
		Scl_r_reg.k_contrl &= 0xff00;
		Scl_r_reg.k_contrl |= 0x0003;
		break;
	case SCL_RBG_TB_B:
		TbNum = 1;
		Scl_r_reg.k_contrl &= 0x00ff;
		Scl_r_reg.k_contrl |= 0x0300;
		break;
	default:
		return;
		break;
    }

#ifdef	DMAOFF
    SCL_Memcpyw((void *)SclRbgKtbAddr[TbNum],data,x*y*2);
#else
    DMA_ScuMemCopy((void *)SclRbgKtbAddr[TbNum],data,x*y*2);
#endif	/* DMAOFF */

    SclRotregBuff[TbNum].matrix_a    = SclRotregBuff[TbNum].zoom.x;
    SclRotregBuff[TbNum].matrix_b    = FIXED(0);
    SclRotregBuff[TbNum].matrix_c    = FIXED(0);
    SclRotregBuff[TbNum].matrix_d    = FIXED(0);
    SclRotregBuff[TbNum].matrix_e    = SclRotregBuff[TbNum].zoom.y;
    SclRotregBuff[TbNum].matrix_f    = FIXED(0);

    xw = x;
    SclRotregBuff[TbNum].k_tab     = FIXED(0);
    SclRotregBuff[TbNum].k_delta.x = FIXED(xw);
    SclRotregBuff[TbNum].k_delta.y = FIXED(1);

    SclRotateXy[TbNum] = 0;

    SclProcess = SclProcessW;
    if(!SclProcess)	SclProcess = 1;
}
