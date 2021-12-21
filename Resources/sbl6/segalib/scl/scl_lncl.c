/*------------------------------------------------------------------------
 *  FILE:	SCL_Lncl.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	Line Color & Back setting function program
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

extern	SclNorscl	Scl_n_reg;
extern	Uint16		SclDisplayY;
extern	Uint16		SclProcess;
extern	Uint32		SclLineColRamOffset;


void SCL_SetLncl(Uint32 addr,Uint16 tbnum,Uint16 *palNumTb)
{
    Uint16	*vram16,Offset,i;

    Offset = SclLineColRamOffset * 256;

    if(tbnum == 1 || tbnum == SclDisplayY)
    {
	Scl_n_reg.linecolmode = addr & 0x0007ffff;

	vram16 = (Uint16 *)(Scl_n_reg.linecolmode | 0x25e00000);

	for (i=0;i<tbnum;i++)	vram16[i] = palNumTb[i]+Offset;

	Scl_n_reg.linecolmode /= 2;

	if(tbnum != 1)
		Scl_n_reg.linecolmode |= 0x80000000;
	if(SclProcess == 0)	SclProcess = 1;
    }
}
