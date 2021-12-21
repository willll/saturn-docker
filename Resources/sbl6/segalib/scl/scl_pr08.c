/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr08.c
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
Uint8  SCL_GetPriority(Uint32 Object)
{
    switch(Object){
      case SCL_SP0:
	return(SCL_GET_S0PRIN());
	break;
      case SCL_SP1:
	return(SCL_GET_S1PRIN());
	break;
      case SCL_SP2:
	return(SCL_GET_S2PRIN());
	break;
      case SCL_SP3:
	return(SCL_GET_S3PRIN());
	break;
      case SCL_SP4:
	return(SCL_GET_S4PRIN());
	break;
      case SCL_SP5:
	return(SCL_GET_S5PRIN());
	break;
      case SCL_SP6:
	return(SCL_GET_S6PRIN());
	break;
      case SCL_SP7:
	return(SCL_GET_S7PRIN());
	break;
      case SCL_NBG0:
      case SCL_RBG1:
	return(SCL_GET_N0PRIN());
	break;
      case SCL_NBG1:
      case SCL_EXBG:
	return(SCL_GET_N1PRIN());
	break;
      case SCL_NBG2:
	return(SCL_GET_N2PRIN());
	break;
      case SCL_NBG3:
	return(SCL_GET_N3PRIN());
	break;
      case SCL_RBG0:
	return(SCL_GET_R0PRIN());
	break;
      default:
	break;
    }
}
