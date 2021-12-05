/*------------------------------------------------------------------------
 *  FILE:	SCL_Func.c
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
 *	Written by K.M on 1994-06-21 Ver.1.00
 *	Updated by K.M on 1994-06-22 Ver.1.00
 *	Updated by Chikahiro Yoshida on 1995-07-19 Ver.1.00a
 *				詳細は SCL_CopyReg()のヘッダを参照
 *	Updated by N.K on 1996-03-19 Ver.1.00b
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 
#include <sega_dma.h> 
//#include	<sgl_work.h>
//#include	<sgl.h>

#define	DMAOFF

#define	REGADDR	0x25F80000
#define	SBL		0x00


/* ラインスクロール、セルスクロール関連 */
	Uint32	SclAddrLsTbl[2];
	Uint32	SclAddrCsTbl[2];
	Uint16	SclLengthLsTbl = 0;
	Uint16	SclLengthCsTbl = 0;

/*	static	Uint16	paramode = 1;	del: no used by C.yoshida	*/
/*	static	Uint16	blank_time;		del: no used by C.yoshida	*/
static	Uint16	*regaddr;
	Uint32	SclCurSclNum =0;
	Uint16	SclProcess = 0;

/* 画面サイズデフォルト */
	Uint16	SclDisplayX = 320;
	Uint16	SclDisplayY = 224;

/* レジスタバッファ */
	SclSysreg	Scl_s_reg;
	SclDataset	Scl_d_reg;
	SclNorscl	Scl_n_reg;
	SclRotscl	Scl_r_reg;
	SclWinscl	Scl_w_reg;
	//SclSblSgl	Scl_flag;

/* このファイルが参照する大域変数の宣言 */
extern	SclPriBuffDirtyFlags	SclPriBuffDirty;
extern	void	SCL_AutoExec(void);
extern	Uint32	SCL_GetColRamMode(void);
extern	void	SCL_VblInit(void);

extern	void	SCL_PriorityInit(void);	/*  add: C.yoshida  */

/*******************************************************
 *  回転マトリックスパラメータテーブルバッファ         *
 *******************************************************/

SclRotreg	_SclRotregBuff[2];
SclRotreg	*SclRotregBuff = _SclRotregBuff;

/*******************************************************
 *  回転マトリックスパラメータ係数テーブルバッファ     *
 *******************************************************/
	Uint16	SclK_TableBuff[2][820];
	Uint32	SclK_TableNum[2];
	Uint16	SclK_TableFlag[2];


	Uint32	SclRbgKtbAddr[2];
	Sint32	SclRbgKtbOffset[2];
	Uint16	SclRotXySw[2];
	Uint8	SclRa,SclRb;
	Uint32	SclRotateTableAddress;
/* 回転パラメータ用角度データ */
	Fixed32	SclRotateXy[2];
	Fixed32	SclRotateZ[2];
	Fixed32	SclRotateDisp[2];
	Fixed32	SclRotateMoveZ[2];

	Uint16	SclRPMD;	/* 回転パラメータモード */

#if	0
	Uint16	SclRotateTableMode=0xff;
#else
	Uint16	SclRotateTableMode=2;
#endif

/* このファイルで定義する関数 */
	void	SCL_ParametersInit(void);
	void	SCL_ScrollShow(void);
	void	SCL_PriIntProc(void);
	void	SCL_Memcpyw(void *dest,void *src,Uint32 tcnt);
	//void	SCL_SglOn(void);		/*	use SGL	*/
	//void	SCL_SglOff(void);		/*	use SGL	*/

extern	void   SCL_Rotate(Fixed32 xy,Fixed32 z,Fixed32 disp);


