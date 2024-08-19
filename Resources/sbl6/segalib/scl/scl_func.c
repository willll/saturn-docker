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
 *
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h>
#include <sega_dma.h>
#include <sgl_work.h>



#define	DMAOFF

#define	REGADDR	0x25F80000

/* ���C���X�N���[���A�Z���X�N���[���֘A */
	Uint32	SclAddrLsTbl[2];
	Uint32	SclAddrCsTbl[2];
	Uint16	SclLengthLsTbl = 0;
	Uint16	SclLengthCsTbl = 0;

static	Uint16	paramode = 1;
static	Uint16	blank_time;
static	volatile Uint16	*regaddr;
	Uint32	SclCurSclNum =0;
	Uint16	SclProcess = 0;

/* ���ʃT�C�Y�f�t�H���g */
	Uint16	SclDisplayX = 320;
	Uint16	SclDisplayY = 224;

/* ���W�X�^�o�b�t�@ */
	SclSysreg	Scl_s_reg;
	SclDataset	Scl_d_reg;
	SclNorscl	Scl_n_reg;
	SclRotscl	Scl_r_reg;
	SclWinscl	Scl_w_reg;
	SclSblSgl	Scl_flag;

/* ���̃t�@�C�����Q�Ƃ��������ϐ��̐錾 */
extern	SclPriBuffDirtyFlags	SclPriBuffDirty;
extern	void	SCL_AutoExec(void);
extern	Uint32	SCL_GetColRamMode(void);
extern  void	SCL_PriorityInit(void);
extern	void	SCL_VblInit(void);


/*******************************************************
 *  ���]�}�g���b�N�X�p�����[�^�e�[�u���o�b�t�@         *
 *******************************************************/
 SclRotreg	_SclRotregBuff[2];
 SclRotreg	*SclRotregBuff = _SclRotregBuff;


/*******************************************************
 *  ���]�}�g���b�N�X�p�����[�^�W���e�[�u���o�b�t�@     *
 *******************************************************/
	Uint16	SclK_TableBuff[2][820];
	Uint32	SclK_TableNum[2];
	Uint16	SclK_TableFlag[2];


	Uint32	SclRbgKtbAddr[2];
	Sint32	SclRbgKtbOffset[2];
	Uint16	SclRotXySw[2];
	Uint8	SclRa,SclRb;
	Uint32	SclRotateTableAddress;
/* ���]�p�����[�^�p�p�x�f�[�^ */
	Fixed32	SclRotateXy[2];
	Fixed32	SclRotateZ[2];
	Fixed32	SclRotateDisp[2];
	Fixed32	SclRotateMoveZ[2];

	Uint16	SclRPMD;	/* ���]�p�����[�^���[�h */

	Uint16	SclRotateTableMode=0xff;

/* ���̃t�@�C���Œ��`�����֐� */
	void	SCL_ParametersInit(void);
	void	SCL_ScrollShow(void);
	void	SCL_PriIntProc(void);
	void	SCL_Memcpyw(void *dest,void *src,Uint32 tcnt);

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

	regaddr = (volatile Uint16 *)REGADDR;
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
 *------------------------------------------------------------------------
 */
