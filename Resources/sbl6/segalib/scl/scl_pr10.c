/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr10.c
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
extern	Uint8	SclAutoRGB[6][128];


/* 自動カラー演算をセット */
void  SCL_SetAutoColMix(Uint32 Surfaces,Uint32 Interval,Uint32 Time,
			Uint8 StartRate,Uint8 EndRate)
{
    Uint8   dRate;

    if(SclAutoVeFlag) return;
    if(StartRate == EndRate) return;
    SclAutoVeFlag = 2;
    SclAutoSurface = Surfaces;
    SclAutoInterval = Interval;
    if(EndRate > StartRate)
    {	dRate = EndRate - StartRate;
	SclAutoPlusFlag = 1;
    }
    else
    {	dRate = StartRate - EndRate;
	SclAutoPlusFlag = 0;
    }
    if(Time)	SclAutoInterval = Time/dRate;

    SclAutoCurRate = StartRate;
    SclAutoEndRate = EndRate;
    SclAutoCount = SclAutoInterval;
}

