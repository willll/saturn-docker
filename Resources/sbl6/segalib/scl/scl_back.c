/*------------------------------------------------------------------------
 *  FILE:	SCL_Back.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	Back setting function program
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

extern	void	SCL_Memcpyw(void *dest,void *src,Uint32 tcnt);

extern	SclNorscl	Scl_n_reg;
extern	Uint16		SclDisplayY;
extern	Uint16		SclProcess;


void SCL_SetBack(Uint32 addr,Uint16 dataSize,Uint16 *dataTb)
{
    if(dataSize == 1 || dataSize == SclDisplayY)
    {
	Scl_n_reg.backcolmode = addr & 0x0007ffff;

	SCL_Memcpyw((void *)(Scl_n_reg.backcolmode | 0x25e00000),dataTb,dataSize*2);

	Scl_n_reg.backcolmode /=2;

	if(dataSize != 1)
		Scl_n_reg.backcolmode |= 0x80000000;
	if(SclProcess == 0)	SclProcess = 1;
    }
}
