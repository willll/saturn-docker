/*------------------------------------------------------------------------
 *  FILE:	SCL_Pr01.c
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

/*#define SEGA_SCL_PROTO*/

#include <sega_scl.h> 

/* スプライトのタイプをセットする */
void  SCL_SetSpriteMode(Uint8 Type, Uint8 ColMode, Uint8 WinMode)
{
    SCL_SET_SPTYPE(Type);
    SCL_SET_SPCLMD(ColMode);
    SCL_SET_SPWINEN(WinMode);
}