/*------------------------------------------------------------------------
 *
 * NAME : SCL_Vdp2Init
 *
 * PARAMETERS
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
void SCL_Vdp2Init(void)
{
/*    Uint32 i;		del:no used C.yoshida */

    SCL_VblInit();

    SclRotateXy[0]    = FIXED(0);
    SclRotateXy[1]    = FIXED(0);
    SclRotateZ[0]     = FIXED(0);
    SclRotateZ[1]     = FIXED(0);
    SclRotateMoveZ[0] = FIXED(0);
    SclRotateMoveZ[1] = FIXED(0);
    SclK_TableFlag[0] = 0;
    SclK_TableFlag[1] = 0;
    SclRotXySw[0]     = 0;
    SclRotXySw[1]     = 0;

    SclRbgKtbAddr[0] = 0;
    SclRbgKtbAddr[1] = 0;

    SclRbgKtbOffset[0] = 0;
    SclRbgKtbOffset[1] = 0;
	
	/*
	**追加 1995.11.06 C.Yoshida
	**SclRotateTableAddressはポインタだがSCL_RotateInitが呼ばれない限り
	**初期化されることはない。SCL_RotateInitをコールする前にV_Syncにくると
	**えらいことになる。
	*/
	SclRotateTableAddress = 0;
	
    SCL_PriorityInit();
    SCL_ParametersInit();
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_ParametersInit
 *
 * PARAMETERS
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
void SCL_ParametersInit(void)
{
	Uint16	i;

	regaddr = (Uint16 *)REGADDR;
/*
 *	System Registers Area Initialization
 */
	Scl_s_reg.tvmode = 0x8000; /* HREZ = B'001(352 pixels) the others = 0 */
	Scl_s_reg.extenbl = 0;
	Scl_s_reg.vramsize = 0;
	Scl_s_reg.vramchg = 0;
	Scl_s_reg.ramcontrl = 0x2000; /* color 16bit mode; VRAM A, B banked */
	for(i=0;i<8;i++)
		Scl_s_reg.vramcyc[i] = 0xffff;
	Scl_s_reg.dispenbl = 0;
	Scl_s_reg.mosaic = 0;
	Scl_s_reg.specialcode_sel = 0;
	Scl_s_reg.specialcode = 0;
/*
 *	Scroll DataSet Registers Area Initialization
 */
	Scl_d_reg.charcontrl0 = 0;
	Scl_d_reg.charcontrl1 = 0;
	Scl_d_reg.bmpalnum0 = 0;
	Scl_d_reg.bmpalnum1 = 0;
	Scl_d_reg.patnamecontrl[0] = 0x0000;
	Scl_d_reg.patnamecontrl[1] = 0;
	Scl_d_reg.patnamecontrl[2] = 0;
	Scl_d_reg.patnamecontrl[3] = 0;
	Scl_d_reg.patnamecontrl[4] = 0;
	Scl_d_reg.platesize = 0x0000;
	Scl_d_reg.mapoffset0 = 0;
	Scl_d_reg.mapoffset1 = 0;
	for(i=0;i<8;i++)	Scl_d_reg.normap[i] = 0;
	for(i=0;i<16;i++)	Scl_d_reg.rotmap[i] = 0;
/*
 *	Normal Scroll Registers Area Initialization
 */
	Scl_n_reg.n0_move_x = 0;
	Scl_n_reg.n0_move_y = 0;
	Scl_n_reg.n0_delta_x = FIXED(1);
	Scl_n_reg.n0_delta_y = FIXED(1);
	Scl_n_reg.n1_move_x = 0;
	Scl_n_reg.n1_move_y = 0;
	Scl_n_reg.n1_delta_x = FIXED(1);
	Scl_n_reg.n1_delta_y = FIXED(1);
	Scl_n_reg.n2_move_x = 0;
	Scl_n_reg.n2_move_y = 0;
	Scl_n_reg.n3_move_x = 0;
	Scl_n_reg.n3_move_y = 0;
	Scl_n_reg.zoomenbl = 0;
	Scl_n_reg.linecontrl = 0;
	Scl_n_reg.celladdr = 0;
	Scl_n_reg.lineaddr[0] = 0;
	Scl_n_reg.lineaddr[1] = 0;
	Scl_n_reg.linecolmode = 0;
	Scl_n_reg.backcolmode = 0;
	
/*
 *	Rotate Scroll Extension Registers Area Initialization
 */
	Scl_r_reg.paramode = 0;
	Scl_r_reg.paramcontrl = 0;
	Scl_r_reg.k_contrl = 0;
	Scl_r_reg.k_offset = 0;
	Scl_r_reg.mapover[0] = 0;
	Scl_r_reg.mapover[1] = 0;
	Scl_r_reg.paramaddr = 0;
/*
 *	Window Scroll Registers Area Initialization
 */
	Scl_w_reg.win0_start[0] = 0;
	Scl_w_reg.win0_start[1] = 0;
	Scl_w_reg.win0_end[0] = 0;
	Scl_w_reg.win0_end[1] = 0;
	Scl_w_reg.win1_start[0] = 0;
	Scl_w_reg.win1_start[1] = 0;
	Scl_w_reg.win1_end[0] = 0;
	Scl_w_reg.win1_end[1] = 0;
	Scl_w_reg.wincontrl[0] = 0;
	Scl_w_reg.wincontrl[1] = 0;
	Scl_w_reg.wincontrl[2] = 0;
	Scl_w_reg.wincontrl[3] = 0;
	Scl_w_reg.linewin0_addr = 0;
	Scl_w_reg.linewin1_addr = 0;

	if(SclProcess == 0)	SclProcess = 1;

}


