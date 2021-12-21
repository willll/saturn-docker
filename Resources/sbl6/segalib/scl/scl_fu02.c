/*------------------------------------------------------------------------
 *  FILE:	SCL_Fu02.c
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

extern	Uint32	SCL_GetColRamMode(void);

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;
extern	SclRotscl	Scl_r_reg;

extern	Uint16	SclDisplayX;
extern	Uint16	SclDisplayY;

extern	Uint32	SclRbgKtbAddr[];
extern	Sint32	SclRbgKtbOffset[];

extern	Uint8	SclRa,SclRb;

/*------------------------------------------------------------------------
 *
 * NAME : SCL_InitVramConfigTb
 *
 * PARAMETERS
 *	param1 -	pointer to SclVramConfig
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
void  SCL_InitVramConfigTb(SclVramConfig *tp)
{
	tp->ktboffsetA = 0;
	tp->ktboffsetB = 0;
	/* VRAM A をバンク区切るかどうかを指定 */
	tp->vramModeA=OFF;
	/* VRAM B をバンク区切るかどうかを指定 */
	tp->vramModeB=OFF;

	tp->vramA0 = SCL_NON;
	tp->vramA1 = SCL_NON;
	tp->vramB0 = SCL_NON;
	tp->vramB1 = SCL_NON;
	tp->colram = SCL_NON;
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_SetVramConfig
 *
 * PARAMETERS
 *	param1 -	pointer to SclVramConfig
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
void  SCL_SetVramConfig(SclVramConfig *tp)
{
	Uint8 sw,offsw;

	SclRbgKtbAddr[0]   = 0;
	SclRbgKtbAddr[1]   = 0;
	SclRbgKtbOffset[0] = tp->ktboffsetA;
	SclRbgKtbOffset[1] = tp->ktboffsetB;

	Scl_s_reg.ramcontrl &= 0x7fff;

	/* VRAM A をバンク切り替えするかどうかを指定 */
	if(tp->vramModeA==ON)	Scl_s_reg.ramcontrl |= 0x0100;
	else			Scl_s_reg.ramcontrl &= 0xfeff;

	/* VRAM B をバンク切り替えするかどうかを指定 */
	if(tp->vramModeB==ON)	Scl_s_reg.ramcontrl |= 0x0200;
	else			Scl_s_reg.ramcontrl &= 0xfdff;

	/************************************************/
	/* SCL_NON 	 0    RBG0では使用しない        */
	/* SCL_RBG0_K  	 1    RBG0の係数ﾃｰﾌﾞﾙを置く     */
	/* SCL_RBG0_PN 	 2    RBG0のﾊﾟﾀｰﾝﾈｰﾑﾃｰﾌﾞﾙを置く */
	/* SCL_RBG0_CHAR 3    RBG0のｷｬﾗｸﾀを置く         */
	/************************************************/
	sw = 0;offsw=0;

	/* VRAM A or A0 を何で使用するかを指定 */
	if(tp->vramA0 < 4) {
		Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0xfffc) | (Uint16)tp->vramA0;
		if(tp->vramA0 == SCL_RBG0_K) {
			if(SclRbgKtbOffset[0])	Scl_s_reg.ramcontrl &= 0xfffc;
			else			offsw = 1;
			sw = 1;
			Scl_r_reg.k_offset &= 0xff00;
			SclRbgKtbAddr[0] = SCL_VDP2_VRAM_A0 + SclRbgKtbOffset[0];
		}
	} else {
		if(tp->vramA0 == SCL_RBG1_K) {
			Scl_r_reg.k_offset &= 0x00ff;
			SclRbgKtbAddr[1] = SCL_VDP2_VRAM_A0 + SclRbgKtbOffset[1];
		}
	}

	/* VRAM A1 を何で使用するかを指定 */
	if(tp->vramA1 < 4) {
		Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0xfff3) | (Uint16)(tp->vramA1 << 2);
		if(tp->vramA1 == SCL_RBG0_K) {
			if(sw == 0) {
				if(SclRbgKtbOffset[0])	Scl_s_reg.ramcontrl &= 0xfff3;
				else			offsw = 2;
				Scl_r_reg.k_offset &= 0xff00;
				Scl_r_reg.k_offset |= 0x0001;
				SclRbgKtbAddr[0] = SCL_VDP2_VRAM_A1 + SclRbgKtbOffset[0];
				sw = 1;
			} else {
				if(SclRbgKtbOffset[1])	Scl_s_reg.ramcontrl &= 0xfff3;
				Scl_r_reg.k_offset &= 0x00ff;
				Scl_r_reg.k_offset |= 0x0100;
				SclRbgKtbAddr[1] = SCL_VDP2_VRAM_A1 + SclRbgKtbOffset[1];
				sw++;
			}
		}
	} else {
		if(tp->vramA1 == SCL_RBG1_K) {
			Scl_r_reg.k_offset &= 0x00ff;
			Scl_r_reg.k_offset |= 0x0100;
			SclRbgKtbAddr[1] = SCL_VDP2_VRAM_A1 + SclRbgKtbOffset[1];
		}
	}

	/* VRAM B or B0 を何で使用するかを指定 */
	if(tp->vramB0 < 4) {
		Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0xffcf) | (Uint16)(tp->vramB0 << 4);
		if(tp->vramB0 == SCL_RBG0_K) {
			if(sw == 0) {
				if(SclRbgKtbOffset[0])	Scl_s_reg.ramcontrl &= 0xffcf;
				else			offsw = 3;
				Scl_r_reg.k_offset &= 0xff00;
				Scl_r_reg.k_offset |= 0x0002;
				sw = 1;
				SclRbgKtbAddr[0] = SCL_VDP2_VRAM_B0 + SclRbgKtbOffset[0];
			} else {
				if(SclRbgKtbOffset[1])	Scl_s_reg.ramcontrl &= 0xffcf;
				Scl_r_reg.k_offset &= 0x00ff;
				Scl_r_reg.k_offset |= 0x0200;
				sw++;
				SclRbgKtbAddr[1] = SCL_VDP2_VRAM_B0 + SclRbgKtbOffset[1];
			}
		}
	} else {
		if(tp->vramB0 == SCL_RBG1_K && SclRbgKtbOffset[1]) {
			Scl_r_reg.k_offset &= 0x00ff;
			Scl_r_reg.k_offset |= 0x0200;
			SclRbgKtbAddr[1] = SCL_VDP2_VRAM_B0 + SclRbgKtbOffset[1];
		}
	}

	/* VRAM B1 を何で使用するかを指定 */
	if(tp->vramB1 < 4) {
		Scl_s_reg.ramcontrl = (Scl_s_reg.ramcontrl & 0xff3f) | (Uint16)(tp->vramB1 << 6);
		if(tp->vramB1 == SCL_RBG0_K) {
			if(sw == 0) {
				if(SclRbgKtbOffset[0])	Scl_s_reg.ramcontrl &= 0xff3f;
				else			offsw = 4;
				Scl_r_reg.k_offset &= 0xff00;
				Scl_r_reg.k_offset |= 0x0003;
				sw = 1;
				SclRbgKtbAddr[0] = SCL_VDP2_VRAM_B1 + SclRbgKtbOffset[0];
			} else {
				if(SclRbgKtbOffset[1])	Scl_s_reg.ramcontrl &= 0xff3f;
				Scl_r_reg.k_offset &= 0x00ff;
				Scl_r_reg.k_offset |= 0x0300;
				sw++;
				SclRbgKtbAddr[1] = SCL_VDP2_VRAM_B1 + SclRbgKtbOffset[1];
			}
		}
	} else {
		if(tp->vramB1 == SCL_RBG1_K && SclRbgKtbOffset[1]) {
			Scl_r_reg.k_offset &= 0x00ff;
			Scl_r_reg.k_offset |= 0x0300;
			SclRbgKtbAddr[1] = SCL_VDP2_VRAM_B1 + SclRbgKtbOffset[1];
		}
	}

    if(!SclRbgKtbAddr[1]) {
	if(tp->colram==SCL_RBG0_K && !SclRbgKtbAddr[0] && SCL_GetColRamMode() == SCL_CRM15_2048) {
		Scl_s_reg.ramcontrl |= 0x8000;
		SclRbgKtbAddr[0] = SCL_COLRAM_ADDR + 0x800;
	}else if(SclRbgKtbAddr[0] && SclRb!=SCL_NON/* && !SclRbgKtbOffset[1]*/) {
		if(!SclRbgKtbOffset[1])
			SclRbgKtbOffset[1] = 0x400 + SclRbgKtbOffset[0];
		if(!SclRbgKtbOffset[0])	SclRbgKtbOffset[0] = -1;
		SclRbgKtbAddr[1] = SclRbgKtbAddr[0] + SclRbgKtbOffset[1];
		Scl_r_reg.k_offset = (Scl_r_reg.k_offset & 0x00ff)
					| Scl_r_reg.k_offset << 8;
		switch(offsw){
			case	1:
				Scl_s_reg.ramcontrl &= 0xfffc;
				break;
			case	2:
				Scl_s_reg.ramcontrl &= 0xfff3;
				break;
			case	3:
				Scl_s_reg.ramcontrl &= 0xffcf;
				break;
			case	4:
				Scl_s_reg.ramcontrl &= 0xff3f;
				break;
		}
	}else if(!SclRbgKtbAddr[0]) {
		if(SclRbgKtbOffset[0] && SclRa!=SCL_NON) {
			Scl_r_reg.k_offset &= 0xff00;
			Scl_r_reg.k_offset |= (Uint16)(SclRbgKtbOffset[0]/0x20000);
			SclRbgKtbAddr[0] = SCL_VDP2_VRAM + SclRbgKtbOffset[0];
		}
		if(SclRbgKtbOffset[1] && SclRb!=SCL_NON) {
			Scl_r_reg.k_offset &= 0x00ff;
			Scl_r_reg.k_offset |= ((Uint16)(SclRbgKtbOffset[1]/0x20000)) << 8;
			SclRbgKtbAddr[1] = SCL_VDP2_VRAM + SclRbgKtbOffset[1];
		}
	}
    }
}
