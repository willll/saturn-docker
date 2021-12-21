/*----------------------------------------------------------------------------
 *  FILE: SCL_Pri.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *         Priority Library Functions 
 *
 *  AUTHOR(S):
 *		K.M
 *
 *  MOD HISTORY:
 *      $Log$
 *
 *----------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stddef.h>

#define SEGA_SCL_PROTO

#include <sega_scl.h> 

extern	void	SCL_SetColRam(Uint32 Object, Uint32 Index,Uint32 num,void *Color);

/* scroll library's registers */
extern	SclSysreg	Scl_s_reg;

static	void	SCL_InitSclOtherPri(void);
static	void	SCL_InitSclSpPriNum(void);
static	void	SCL_InitSclBgPriNum(void);
static	void	SCL_InitSclSpColMix(void);
static	void	SCL_InitSclBgColMix(void);
static	void	SCL_InitSclColOffset(void);

	void	SCL_FreeColRam(Uint32 Surface);
	void	SCL_SetColRamOffset(Uint32 Object,Uint32 Offset,Uint8 transparent);
	Uint32  SCL_GetColRamOffset(Uint32 object);
	Uint32  SCL_GetColRamMode(void);
	void	SCL_SetAutoColMix(Uint32 Surfaces,Uint32 Interval,Uint32 Time,
			Uint8 StartRate,Uint8 EndRate);
extern	void	SCL_IncColOffset(Uint32 OffsetReg,Sint16 red,Sint16 green,Sint16 blue);


/*--- Priority Register Buffers ---*/
	SclOtherPriRegister  SclOtherPri;  /* いろいろ */
	SclSpPriNumRegister  SclSpPriNum;  /* スプライトのプライオリティ番号 */
	SclBgPriNumRegister  SclBgPriNum;  /* スクロールのプライオリティ番号 */
	SclSpColMixRegister  SclSpColMix;  /* スプライトの色演算 */
	SclBgColMixRegister  SclBgColMix;  /* スクロールの色演算 */
	SclColOffsetRegister SclColOffset; /* カラーオフセット */
	SclPriBuffDirtyFlags SclPriBuffDirty; /* レジスタバッファのダーティー・フラグ */


/*--- REAL Priority Registers (Pointer) ---*/
	SclOtherPriRegister  *SclRealOtherPri = (SclOtherPriRegister *)0x25F800E0;
	SclSpPriNumRegister  *SclRealSpPriNum = (SclSpPriNumRegister *)0x25F800F0;
	SclBgPriNumRegister  *SclRealBgPriNum = (SclBgPriNumRegister *)0x25F800F8;
	SclSpColMixRegister  *SclRealSpColMix = (SclSpColMixRegister *)0x25F80100;
	SclBgColMixRegister  *SclRealBgColMix = (SclBgColMixRegister *)0x25F80108;
	SclColOffsetRegister *SclRealColOffset= (SclColOffsetRegister *)0x25F80110;

	Uint32	SclLineColRamOffset;

	Uint32	SclColRamAlloc256[8];
	Uint32	SclColRamAlloc2048[1];

	Uint32	SclAutoSurface;
	Uint32	SclAutoInterval;
	Uint32	SclAutoCount;
	Uint32	SclAutoIndex;
	Uint32	SclAutoMax;
	Sint16	SclAutoRed,SclAutoGreen,SclAutoBlue;
	Sint16	SclAutoEndRed,SclAutoEndGreen,SclAutoEndBlue;
	Sint16	SclAutoEndRed2,SclAutoEndGreen2,SclAutoEndBlue2;
	Uint8	SclAutoCurRate;
	Uint8	SclAutoEndRate;
	Uint8	SclAutoVeFlag;
	Uint8	SclAutoPlusFlag;
	Uint8	SclAutoRGB[6][256];

/* プライオリティライブラリの初期化 */
void  SCL_PriorityInit(void)
{
    Uint8	i;

    /* カラー RAM モード(24 or 16)はスクロールライブラリが設定する */
    SCL_InitSclOtherPri();
    SCL_InitSclSpPriNum();
    SCL_InitSclBgPriNum();
    SCL_InitSclSpColMix();
    SCL_InitSclBgColMix();
    SCL_InitSclColOffset();

    for(i=0;i<8;i++)
    {
    	SclColRamAlloc256[i]=0;
    }
    SclColRamAlloc2048[0]=0;
    SclAutoVeFlag = 0;
}