/*----------------------------------------------------------------------
 *
 * NAME : SCL_Open
 *
 * PARAMETERS
 *	param1 -	Uint32 sclnum   :
 *
 * DESCRIPTION
 *			Open the certain scroll to ready for
 *			scroll operation
 * PRECONDITIONS
 *			None
 * POSTCONDITIONS
 * 			None
 * CAVEATS
 * 		
 *---------------------------------------------------------------------
 */
void SCL_Open(Uint32 sclnum)
{
	if(SclProcess == 1)	SclProcess = 0;
	SclCurSclNum = sclnum;
}


/*----------------------------------------------------------------------
 * NAME : SCL_Close
 *
 * PARAMETERS
 *			None
 * DESCRIPTION
 *			Close the certain scroll after scroll
 *			operation
 * PRECONDITIONS
 *			None
 * POSTCONDITIONS
 * 			None
 * CAVEATS
 * 		should be used as the pair of SCL_Open		
 *---------------------------------------------------------------------
 */
void SCL_Close(void)
{
	if(SclProcess == 0)
		SclProcess = 1;
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_MoveTo
 *
 * PARAMETERS
 *	param1 - x, y, z
 *	param2 -
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
void SCL_MoveTo(Fixed32 x,Fixed32 y,Fixed32 z)
{
	switch(SclCurSclNum) {
		case SCL_NBG0:
			Scl_n_reg.n0_move_x = x;
			Scl_n_reg.n0_move_y = y;
			break;
		case SCL_NBG1:
			Scl_n_reg.n1_move_x = x;
			Scl_n_reg.n1_move_y = y;
			break;
		case SCL_NBG2:
			Scl_n_reg.n2_move_x = x >> 16;
			Scl_n_reg.n2_move_y = y >> 16;
			break;
		case SCL_NBG3:
			Scl_n_reg.n3_move_x = x >> 16;
			Scl_n_reg.n3_move_y = y >> 16;
			break;
		case SCL_RBG_TB_A:
			SclRotregBuff[0].move.x = x;
			SclRotregBuff[0].move.y = y;
			if(SclRbgKtbAddr[0] && !(SclRbgKtbOffset[0] && SclRotateDisp[0]) )
				SclRotateMoveZ[0]   = z;
			else	SclRotateMoveZ[0]   = 0;
			if( /*(Scl_r_reg.k_contrl & 0x00ff) &&*/ SclRotateMoveZ[0])
			    SCL_Rotate(0,0,0);
			break;
		case SCL_RBG_TB_B:
			SclRotregBuff[1].move.x = x;
			SclRotregBuff[1].move.y = y;
			if(SclRbgKtbAddr[1] && !(SclRbgKtbOffset[1] && SclRotateDisp[1]) )
				SclRotateMoveZ[1]   = z;
			else	SclRotateMoveZ[1]   = 0;
			if( /*(Scl_r_reg.k_contrl & 0xff00) &&*/ SclRotateMoveZ[1] )
			    SCL_Rotate(0,0,0);
			break;
	}
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_Move
 *
 * PARAMETERS
 *	param1 - 	Fixed32	Mx:	X Movement
 *	param1 - 	Fixed32	My:	Y Movement
 *	param1 - 	Fixed32	Mz:	Z Movement
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
void SCL_Move(Fixed32 x,Fixed32 y,Fixed32 z)
{
	switch(SclCurSclNum) {
		case SCL_NBG0:
			Scl_n_reg.n0_move_x += x;
			Scl_n_reg.n0_move_y += y;
			break;
		case SCL_NBG1:
			Scl_n_reg.n1_move_x += x;
			Scl_n_reg.n1_move_y += y;
			break;
		case SCL_NBG2:
			Scl_n_reg.n2_move_x += x >> 16;
			Scl_n_reg.n2_move_y += y >> 16;
			break;
		case SCL_NBG3:
			Scl_n_reg.n3_move_x += x >> 16;
			Scl_n_reg.n3_move_y += y >> 16;
			break;
		case SCL_RBG_TB_A:
			SclRotregBuff[0].move.x += x;
			SclRotregBuff[0].move.y += y;
			if(SclRbgKtbAddr[0] && !(SclRbgKtbOffset[0] && SclRotateDisp[0]) )
				SclRotateMoveZ[0]   += z;
			else	SclRotateMoveZ[0]   = 0;
			if( /*(Scl_r_reg.k_contrl & 0x00ff) &&*/ SclRotateMoveZ[0] )
			    SCL_Rotate(0,0,0);
			break;
		case SCL_RBG_TB_B:
			SclRotregBuff[1].move.x += x;
			SclRotregBuff[1].move.y += y;
			if(SclRbgKtbAddr[1] && !(SclRbgKtbOffset[1] && SclRotateDisp[1]) )
				SclRotateMoveZ[1]   += z;
			else	SclRotateMoveZ[1]   = 0;
			if( /*(Scl_r_reg.k_contrl & 0xff00) &&*/ SclRotateMoveZ[1] )
			    SCL_Rotate(0,0,0);
			break;
	}
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_Scale
 *
 * PARAMETERS
 *	param1 - 	Fixed32	Sx:	X scale ratio
 *	param2 - 	Fixed32	Sy:	Y scale ratio
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
void SCL_Scale(Fixed32 Sx, Fixed32 Sy)
{
    Fixed32  wSx,wSy;

    wSx=Sx;
    wSy=Sy;
    switch(SclCurSclNum) {
	case SCL_NBG0:
	case SCL_NBG1:
	    if(wSx > FIXED(255))	wSx=FIXED(255);
	    else if(wSx < FIXED(0.25))	wSx=FIXED(0.25);
	    else if(wSx == FIXED(0))	wSx=FIXED(1);
	    if(wSy > FIXED(255))	wSy=FIXED(255);
	    else if(wSy < FIXED(0.25))	wSy=FIXED(0.25);
	    else if(wSy == FIXED(0))	wSy=FIXED(1);
	    wSx = DIV_FIXED(FIXED(1),wSx);
	    wSy = DIV_FIXED(FIXED(1),wSy);
	    break;
	case SCL_RBG_TB_A:
	case SCL_RBG_TB_B:
	    wSx = DIV_FIXED(FIXED(1),wSx);
	    wSy = DIV_FIXED(FIXED(1),wSy);
	    break;
   }

    switch(SclCurSclNum) {
	case SCL_NBG0:
	    Scl_n_reg.n0_delta_x = wSx;
	    Scl_n_reg.n0_delta_y = wSy;
	    break;
	case SCL_NBG1:
	    Scl_n_reg.n1_delta_x = wSx;
	    Scl_n_reg.n1_delta_y = wSy;
	    break;
	case SCL_RBG_TB_A:
	    SclRotregBuff[0].zoom.x = wSx;
	    SclRotregBuff[0].zoom.y = wSy;
	    if( Scl_r_reg.k_contrl & 0x00ff )
		SCL_Rotate(0,0,0);
	    break;
	case SCL_RBG_TB_B:
	    SclRotregBuff[1].zoom.x = wSx;
	    SclRotregBuff[1].zoom.y = wSy;
	    if( Scl_r_reg.k_contrl & 0xff00 )
		SCL_Rotate(0,0,0);
	    break;
   }
}

/*------------------------------------------------------------------------
 *
 * NAME : SCL_CopyReg
 *
 * PARAMETERS
 *	param1 - 
 *	param2 -
 *
 * DESCRIPTION
 *		Copy parameters to scroll H/W Registers
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 * 
 * CAVEATS
 * 		used by interrupt routine only(V_BLANK)
 *		***DON'T CALL IT DIRECTLY!***
 *
 * Updated by Chikahiro Yoshida 1995-07-19
 *	修正箇所	Read onlyのVDP2レジスタに対し、書き込みして
 *				いる処理を修正した。
 *
 *------------------------------------------------------------------------
 */
void SCL_CopyReg()
{
	Uint16	i;

			/* 回転パラメータ係数テーブルをＶＲＡＭに転送 */
	if(SclK_TableFlag[0] && SclRbgKtbAddr[0])	{
		SCL_Memcpyw((void *)SclRbgKtbAddr[0],SclK_TableBuff[0],SclK_TableNum[0]*2);
		SclK_TableFlag[0] = 0;
	}
		if(SclK_TableFlag[1] && SclRbgKtbAddr[1])	{
		SCL_Memcpyw((void *)SclRbgKtbAddr[1],SclK_TableBuff[1],SclK_TableNum[1]*2);
		SclK_TableFlag[1] = 0;
	}
#if	0
	switch(SclRotateTableMode)	{
	case 2:
		SCL_Memcpyw((void *)SclRotateTableAddress,&SclRotregBuff[0],sizeof(SclRotregBuff[0])*2);
		/*SCL_Memcpyw((void *)(SclRotateTableAddress+sizeof(SclRotregBuff[0])),
		&SclRotregBuff[1],sizeof(SclRotregBuff[1]));*/
		break;
	case 1:
		SCL_Memcpyw((void *)SclRotateTableAddress,&SclRotregBuff[0],sizeof(SclRotregBuff[0]));
		break;
	default:
		break;
	}
#else
	{
		const	Uint32	size=0x60;	/*	sizeof(struct SclRotreg);	*/
		const	Uint32	p=(Uint32)SclRotateTableAddress;
		void	*const	pA=(void *)p;
		void	*const	pB=(void *)(p+0x80);
		
		/*
		**  追加 1996.03.19 N.K
		**  SclRotateTableAddress の値の検査を追加。
		*/
		if (pA != NULL) {
		    SCL_Memcpyw(pA,&SclRotregBuff[0],size);
		    SCL_Memcpyw(pB,&SclRotregBuff[1],size);
		}
	}
#endif

    i = 0;
#ifdef	DMAOFF
/*  SCL_Memcpyw(&regaddr[i], &Scl_s_reg, sizeof(SclSysreg)); del by C.Y	*/

	regaddr[0] = Scl_s_reg.tvmode;		/* add				by C.Y	*/
	regaddr[1] = Scl_s_reg.extenbl;		/* add				by C.Y	*/
/*	regaddr[2]							del read only reg	by C.Y	*/
	regaddr[3] = Scl_s_reg.vramsize;		/* add				by C.Y	*/
/*	regaddr[4]							del read only reg	by C.Y	*/
/*	regaddr[5]							del read only reg	by C.Y	*/
/*	regaddr[6]							del reserve			by C.Y	*/

	SCL_Memcpyw( &regaddr[7] , &Scl_s_reg.ramcontrl , 13*2 );

    i += sizeof(SclSysreg) / 2;
    SCL_Memcpyw(&regaddr[i], &Scl_d_reg, sizeof(SclDataset));
    i += sizeof(SclDataset) / 2;
    SCL_Memcpyw(&regaddr[i], &Scl_n_reg, sizeof(SclNorscl));
    i += sizeof(SclNorscl) / 2;
    SCL_Memcpyw(&regaddr[i], &Scl_r_reg, sizeof(SclRotscl));
    i += sizeof(SclRotscl) / 2;
    SCL_Memcpyw(&regaddr[i], &Scl_w_reg, sizeof(SclWinscl));
    i += sizeof(SclWinscl) / 2;
#else
/*   DMA_ScuMemCopy(&regaddr[i], &Scl_s_reg, sizeof(SclSysreg) ); by C.Y */

	regaddr[0] = Scl_s_reg.tvmode;		/*	add		by C.Y	*/
	regaddr[1] = Scl_s_reg.extenbl;		/*	add		by C.Y	*/
/*	regaddr[2]							del: read only by C.Y	*/
	regaddr[3] = Scl_s_reg.vramsize;
/*	regaddr[4]							del: read only by C.Y	*/
/*	regaddr[5]							del: read only by C.Y	*/
/*	regaddr[6]							del: reserve	by C.Y	*/
	SCL_Memcpyw( &regaddr[7] , &Scl_s_reg.ramcontrl , 13*2 );

    i += sizeof(SclSysreg) / 2;
    DMA_ScuMemCopy(&regaddr[i], &Scl_d_reg, sizeof(SclDataset) );
    i += sizeof(SclDataset) / 2;
    DMA_ScuMemCopy(&regaddr[i], &Scl_n_reg, sizeof(SclNorscl) );
    i += sizeof(SclNorscl) / 2;
    DMA_ScuMemCopy(&regaddr[i], &Scl_r_reg, sizeof(SclRotscl) );
    i += sizeof(SclRotscl) / 2;
    DMA_ScuMemCopy(&regaddr[i], &Scl_w_reg, sizeof(SclWinscl) );
    i += sizeof(SclWinscl) / 2;
#endif	/*DMAOFF*/
}

/*
 *		Scroll V_BLANK processing routine
 */
void	SCL_ScrollShow(void)
{

    switch(SclProcess){
	    case 1:
		SCL_CopyReg();
		SclProcess = 0;
		break;
	    case 2:			/* line scroll setting */
#ifdef	DMAOFF
		SCL_Memcpyw((void *)SclAddrLsTbl[0],(void *)SclAddrLsTbl[1]
			, SclLengthLsTbl * 4);
		SCL_Memcpyw((void *)SclAddrCsTbl[0],(void *)SclAddrCsTbl[1]
			, SclLengthCsTbl * 4);
#else
		DMA_ScuMemCopy((void *)SclAddrLsTbl[0], (void *)SclAddrLsTbl[1], SclLengthLsTbl * 4 * 3);
		DMA_ScuMemCopy((void *)SclAddrCsTbl[0], (void *)SclAddrCsTbl[1], SclLengthCsTbl * 4);
#endif	/*DMAOFF*/
		SCL_CopyReg();
		SclProcess = 0;
		break;
	    case 3:
		break;
	    case 4:
		break;
	    case 5:			/* Rotate Scroll */
		break;
	    case 6:			/* line window */
		break;
	    case 7:			/* line window */
		break;
    }

    SCL_PriIntProc();
}


void SCL_Memcpyw(void *dest,void *src,Uint32 tcnt)	{

	if((dest != NULL)&&(src != NULL))	{

		Uint32 tcr,tsize;
		tsize=tcnt;
		tsize=tsize/2;

		for (tcr = 0;tcr < tsize;tcr++){
			*((Uint16 *)dest) = *((Uint16 *)src);
			dest = (Uint8 *)dest + 2;
			src = (Uint8 *)src + 2;
		}
	}
}


/* カラー RAM のモードを設定する */
void  SCL_SetColRamMode(Uint32 ComRamMode)
{
    switch(ComRamMode){
      case SCL_CRM15_1024:
/*	Scl_s_reg.ramcontrl = Scl_s_reg.ramcontrl & 0x0FFF | 0x0000;	*/
	Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0x0FFF) | 0x0000;
							/* add:() by C.yoshida */
	break;
      case SCL_CRM15_2048:
/*	Scl_s_reg.ramcontrl = Scl_s_reg.ramcontrl & 0x0FFF | 0x1000;	*/
	Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0x0FFF) | 0x1000;
							/* add:() by C.yoshida */
	break;
      case SCL_CRM24_1024:
/*	Scl_s_reg.ramcontrl = Scl_s_reg.ramcontrl & 0x0FFF | 0x2000;	*/
	Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0x0FFF) | 0x2000;
							/* add:() by C.yoshida */
	break;
    }
    if(SclProcess == 0)		SclProcess = 1;
}


