/*------------------------------------------------------------------------
 *  FILE:	SCL_Fu01.c
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

#include	<sega_scl.h> 
#include	"sega_sys.h"

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;

extern	Uint16	SclDisplayX;
extern	Uint16	SclDisplayY;

extern	Uint16	SclProcess;

/*------------------------------------------------------------------------
 *
 * NAME : SCL_SetDisplayMode
 *
 * PARAMETERS
 *	param1 -	interlace
 *	param2 -	vertical
 *	param3 -	horizontal
 *
 * DESCRIPTION
 *
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 * 
 * CAVEATS
 * 
 *------------------------------------------------------------------------
 */
void SCL_SetDisplayMode(Uint8 interlace,Uint8 vertical,Uint8 horizontal)
{
	Uint32	SysClock;

	/* 現在のクロックを得る */
	SysClock = SYS_GETSYSCK;

	Scl_s_reg.tvmode &= 0xffcf;
	switch(vertical)  {
		case	SCL_224LINE:
			SclDisplayY = 224;
			break;
		case	SCL_240LINE:
			SclDisplayY = 240;
			Scl_s_reg.tvmode |= 0x0010;
			break;
		case	SCL_256LINE:
			SclDisplayY = 256;
			Scl_s_reg.tvmode |= 0x0020;
			break;
	}

	Scl_s_reg.tvmode &= 0xff3f;
	switch(interlace)  {
		case	SCL_NON_INTER:
			break;
		case	SCL_SINGLE_INTER:
			Scl_s_reg.tvmode |= 0x0080;
			break;
		case	SCL_DOUBLE_INTER:
			SclDisplayY *= 2;
			Scl_s_reg.tvmode |= 0x00c0;
			break;
	}

	Scl_s_reg.tvmode &= 0xfff0;
	switch(horizontal) {
		case	SCL_NORMAL_A:
			/* システムクロックを２６Ｍに切替え  */
			if(SysClock != 0)	SYS_CHGSYSCK(0);
			SclDisplayX = 320;
			break;
		case	SCL_NORMAL_B:
			/* システムクロックを２８Ｍに切替え  */
			if(SysClock != 1)	SYS_CHGSYSCK(1);
			SclDisplayX = 352;
			Scl_s_reg.tvmode |= 0x0001;
			break;
		case	SCL_HIRESO_A:
			/* システムクロックを２６Ｍに切替え  */
			if(SysClock != 0)	SYS_CHGSYSCK(0);
			SclDisplayX = 640;
			Scl_s_reg.tvmode |= 0x0002;
			break;
		case	SCL_HIRESO_B:
			/* システムクロックを２８Ｍに切替え  */
			if(SysClock != 1)	SYS_CHGSYSCK(1);
			SclDisplayX = 704;
			Scl_s_reg.tvmode |= 0x0003;
			break;
		case	SCL_NORMAL_AE:
			/* システムクロックを２６Ｍに切替え  */
			if(SysClock != 0)	SYS_CHGSYSCK(0);
			SclDisplayX = 320;
			SclDisplayY = 480;
			Scl_s_reg.tvmode |= 0x0004;
			break;
		case	SCL_NORMAL_BE:
			/* システムクロックを２８Ｍに切替え  */
			if(SysClock != 1)	SYS_CHGSYSCK(1);
			SclDisplayX = 352;
			SclDisplayY = 480;
			Scl_s_reg.tvmode |= 0x0005;
			break;
		case	SCL_HIRESO_AE:
			/* システムクロックを２６Ｍに切替え  */
			if(SysClock!=0)	SYS_CHGSYSCK(0);
			SclDisplayX = 640;
			SclDisplayY = 480;
			Scl_s_reg.tvmode |= 0x0006;
			break;
		case	SCL_HIRESO_BE:
			/* システムクロックを２８Ｍに切替え  */
			if(SysClock != 1)	SYS_CHGSYSCK(1);
			SclDisplayX = 704;
			SclDisplayY = 480;
			Scl_s_reg.tvmode |= 0x0007;
			break;
	}
	if(SclProcess == 0)	SclProcess = 1;
}
