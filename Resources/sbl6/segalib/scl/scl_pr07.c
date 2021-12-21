/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr07.c
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

/* プライオリティ番号の設定関数*/
void  SCL_SetPriority(Uint32 Object, Uint8 Priority)
{
Uint16 PriNum;

    PriNum = Priority;
    if(SCL_SP0 & Object)	SCL_SET_S0PRIN(PriNum);
    if(SCL_SP1 & Object)	SCL_SET_S1PRIN(PriNum);
    if(SCL_SP2 & Object)	SCL_SET_S2PRIN(PriNum);
    if(SCL_SP3 & Object)	SCL_SET_S3PRIN(PriNum);
    if(SCL_SP4 & Object)	SCL_SET_S4PRIN(PriNum);
    if(SCL_SP5 & Object)	SCL_SET_S5PRIN(PriNum);
    if(SCL_SP6 & Object)	SCL_SET_S6PRIN(PriNum);
    if(SCL_SP7 & Object)	SCL_SET_S7PRIN(PriNum);
    if(SCL_NBG0 & Object)	SCL_SET_N0PRIN(PriNum);
    if(SCL_RBG1 & Object)	SCL_SET_N0PRIN(PriNum);
    if(SCL_NBG1 & Object)	SCL_SET_N1PRIN(PriNum);
    if(SCL_EXBG & Object)	SCL_SET_N1PRIN(PriNum);
    if(SCL_NBG2 & Object)	SCL_SET_N2PRIN(PriNum);
    if(SCL_NBG3 & Object)	SCL_SET_N3PRIN(PriNum);
    if(SCL_RBG0 & Object)	SCL_SET_R0PRIN(PriNum);
}