/***************************************************************
 *	Priority Interrupt Routine 
 *      この関数はVB interval の割り込み関数から呼ばれる 
 ***************************************************************/
void  SCL_PriIntProc(void)
{
/*    Uint8 i;		del:no used by C.yoshida */

    SCL_AutoExec();

    if(SclPriBuffDirty.SclOtherPri){
	/* SclOtherPri のレジスタへのコピー */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealOtherPri, &SclOtherPri, sizeof(SclOtherPri));
#else
	DMA_ScuMemCopy(SclRealOtherPri, &SclOtherPri, sizeof(SclOtherPri));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclOtherPri = 0;
    }
    if(SclPriBuffDirty.SclSpPriNum){
	/* SclSpPriNum のレジスタへのコピー */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealSpPriNum, &SclSpPriNum, sizeof(SclSpPriNum));
#else
	DMA_ScuMemCopy(SclRealSpPriNum, &SclSpPriNum, sizeof(SclSpPriNum));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclSpPriNum = 0;
    }
    if(SclPriBuffDirty.SclBgPriNum){
	/* SclBgPriNum のレジスタへのコピー */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealBgPriNum, &SclBgPriNum, sizeof(SclBgPriNum));
#else
	DMA_ScuMemCopy(SclRealBgPriNum, &SclBgPriNum, sizeof(SclBgPriNum));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclBgPriNum = 0;
    }
    if(SclPriBuffDirty.SclSpColMix){
	/* SclSpColMix のレジスタへのコピー */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealSpColMix, &SclSpColMix, sizeof(SclSpColMix));
