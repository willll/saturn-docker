/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr05.c
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

/* カラーＲＡＭのアドレスオフセットを取得 */
Uint32  SCL_GetColRamOffset(Uint32 Object)
{
    switch(Object){
      case SCL_SPR:
	return(SCL_GET_SPCAOS());
	break;
      case SCL_NBG0:
      case SCL_RBG1:
	return(SCL_GET_N0CAOS());
	break;
      case SCL_NBG1:
      case SCL_EXBG:
	return(SCL_GET_N1CAOS());
	break;
      case SCL_NBG2:
	return(SCL_GET_N2CAOS());
	break;
      case SCL_NBG3:
	return(SCL_GET_N3CAOS());
	break;
      case SCL_RBG0:
	return(SCL_GET_R0CAOS());
	break;
      case SCL_LNCL:
	return(SclLineColRamOffset);
	break;
      default:
	break;
    }
}
