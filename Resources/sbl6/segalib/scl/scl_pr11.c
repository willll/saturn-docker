/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr11.c
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
extern	Sint16	SclAutoEndRed2,SclAutoEndGreen2,SclAutoEndBlue2;
extern	Uint8	SclAutoCurRate;
extern	Uint8	SclAutoEndRate;
extern	Uint8	SclAutoVeFlag;
extern	Uint8	SclAutoPlusFlag;
extern	Uint8	SclAutoRGB[6][128];


 /* 自動カラーオッフセットを設定 */
void  SCL_SetAutoColOffset(Uint32 OffsetReg,Uint32 Interval,Uint32 Time,
				SclRgb *start,SclRgb *end)
{
	if(SclAutoVeFlag) return;
	if(!Interval && !Time) return;
	SclAutoVeFlag = 3;
	SclAutoSurface = OffsetReg;
	SclAutoInterval = Interval;
	SclAutoRed   = end->red   - start->red;
	SclAutoGreen = end->green - start->green;
	SclAutoBlue  = end->blue  - start->blue;

	SclAutoEndRed   = SclAutoRed;
	SclAutoEndGreen = SclAutoGreen;
	SclAutoEndBlue  = SclAutoBlue;

	SclAutoEndRed2   = end->red;
	SclAutoEndGreen2 = end->green;
	SclAutoEndBlue2  = end->blue;

	if(Time!=0 && ABS(SclAutoRed) > Time)  SclAutoRed  /= (Sint16 )Time;
	else if(SclAutoRed > 0)   SclAutoRed = 1;
	else              SclAutoRed = - 1;
	if(Time!=0 && ABS(SclAutoGreen) > Time)  SclAutoGreen  /= (Sint16 )Time;
	else if(SclAutoGreen > 0) SclAutoGreen = 1;
	else              SclAutoGreen = -1;
	if(Time!=0 && ABS(SclAutoBlue) > Time)  SclAutoBlue /= (Sint16 )Time;
	else if(SclAutoBlue > 0)  SclAutoBlue = 1;
	else              SclAutoBlue = -1;

	if(Time) SclAutoInterval=0;
	SclAutoCount = SclAutoInterval;
}