#else
	DMA_ScuMemCopy(SclRealSpColMix, &SclSpColMix, sizeof(SclSpColMix));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclSpColMix = 0;
    }
    if(SclPriBuffDirty.SclBgColMix){
	/* SclBgColMix のレジスタへのコピー */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealBgColMix, &SclBgColMix, sizeof(SclBgColMix));
#else
	DMA_ScuMemCopy(SclRealBgColMix, &SclBgColMix, sizeof(SclBgColMix));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclBgColMix = 0;
    }
    if(SclPriBuffDirty.SclColOffset){
	/* ColOffsetのレジスタへのコピー */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealColOffset, &SclColOffset, sizeof(SclColOffset));
#else
	DMA_ScuMemCopy(SclRealColOffset, &SclColOffset, sizeof(SclColOffset));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclColOffset = 0;
    }

    return;
}


/*------------------------------------------------------------------------
 * NAME : SCL_Vdp2_SGLInit
 * ＳＧＬモードのデフォルトにする
 *----------------------------------------------------------------------*/
static Uint16  tvsize[16] = { 0x0000,   	/* SPR_TV_320X224  */
                              0x0010,       /* SPR_TV_320X240  */
                              0x0001,       /* SPR_TV_352X224  */
                              0x0011,       /* SPR_TV_352X240  */
                              0x0002,       /* SPR_TV_640X224  */
                              0x0012,       /* SPR_TV_640X240  */
                              0x0003,       /* SPR_TV_704X224  */
                              0x0013,       /* SPR_TV_704X240  */
                              0x00c0,       /* SPR_TV_320X448  */
                              0x00d0,       /* SPR_TV_320X480  */
                              0x00c1,       /* SPR_TV_352X448  */
                              0x00d1,       /* SPR_TV_352X480  */
                              0x00c2,       /* SPR_TV_640X448  */
                              0x00d2,       /* SPR_TV_640X480  */
                              0x00c3,       /* SPR_TV_704X448  */
                              0x00d3        /* SPR_TV_704X480  */
                            };

