/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr02.c
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

#define SEGA_SCL_PROTO
#define	DMAOFF

#include <sega_scl.h> 
#include <sega_dma.h> 

extern	void	SCL_Memcpyw(void *dest,void *src,Uint32 tcnt);

extern	Uint32  SCL_GetColRamMode(void);
extern	Uint32  SCL_GetColRamOffset(Uint32 object);


 /* �J���[ RAM (�o�b�t�@)�ւ̃J���[�̃Z�b�g */
void  SCL_SetColRam(Uint32 Object, Uint32 Index,Uint32 num,void *Color)
{
Uint32	*color32,*ram32;
Uint16	*color16,*ram16;

    if(SCL_GetColRamMode() == 2){
	color32 = (Uint32 *)Color;
/*	ram32 = (Uint32 *)((Uint32)ColorRAM + ((SCL_GetColRamOffset(Object) * 0x400))) + Index;*/
	ram32 = (Uint32 *)((Uint32)SCL_COLRAM_ADDR + ((SCL_GetColRamOffset(Object) * 0x400))) + Index;

	if(num==1)
	{   /* 24 bits color mode */
	    ram32[0] = color32[0];
	}else{
#ifdef	DMAOFF
	    SCL_Memcpyw(ram32,color32,num*4);
#else
	    DMA_ScuMemCopy(ram32,color32,num*4);
#endif	/* DMAOFF */
	}
    }else{
	color16 = (Uint16 *)Color;
/*	ram16   = (Uint16 *)((Uint32)ColorRAM + ((SCL_GetColRamOffset(Object) * 0x200))) + Index;*/
	ram16   = (Uint16 *)((Uint32)SCL_COLRAM_ADDR + ((SCL_GetColRamOffset(Object) * 0x200))) + Index;

	if(num==1)
	{   /* 16 bits color mode */
	    ram16[0] = color16[0];
	}else{
#ifdef	DMAOFF
	    SCL_Memcpyw(ram16,color16,num*2);
#else
	    DMA_ScuMemCopy(ram16,color16,num*2);
#endif	/* DMAOFF */
	}
    }
}
