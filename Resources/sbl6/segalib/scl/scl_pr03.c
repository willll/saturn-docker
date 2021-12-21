/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr03.c
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
 *	Updated by K.M on 1994-11-11 Ver.1.05
 * 
 *------------------------------------------------------------------------
 */

#include <sega_scl.h> 

extern	void  SCL_SetColRamOffset(Uint32 Object, Uint32 Offset,Uint8 transparent);

/* カラーＲＡＭ管理テーブル */
extern	Uint32	SclColRamAlloc256[];
extern	Uint32	SclColRamAlloc2048[];

/* カラーＲＡＭに確保した領域の解放 */
void  SCL_FreeColRam(Uint32 Surface)
{
	Uint32	work;
	Uint8	i;

	work = Surface ^ 0xffffffff;

	for(i=0;i<8;i++){
		SclColRamAlloc256[i] &= work;
	}
	SclColRamAlloc2048[0] &= work;
}


/* カラーＲＡＭに領域確保 */
Uint32  SCL_AllocColRam(Uint32 Surface, Uint32 NumOfColors, Uint8 transparent)
{
	Uint8	Max256;
	Uint32	i,Size256;

	SCL_FreeColRam(Surface);

	if(SclColRamAlloc2048[0] != 0)
		return((Uint32 )NULL); /* アロケートエラー */

	switch(SCL_GetColRamMode()){
		case SCL_CRM15_1024:
			if(1024 < NumOfColors)	return((Uint32 )NULL);
			Max256=4;
			Size256=512;
			break;
		case SCL_CRM15_2048:
			if(2048 < NumOfColors)	return((Uint32 )NULL);
			Max256=8;
			Size256=512;
			break;
		case SCL_CRM24_1024:
			if(1024 < NumOfColors)	return((Uint32 )NULL);
			Max256=4;
			Size256=1024;
			break;
	}

	if(256 >= NumOfColors){
		for(i=0;i<Max256;i++){
			if(SclColRamAlloc256[i]==0){
				SclColRamAlloc256[i]=Surface;
				SCL_SetColRamOffset(Surface,i,transparent);
				return(SCL_COLRAM_ADDR+(Size256*i));
			}
		}
	}else{
		for(i=0;i<Max256;i++)
			if(SclColRamAlloc256[i] != 0) return((Uint32 )NULL);
		SclColRamAlloc2048[0] = Surface;
		SCL_SetColRamOffset(Surface,0,transparent);
		return(SCL_COLRAM_ADDR);
	}
}
