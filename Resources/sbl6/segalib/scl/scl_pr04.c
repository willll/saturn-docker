/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr04.c
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
 *	Written by K.M on 1994-07-01 Ver.1.00
 *	Updated by K.M on 1994-07-01 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#include <sega_scl.h> 

extern	Uint32	SclAutoSurface;
extern	Uint32	SclAutoInterval;
extern	Uint32	SclAutoCount;
extern	Uint32	SclAutoIndex;
extern	Uint32	SclAutoMax;
extern	Sint16	SclAutoRed,SclAutoGreen,SclAutoBlue;
extern	Sint16	SclAutoEndRed,SclAutoEndGreen,SclAutoEndBlue;
extern	Uint8	SclAutoCurRate;
extern	Uint8	SclAutoEndRate;
extern	Uint8	SclAutoVeFlag;
extern	Uint8	SclAutoPlusFlag;
extern	Uint8	SclAutoRGB[6][256];


/* 自動カラーチェンジをセット */
void  SCL_SetAutoColChg(Uint32 Surface,Uint32 Interval,Uint32 Index,
			Uint32 NumOfCol,Uint32 NumOfTbl,Uint32 *ChangeTbl)
{
    Uint32 *LWork;
    Uint16 i,*Work;
    Uint8  *WorkChangeTbl;

    if(SclAutoVeFlag) return;
    SclAutoVeFlag = 1;
    SclAutoSurface = Surface;
    SclAutoInterval = Interval;
    SclAutoIndex = Index;

    SclAutoMax = NumOfCol;
    if(SclAutoMax > 128) SclAutoMax=128;

    WorkChangeTbl  = (Uint8 *)ChangeTbl;
    if(SCL_GetColRamMode()==SCL_CRM24_1024) {
	for(i=0;i<SclAutoMax;i++) {
	    if(NumOfTbl==1) {
		SclAutoRGB[0][i] = WorkChangeTbl[1];
		SclAutoRGB[1][i] = WorkChangeTbl[2];
		SclAutoRGB[2][i] = WorkChangeTbl[3];
	    }else{
	        SclAutoRGB[0][i] = WorkChangeTbl[4*i+1];
	        SclAutoRGB[1][i] = WorkChangeTbl[4*i+2];
	        SclAutoRGB[2][i] = WorkChangeTbl[4*i+3];
	    }
	    LWork = (Uint32 *)((Uint32)SCL_COLRAM_ADDR + ((SCL_GetColRamOffset(Surface) * 0x400))) + Index;
	    SclAutoRGB[3][i] = (Uint8)((LWork[0] & 0x00ff0000) >>8 );
	    SclAutoRGB[4][i] = (Uint8)((LWork[0] & 0x0000ff00) >>4 );
	    SclAutoRGB[5][i] = (Uint8)((LWork[0] & 0x000000ff));
	}
    }else{
        for(i=0;i<SclAutoMax;i++) {
	    if(NumOfTbl==1)
	    {   SclAutoRGB[0][i] = (WorkChangeTbl[1] & 0x7c)>>2;
	        SclAutoRGB[1][i] = (Uint8)((ChangeTbl[0] & 0x03e0)>>5);
	        SclAutoRGB[2][i] = WorkChangeTbl[1] & 0x1f;
	    }
	    else
	    {   SclAutoRGB[0][i] = (WorkChangeTbl[2*i] & 0x7c)>>2;
	        SclAutoRGB[1][i] = (Uint8)((ChangeTbl[i] & 0x03e0)>>5);
	        SclAutoRGB[2][i] = WorkChangeTbl[2*i+1] & 0x1f;
	    }
            Work = (Uint16 *)((Uint32)SCL_COLRAM_ADDR + ((SCL_GetColRamOffset(Surface) * 0x200))) + Index;
	    SclAutoRGB[3][i] = (Uint8)(Work[0] >> 10);
	    SclAutoRGB[4][i] = (Uint8)((Work[0] & 0x03e0)>>5);
	    SclAutoRGB[5][i] = (Uint8)(Work[0] & 0x1f);
	}
    }
    SclAutoCount = SclAutoInterval;
}