/* カラー RAM のモードを得る */
Uint32  SCL_GetColRamMode(void)
{
    return((Uint32)((Scl_s_reg.ramcontrl & 0x3000) >> 12));
}


void  SCL_AbortAutoVe(void)
{
	SclAutoVeFlag = 0;
}


/* カラー演算の割合の設定 */
void  SCL_SetColMixRate(Uint32 Surfaces, Uint8 Rate)
{
	SCL_SET_CCRTMD(0);
	SCL_SET_CCMD(0);
	if(Surfaces & SCL_SP0){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S0CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP1){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S1CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP2){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S2CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP3){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S3CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP4){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S4CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP5){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S5CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP6){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S6CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if(Surfaces & SCL_SP7){
		if(Rate < 32) {
			SCL_SET_SPCCEN(1);
			SCL_SET_S7CCRT(Rate);
		}else{
			SCL_SET_SPCCEN(0);
		}
	}
	if( (Surfaces & SCL_NBG0) || (Surfaces & SCL_RBG1)){
		if(Rate < 32) {
			SCL_SET_N0CCEN(1);
			SCL_SET_N0CCRT(Rate);
		}else{
			SCL_SET_N0CCEN(0);
		}
	}
	if( (Surfaces & SCL_NBG1) || (Surfaces & SCL_EXBG) ){
		if(Rate < 32) {
			SCL_SET_N1CCEN(1);
			SCL_SET_N1CCRT(Rate);
		}else{
			SCL_SET_N1CCEN(0);
		}
	    }
	if(Surfaces & SCL_NBG2){
		if(Rate < 32) {
			SCL_SET_N2CCEN(1);
			SCL_SET_N2CCRT(Rate);
		}else{
			SCL_SET_N2CCEN(0);
		}
	}
	if(Surfaces & SCL_NBG3){
		if(Rate < 32) {
			SCL_SET_N3CCEN(1);
			SCL_SET_N3CCRT(Rate);
		}else{
			SCL_SET_N3CCEN(0);
		}
	}
	if(Surfaces & SCL_RBG0){
		if(Rate < 32) {
			SCL_SET_R0CCEN(1);
			SCL_SET_R0CCRT(Rate);
		}else{
			SCL_SET_R0CCEN(0);
		}
	}
}


void SCL_AutoExec(void)
{
    Uint32 ColorL[1];
    Uint8  *ColorB;
    Uint32 i;

    ColorB = (Uint8 *)ColorL;

    switch(SclAutoVeFlag)
    {	case 1:
	    if(SclAutoInterval > SclAutoCount) SclAutoCount++;
	    else
	    {
		SclAutoVeFlag=0;
		for(i=0;i<SclAutoMax;i++)
		{
		    /* Blue */
		    if(SclAutoRGB[0][i] > SclAutoRGB[3][i])
		    {  SclAutoRGB[3][i]++;
		       SclAutoVeFlag=1;
		    } else if(SclAutoRGB[0][i] < SclAutoRGB[3][i])
		    {  SclAutoRGB[3][i]--;
		       SclAutoVeFlag=1;
		    }
		    /* Green */
		    if(SclAutoRGB[1][i] > SclAutoRGB[4][i])
		    {  SclAutoRGB[4][i]++;
		       SclAutoVeFlag=1;
		    } else if(SclAutoRGB[1][i] < SclAutoRGB[4][i])
		    {  SclAutoRGB[4][i]--;
		       SclAutoVeFlag=1;
		    }
		    /* Red */
		    if(SclAutoRGB[2][i] > SclAutoRGB[5][i])
		    {  SclAutoRGB[5][i]++;
		       SclAutoVeFlag=1;
		    } else if(SclAutoRGB[2][i] < SclAutoRGB[5][i])
		    {  SclAutoRGB[5][i]--;
		       SclAutoVeFlag=1;
		    }
		}
		SclAutoCount = 0;
		if(SCL_GetColRamMode()==SCL_CRM24_1024) {
		    for(i=0;i<SclAutoMax;i++) {
			ColorB[0]=0;
			ColorB[1]=SclAutoRGB[3][i];
			ColorB[2]=SclAutoRGB[4][i];
			ColorB[3]=SclAutoRGB[5][i];
			SCL_SetColRam(SclAutoSurface,SclAutoIndex+i,1,&ColorL[0]);
		    }
		}else{
		    for(i=0;i<SclAutoMax;i++) {
			ColorL[0] = SclAutoRGB[3][i];
			ColorL[0] = (ColorL[0] << 5) | SclAutoRGB[4][i];
			ColorL[0] = (ColorL[0] << 5) | SclAutoRGB[5][i];
		        SCL_SetColRam(SclAutoSurface,SclAutoIndex+i,1,&ColorL[0]);
		    }
		}
	    }
	    break;
    	case 2:
	    if(SclAutoInterval > SclAutoCount) SclAutoCount++;
	    else
	    {	if(SclAutoPlusFlag)
	    	{   SclAutoCurRate++;
		    if(SclAutoCurRate>=SclAutoEndRate)
		    {	SclAutoCurRate=SclAutoEndRate;
		    	SclAutoVeFlag=0;
		    }
		}
		else
	    	{   SclAutoCurRate--;
		    if(SclAutoCurRate<=SclAutoEndRate)
		    {	SclAutoCurRate=SclAutoEndRate;
		    	SclAutoVeFlag=0;
		    }
		}
		if(SCL_SP0 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP0,SclAutoCurRate);
		if(SCL_SP1 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP1,SclAutoCurRate);
		if(SCL_SP2 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP2,SclAutoCurRate);
		if(SCL_SP3 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP3,SclAutoCurRate);
		if(SCL_SP4 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP4,SclAutoCurRate);
		if(SCL_SP5 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP5,SclAutoCurRate);
		if(SCL_SP6 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP6,SclAutoCurRate);
		if(SCL_SP7 & SclAutoSurface)  SCL_SetColMixRate(SCL_SP7,SclAutoCurRate);
		if(SCL_NBG0 & SclAutoSurface) SCL_SetColMixRate(SCL_NBG0,SclAutoCurRate);
		if(SCL_NBG1 & SclAutoSurface) SCL_SetColMixRate(SCL_NBG1,SclAutoCurRate);
		if(SCL_NBG2 & SclAutoSurface) SCL_SetColMixRate(SCL_NBG2,SclAutoCurRate);
		if(SCL_NBG3 & SclAutoSurface) SCL_SetColMixRate(SCL_NBG3,SclAutoCurRate);
		if(SCL_RBG0 & SclAutoSurface) SCL_SetColMixRate(SCL_RBG0,SclAutoCurRate);
		if(SCL_RBG1 & SclAutoSurface) SCL_SetColMixRate(SCL_RBG1,SclAutoCurRate);
		if(SCL_EXBG & SclAutoSurface) SCL_SetColMixRate(SCL_EXBG,SclAutoCurRate);
		SclAutoCount = 0;
	    }
	    break;
    	case 3:
	    if(SclAutoInterval > SclAutoCount) SclAutoCount++;
	    else
	    {
		SclAutoEndRed -= SclAutoRed;
		SclAutoEndGreen -= SclAutoGreen;
		SclAutoEndBlue -= SclAutoBlue;
		if(SclAutoRed > 0)
		{  if(SclAutoEndRed <= 0) SclAutoRed = 0;}
		else
		{  if(SclAutoEndRed >= 0) SclAutoRed = 0;}

		if(SclAutoGreen > 0)
		{  if(SclAutoEndGreen <= 0) SclAutoGreen = 0;}
		else
		{  if(SclAutoEndGreen >= 0) SclAutoGreen = 0;}

		if(SclAutoBlue > 0)
		{  if(SclAutoEndBlue <= 0) SclAutoBlue = 0;}
		else
		{  if(SclAutoEndBlue >= 0) SclAutoBlue = 0;}

		SCL_IncColOffset(SclAutoSurface,SclAutoRed,SclAutoGreen,SclAutoBlue);

		if(SclAutoRed == 0 && SclAutoGreen == 0 && SclAutoBlue == 0){
			SclAutoVeFlag = 0;
			if(SclAutoSurface == SCL_OFFSET_A){
				SCL_SET_COARD(SclAutoEndRed2);
				SCL_SET_COAGR(SclAutoEndGreen2);
				SCL_SET_COABL(SclAutoEndBlue2);
			}else{/* OFFSET_B */
				SCL_SET_COBRD(SclAutoEndRed2);
				SCL_SET_COBGR(SclAutoEndGreen2);
				SCL_SET_COBBL(SclAutoEndBlue2);
			}
		}
		SclAutoCount = 0;
	    }
	    break;
    }
}

/* カラーオフセットの値を増減 */
void  SCL_IncColOffset(Uint32 OffsetReg,Sint16 red,Sint16 green,Sint16 blue)
{
	/* オフセットレジスタに値を設定 */
	if(OffsetReg == SCL_OFFSET_A){
		SCL_SET_COARD(SCL_GET_COARD()+red);
		SCL_SET_COAGR(SCL_GET_COAGR()+green);
		SCL_SET_COABL(SCL_GET_COABL()+blue);
	}else{/* OFFSET_B */
		SCL_SET_COBRD(SCL_GET_COBRD()+red);
		SCL_SET_COBGR(SCL_GET_COBGR()+green);
		SCL_SET_COBBL(SCL_GET_COBBL()+blue);
	}
}


static	void	SCL_InitSclOtherPri(void)
{
    SCL_SET_SPCCCS(3);
    SCL_SET_SPCCEN(0);  /* スプライト色演算条件ナンバ */
    SCL_SET_SPCLMD(1); /* スプライトの色形式 */
    SCL_SET_SPWINEN(0);/* スプライトウィンドウイネーブル */
    SCL_SET_SPTYPE(7); /* スプライトのタイプ指定 */
    SCL_SET_N0CAOS(0); /* Color RAM Address offsets */
    SCL_SET_N1CAOS(0);
    SCL_SET_N2CAOS(0);
    SCL_SET_N3CAOS(0);
    SCL_SET_R0CAOS(0);
    SCL_SET_SPCAOS(0);
    SCL_SET_N0CAOS(0);
    SCL_SET_N1CAOS(0);
    SCL_SET_N2CAOS(0);
    SCL_SET_N3CAOS(0);
    SCL_SET_R0CAOS(0);
    SCL_SET_SPCAOS(0);
    SCL_SET_N0LCEN(0); /* line color insert Enable */
    SCL_SET_N1LCEN(0);
    SCL_SET_N2LCEN(0);
    SCL_SET_N3LCEN(0);
    SCL_SET_R0LCEN(0);
    SCL_SET_SPLCEN(0);
    SCL_SET_N0SPRM(0); /* Special Priority Enable */
    SCL_SET_N1SPRM(0);
    SCL_SET_N2SPRM(0);
    SCL_SET_N3SPRM(0);
    SCL_SET_R0SPRM(0);
    SCL_SET_BOKEN(0);  /* ボカシ演算イネーブル */
    SCL_SET_BOKN(0);   /* ボカシ画面番号 */
    SCL_SET_EXCCEN(0); /* 拡張カラー演算イネーブル */
    SCL_SET_CCRTMD(0); /* カラー演算割合モード */
    SCL_SET_CCMD(1);   /* カラー演算モード */
    SCL_SET_SPCCEN(0); /* カラー演算イネーブル */
    SCL_SET_LCCCEN(0);
    SCL_SET_R0CCEN(0);
    SCL_SET_N3CCEN(0);
    SCL_SET_N2CCEN(0);
    SCL_SET_N1CCEN(0);
    SCL_SET_N0CCEN(0);
    SCL_SET_N0SCCM(0); /* 特殊カラー演算モード */
    SCL_SET_N1SCCM(0);
    SCL_SET_N2SCCM(0);
    SCL_SET_N3SCCM(0);
    SCL_SET_R0SCCM(0);
    SCL_SET_N0SDEN(0); /* Shadow Enable */
    SCL_SET_N1SDEN(0);
    SCL_SET_N2SDEN(0);
    SCL_SET_N3SDEN(0);
    SCL_SET_R0SDEN(0);
    SCL_SET_BKSDEN(0);
    SCL_SET_TPSDSL(0); /* 透明シャドウセレクト */
    SclPriBuffDirty.SclOtherPri = 1;
}

static	void  SCL_InitSclSpPriNum(void)
{
    SCL_SET_S0PRIN(0); 
    SCL_SET_S1PRIN(0); 
    SCL_SET_S2PRIN(0); 
    SCL_SET_S3PRIN(0); 
    SCL_SET_S4PRIN(0); 
    SCL_SET_S5PRIN(0); 
    SCL_SET_S6PRIN(0); 
    SCL_SET_S7PRIN(0); 
    SclPriBuffDirty.SclSpPriNum = 1;
}

static	void  SCL_InitSclBgPriNum(void)
{
    SCL_SET_N0PRIN(3);
    SCL_SET_N1PRIN(2);
    SCL_SET_N2PRIN(1);
    SCL_SET_N3PRIN(0);
    SCL_SET_R0PRIN(4);
    SclPriBuffDirty.SclBgPriNum = 1;
}

static	void  SCL_InitSclSpColMix(void)
{
    SCL_SET_S0CCRT(0);
    SCL_SET_S1CCRT(0);
    SCL_SET_S2CCRT(0);
    SCL_SET_S3CCRT(0);
    SCL_SET_S4CCRT(0);
    SCL_SET_S5CCRT(0);
    SCL_SET_S6CCRT(0);
    SCL_SET_S5CCRT(0);
    SclPriBuffDirty.SclSpColMix = 1;
}

static	void  SCL_InitSclBgColMix(void)
{
    SCL_SET_N0CCRT(0);
    SCL_SET_N1CCRT(0);
    SCL_SET_N2CCRT(0);
    SCL_SET_N3CCRT(0);
    SCL_SET_R0CCRT(0);
    SCL_SET_LCCCRT(0);
    SCL_SET_BKCCRT(0);
    SclPriBuffDirty.SclBgColMix = 1;
}

static	void  SCL_InitSclColOffset(void)
{

    SCL_SET_N0COEN(0);
    SCL_SET_N1COEN(0);
    SCL_SET_N2COEN(0);
    SCL_SET_N3COEN(0);
    SCL_SET_R0COEN(0);
    SCL_SET_BKCOEN(0);
    SCL_SET_SPCOEN(0);
    SCL_SET_N0COSL(0);
    SCL_SET_N1COSL(0);
    SCL_SET_N2COSL(0);
    SCL_SET_N3COSL(0);
    SCL_SET_R0COSL(0);
    SCL_SET_BKCOSL(0);
    SCL_SET_SPCOSL(0);
    SCL_SET_COARD(0);    /* カラーオフセットレジスタ A */
    SCL_SET_COAGR(0);
    SCL_SET_COABL(0);
    SCL_SET_COBRD(0);    /* カラーオフセットレジスタ B */
    SCL_SET_COBGR(0);
    SCL_SET_COBBL(0);
    SclPriBuffDirty.SclColOffset = 1;
}

/*  */
void  SCL_SetShadowBit(Uint32 Object)
{
	SCL_SET_TPSDSL(0);
	SCL_SET_N0SDEN(0);
	SCL_SET_N1SDEN(0);
	SCL_SET_N2SDEN(0);
	SCL_SET_N3SDEN(0);
	SCL_SET_R0SDEN(0);
	SCL_SET_BKSDEN(0);

	if(Object) {
		SCL_SET_TPSDSL(1);
		if(SCL_NBG0 & Object)	SCL_SET_N0SDEN(1);
		if(SCL_RBG1 & Object)	SCL_SET_N0SDEN(1);
		if(SCL_NBG1 & Object)	SCL_SET_N1SDEN(1);
		if(SCL_EXBG & Object)	SCL_SET_N1SDEN(1);
		if(SCL_NBG2 & Object)	SCL_SET_N2SDEN(1);
		if(SCL_NBG3 & Object)	SCL_SET_N3SDEN(1);
		if(SCL_RBG0 & Object)	SCL_SET_R0SDEN(1);
		if(SCL_BACK & Object)	SCL_SET_BKSDEN(1);
	}
}
