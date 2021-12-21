/*------------------------------------------------------------------------
 *  FILE:	SCL_Fu04.c
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
 *	Written by K.M on 1994-06-21 Ver.1.00
 *	Updated by K.M on 1994-06-21 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;

/*------------------------------------------------------------------------
 *
 * NAME : SCL_SetCycleTable
 *
 * PARAMETERS
 *	param1 -	Uint16 *	a pointer to cycle table 
 *	param2 -	Uint16 *	a pointer to data table 
 *
 * DESCRIPTION
 *		Set data into VRAM access cycle table
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 * 
 * CAVEATS
 * 
 *------------------------------------------------------------------------
 */
void	SCL_SetCycleTable(Uint16 *tp)
{
	Uint16 i;
	for(i = 0; i<8; i++){
		Scl_s_reg.vramcyc[i] = tp[i];
	}
}