void SCL_Vdp2_SGLInit(Uint16 tvmod)
{
	/* slInitSystem のtvmode から Scl_s_reg.tvmode を再セット	*/
	tvmod &= 0x0f ;						/* TVmode = 0 ～ 15 */
	Scl_s_reg.tvmode |= tvsize[tvmod];
	if(SclProcess == 0)	SclProcess = 1;


	/* スプライトカラー演算割合	*/
    SCL_SET_S0CCRT(8);
    SCL_SET_S1CCRT(8);
    SCL_SET_S2CCRT(8);
    SCL_SET_S3CCRT(8);
    SCL_SET_S4CCRT(8);
    SCL_SET_S5CCRT(8);
    SCL_SET_S6CCRT(8);
    SCL_SET_S5CCRT(8);
    SclPriBuffDirty.SclSpColMix = 1;

	/* スプライトプライオリティ	*/
    SCL_SET_S0PRIN(7); 
    SCL_SET_S1PRIN(7); 
    SCL_SET_S2PRIN(7); 
    SCL_SET_S3PRIN(7); 
    SCL_SET_S4PRIN(7); 
    SCL_SET_S5PRIN(7); 
    SCL_SET_S6PRIN(7); 
    SCL_SET_S7PRIN(7); 
    SclPriBuffDirty.SclSpPriNum = 1;

	/* スプライトコントロール	*/
	SCL_SET_SPTYPE(3);
	SCL_SET_SPCLMD(1);
	SCL_SET_SPWINEN(0);
	SCL_SET_SPCCCS(3);
	SCL_SET_SPCCN(0);

}

/*void SCL_SglOn(void){
	SclRotregBuff = ( SclRotreg * )RotScrParA;
	Scl_flag.sgl_flag=0x0001;
}*/

/*void SCL_SglOff(void){
	Scl_flag.sgl_flag=0x0000;
	SclRotregBuff = _SclRotregBuff;
}*/

