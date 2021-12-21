/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr06.c
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

extern	Uint32	SclLineColRamOffset;

/* カラー RAM のオフセットを（バッファに）書込む */
void  SCL_SetColRamOffset(Uint32 Object, Uint32 Offset,Uint8 transparent)
{
    if(Object & SCL_SPR)	SCL_SET_SPCAOS(Offset);

    if( (Object & SCL_NBG0) || (Object & SCL_RBG1))
    {
	SCL_SET_N0CAOS(Offset);
	if(transparent)	Scl_s_reg.dispenbl |= 0x0100;
	else		Scl_s_reg.dispenbl &= 0xfeff;
    }

    if( (Object & SCL_NBG1) || (Object & SCL_EXBG))
    {
	SCL_SET_N1CAOS(Offset);
	if(transparent)	Scl_s_reg.dispenbl |= 0x0200;
	else		Scl_s_reg.dispenbl &= 0xfdff;
    }

    if(Object & SCL_NBG2)
    {
	SCL_SET_N2CAOS(Offset);
	if(transparent)	Scl_s_reg.dispenbl |= 0x0400;
	else		Scl_s_reg.dispenbl &= 0xfbff;
    }

    if(Object & SCL_NBG3)
    {
    	SCL_SET_N3CAOS(Offset);
	if(transparent)	Scl_s_reg.dispenbl |= 0x0800;
	else		Scl_s_reg.dispenbl &= 0xf7ff;
    }

    if(Object & SCL_RBG0)
    {
	SCL_SET_R0CAOS(Offset);
	if(transparent)	Scl_s_reg.dispenbl |= 0x1000;
	else		Scl_s_reg.dispenbl &= 0xefff;
    }

    if(Object & SCL_LNCL) {
	SclLineColRamOffset = Offset;
    }
}
