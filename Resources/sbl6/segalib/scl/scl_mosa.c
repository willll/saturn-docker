/*------------------------------------------------------------------------
 *  FILE:	SCL_Mosa.c
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *		Mosaic function program
 *
 *  AUTHOR(S):
 *		K.M
 *
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 

extern	SclSysreg	Scl_s_reg;
extern	Uint16		SclProcess;

void	SCL_SetMosaic(Uint32 surface,Uint8 x,Uint8 y)
{
    Uint16   Work;

    Work = 0;

    if( x > 0 && y > 0 && x < 17 && y < 17 && surface != SCL_NON)
    {
	Work = (y - 1)  << 4;
	Work |= x - 1;
	Work = Work << 8;

	if(surface & SCL_NBG0)
	{
	    Work |= 0x0001;
	}
	if(surface & SCL_NBG1)
	{
	    Work |= 0x0002;
	}
	if(surface & SCL_NBG2)
	{
	    Work |= 0x0004;
	}
	if(surface & SCL_NBG3)
	{
	    Work |= 0x0008;
	}
	if(surface & SCL_RBG0)
	{
	    Work |= 0x0010;
	}
	if(surface & SCL_RBG1)
	{
	    Work |= 0x0001;
	}
	if(surface & SCL_EXBG)
	{
	    Work |= 0x0002;
	}
    }

    Scl_s_reg.mosaic = Work;
    if(SclProcess == 0)	SclProcess = 1;
}