void SCL_CopyReg()
{
	Uint16	i;

    /* ���]�p�����[�^�W���e�[�u�����u�q�`�l�ɓ]�� */
    if(SclK_TableFlag[0] && SclRbgKtbAddr[0])
    {
	SCL_Memcpyw((void *)SclRbgKtbAddr[0],SclK_TableBuff[0],SclK_TableNum[0]*2);
        SclK_TableFlag[0] = 0;
    }

    if(SclK_TableFlag[1] && SclRbgKtbAddr[1])
    {
	SCL_Memcpyw((void *)SclRbgKtbAddr[1],SclK_TableBuff[1],SclK_TableNum[1]*2);
        SclK_TableFlag[1] = 0;
    }

    switch(SclRotateTableMode)
    {	case 2:
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

    i = 0;
#ifdef	DMAOFF
    SCL_Memcpyw((void *)&regaddr[i], &Scl_s_reg, sizeof(SclSysreg));
    i += sizeof(SclSysreg) / 2;
    SCL_Memcpyw((void *)&regaddr[i], &Scl_d_reg, sizeof(SclDataset));
    i += sizeof(SclDataset) / 2;
    SCL_Memcpyw((void *)&regaddr[i], &Scl_n_reg, sizeof(SclNorscl));
    i += sizeof(SclNorscl) / 2;
    SCL_Memcpyw((void *)&regaddr[i], &Scl_r_reg, sizeof(SclRotscl));
    i += sizeof(SclRotscl) / 2;
    SCL_Memcpyw((void *)&regaddr[i], &Scl_w_reg, sizeof(SclWinscl));
    i += sizeof(SclWinscl) / 2;
#else
    DMA_ScuMemCopy(&regaddr[i], &Scl_s_reg, sizeof(SclSysreg) );
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


void SCL_Memcpyw(void *dest,void *src,Uint32 tcnt)
{
    Uint32 tcr,tsize;

    tsize=tcnt;
    tsize=tsize/2;

    for (tcr = 0;tcr < tsize;tcr++){
	*((Uint16 *)dest) = *((Uint16 *)src);
	dest = (Uint8 *)dest + 2;
	src = (Uint8 *)src + 2;
    }
}


/* �J���[ RAM �̃��[�h���ݒ肷�� */
void  SCL_SetColRamMode(Uint32 ComRamMode)
{
    switch(ComRamMode){
      case SCL_CRM15_1024:
	Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0x0FFF) | 0x0000;
	break;
      case SCL_CRM15_2048:
	Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0x0FFF) | 0x1000;
	break;
      case SCL_CRM24_1024:
	Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0x0FFF) | 0x2000;
	break;
    }
    if(SclProcess == 0)		SclProcess = 1;
}


/***************************************************************
 *	Priority Interrupt Routine
 *      ���̊֐���VB interval �̊��荞�݊֐������Ă΂���
 ***************************************************************/
void  SCL_PriIntProc(void)
{
    SCL_AutoExec();

    if(SclPriBuffDirty.SclOtherPri){
	/* SclOtherPri �̃��W�X�^�ւ̃R�s�[ */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealOtherPri, &SclOtherPri, sizeof(SclOtherPri));
#else
	DMA_ScuMemCopy(SclRealOtherPri, &SclOtherPri, sizeof(SclOtherPri));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclOtherPri = 0;
    }
    if(SclPriBuffDirty.SclSpPriNum){
	/* SclSpPriNum �̃��W�X�^�ւ̃R�s�[ */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealSpPriNum, &SclSpPriNum, sizeof(SclSpPriNum));
#else
	DMA_ScuMemCopy(SclRealSpPriNum, &SclSpPriNum, sizeof(SclSpPriNum));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclSpPriNum = 0;
    }
    if(SclPriBuffDirty.SclBgPriNum){
	/* SclBgPriNum �̃��W�X�^�ւ̃R�s�[ */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealBgPriNum, &SclBgPriNum, sizeof(SclBgPriNum));
#else
	DMA_ScuMemCopy(SclRealBgPriNum, &SclBgPriNum, sizeof(SclBgPriNum));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclBgPriNum = 0;
    }
    if(SclPriBuffDirty.SclSpColMix){
	/* SclSpColMix �̃��W�X�^�ւ̃R�s�[ */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealSpColMix, &SclSpColMix, sizeof(SclSpColMix));
#else
	DMA_ScuMemCopy(SclRealSpColMix, &SclSpColMix, sizeof(SclSpColMix));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclSpColMix = 0;
    }
    if(SclPriBuffDirty.SclBgColMix){
	/* SclBgColMix �̃��W�X�^�ւ̃R�s�[ */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealBgColMix, &SclBgColMix, sizeof(SclBgColMix));
#else
	DMA_ScuMemCopy(SclRealBgColMix, &SclBgColMix, sizeof(SclBgColMix));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclBgColMix = 0;
    }
    if(SclPriBuffDirty.SclColOffset){
	/* ColOffset�̃��W�X�^�ւ̃R�s�[ */
#ifdef	DMAOFF
	SCL_Memcpyw(SclRealColOffset, &SclColOffset, sizeof(SclColOffset));
#else
	DMA_ScuMemCopy(SclRealColOffset, &SclColOffset, sizeof(SclColOffset));
#endif	/* DMAOFF */
	SclPriBuffDirty.SclColOffset = 0;
    }

    return;
}

void SCL_SglOn(void){
	SclRotregBuff = ( SclRotreg * )RotScrParA;
	Scl_flag.sgl_flag=0x0001;
}

void SCL_SglOff(void){
	Scl_flag.sgl_flag=0x0000;
	SclRotregBuff = _SclRotregBuff;
}
