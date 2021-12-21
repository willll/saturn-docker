/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr09.c
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

/* スプライトのカラー演算条件の設定 */
void  SCL_SetColMixMode(Uint32 ColMixPriority, Uint8 Mode)
{
    SCL_SET_SPCCCS(Mode);
    SCL_SET_SPCCN(ColMixPriority);
}
