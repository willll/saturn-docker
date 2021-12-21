/*------------------------------------------------------------------------
 *  FILE:	SCL_Fu03.c
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
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;

extern	Uint16	SclProcess;

extern	Uint8	SclRa,SclRb;

/*------------------------------------------------------------------------
 *
 * NAME : SCL_InitConfigTb
 *
 * PARAMETERS
 *	param1 -	pointer to SclConfig
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
void SCL_InitConfigTb(SclConfig *scfg)
{
	Uint8	i;

	scfg->dispenbl      = OFF;
	scfg->charsize      = SCL_CHAR_SIZE_1X1;
	scfg->pnamesize     = SCL_PN2WORD;
	scfg->platesize     = SCL_PL_SIZE_1X1;
	scfg->bmpsize       = SCL_BMP_SIZE_512X256;
	scfg->coltype       = SCL_COL_TYPE_16;
	scfg->datatype      = SCL_CELL;
	scfg->flip          = SCL_PN_10BIT;
	scfg->mapover       = SCL_OVER_0;
	scfg->patnamecontrl = 0;

	for(i=0;i<16;i++)	scfg->plate_addr[i] = 0;
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_SetConfig
 *
 * PARAMETERS
 *	param1 -	Uint16		sclnum;
 *	param2 -	pointer to SclConfig
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
void SCL_SetConfig(Uint16 sclnum, SclConfig *scfg)
{
	Uint16	temp, *mapoffset, boundary, *map, *map2, shift,shift2;
	Uint16	max, i, flip, mapoffsetW;

	map2 = 0;

	flip = scfg->flip;				/* get flip bit*/
	flip = (flip<<14) & 0x4000;
	switch(sclnum) {
		case SCL_NBG0:
			temp = scfg->dispenbl;		/* display enable */
 			temp &= 0x0001;
			Scl_s_reg.dispenbl &= 0xfffe;
			Scl_s_reg.dispenbl |= temp;

			temp = scfg->charsize;		/* char size */
			temp &= 0x0001;
			Scl_d_reg.charcontrl0 &= 0xfffe;
			Scl_d_reg.charcontrl0 |= temp;

			Scl_d_reg.patnamecontrl[0] = scfg->patnamecontrl;

			temp = scfg->pnamesize;		/* pattern name size */
			temp = (temp <<15) & 0x8000;
			Scl_d_reg.patnamecontrl[0] &= 0x7fff;
			Scl_d_reg.patnamecontrl[0] |= temp;

			Scl_d_reg.patnamecontrl[0] &= 0xbfff;	/* flip bit set */
			Scl_d_reg.patnamecontrl[0] |= flip;	/* flip bit set */

			temp = scfg->platesize;		/* plate size */
			temp &=  0x0003;
			Scl_d_reg.platesize &= 0xfffc;
			Scl_d_reg.platesize |= temp;

			temp = scfg->bmpsize;		/* bitmap size */
			temp = (temp << 2) & 0x000c;
			Scl_d_reg.charcontrl0 &= 0xfff3;
			Scl_d_reg.charcontrl0 |= temp;

			temp = scfg->coltype;		/* color type */
			temp = (temp <<4) & 0x0070;
			Scl_d_reg.charcontrl0 &= 0xff8f;
			Scl_d_reg.charcontrl0 |= temp;

			temp = scfg->datatype;		/* data type */
			temp = (temp <<1) & 0x0002;
			Scl_d_reg.charcontrl0 &= 0xfffd;
			Scl_d_reg.charcontrl0 |= temp;

			Scl_d_reg.mapoffset0 &= 0xfff0;	/* Init map offset */ 
			mapoffset = &Scl_d_reg.mapoffset0; /* map offset */ 
			shift = 0;
			map = &Scl_d_reg.normap[0];
			max = 2;
			break;
		case SCL_NBG1:
			temp = scfg->dispenbl;		/* display enable */
			temp = (temp << 1) & 0x0002;
			Scl_s_reg.dispenbl &= 0xfffD;
			Scl_s_reg.dispenbl |= temp;

			temp = scfg->charsize;		/* char size */
			temp = (temp<<8) & 0x0100;
			Scl_d_reg.charcontrl0 &= 0xfeff;
			Scl_d_reg.charcontrl0 |= temp;

			Scl_d_reg.patnamecontrl[1] = scfg->patnamecontrl;

			temp = scfg->pnamesize;		/* pattern name size */
			temp = (temp <<15) & 0x8000;
			Scl_d_reg.patnamecontrl[1] &= 0x7fff;
			Scl_d_reg.patnamecontrl[1] |= temp;

			Scl_d_reg.patnamecontrl[1] &= 0xbfff;
			Scl_d_reg.patnamecontrl[1] |= flip; /* flip bit set*/

			temp = scfg->platesize;		/* plate size */
			temp = (temp << 2) &  0x000c;
			Scl_d_reg.platesize &= 0xfff3;
			Scl_d_reg.platesize |= temp;

			temp = scfg->bmpsize;		/* bitmap size */
			temp = (temp << 10) & 0x0c00;
			Scl_d_reg.charcontrl0 &= 0xf3ff;
			Scl_d_reg.charcontrl0 |= temp;

			temp = scfg->coltype;		/* color type */
			temp = (temp <<12) & 0x3000;
			Scl_d_reg.charcontrl0 &= 0xcfff;
			Scl_d_reg.charcontrl0 |= temp;

			temp = scfg->datatype;		/* data type */
			temp = (temp <<9) & 0x0200;
			Scl_d_reg.charcontrl0 &= 0xfdff;
			Scl_d_reg.charcontrl0 |= temp;

			Scl_d_reg.mapoffset0 &= 0xff0f;	/* Init map offset */ 
			mapoffset = &Scl_d_reg.mapoffset0; /* map offset */ 
			shift = 4;
			map = &Scl_d_reg.normap[2];
			max = 2;
			break;
		case SCL_NBG2:
			temp = scfg->dispenbl;		/* display enable */
			temp = (temp << 2) & 0x0004;
			Scl_s_reg.dispenbl &= 0xfffb;
			Scl_s_reg.dispenbl |= temp;

			temp = scfg->charsize;		/* char size */
			temp = temp & 0x0001;
			Scl_d_reg.charcontrl1 &= 0xfffe;
			Scl_d_reg.charcontrl1 |= temp;

			Scl_d_reg.patnamecontrl[2] = scfg->patnamecontrl;

			temp = scfg->pnamesize;		/* pattern name size */
			temp = (temp <<15) & 0x8000;
			Scl_d_reg.patnamecontrl[2] &= 0x7fff;
			Scl_d_reg.patnamecontrl[2] |= temp;

			Scl_d_reg.patnamecontrl[2] &= 0xbfff;
			Scl_d_reg.patnamecontrl[2] |= flip; /* flip bit set*/

			temp = scfg->platesize;		/* plate size */
			temp = (temp << 4) &  0x0030;
			Scl_d_reg.platesize &= 0xffcf;
			Scl_d_reg.platesize |= temp;

			temp = scfg->coltype;		/* color type */
			temp = (temp <<1) & 0x0002;
			Scl_d_reg.charcontrl1 &= 0xfffd;
			Scl_d_reg.charcontrl1 |= temp;

			Scl_d_reg.mapoffset0 &= 0xf0ff;	/* Init map offset */ 
			mapoffset = &Scl_d_reg.mapoffset0; /* map offset */ 
			shift = 8;
			map = &Scl_d_reg.normap[4];
			max = 2;
			break;
		case SCL_NBG3:
			temp = scfg->dispenbl;		/* display enable */
			temp = (temp << 3) & 0x0008;
			Scl_s_reg.dispenbl &= 0xfff7;
			Scl_s_reg.dispenbl |= temp;

			temp = scfg->charsize;		/* char size */
			temp = (temp << 4) & 0x0010;
			Scl_d_reg.charcontrl1 &= 0xffef;
			Scl_d_reg.charcontrl1 |= temp;

			Scl_d_reg.patnamecontrl[3] = scfg->patnamecontrl;

			temp = scfg->pnamesize;		/* pattern name size */
			temp = (temp <<15) & 0x8000;
			Scl_d_reg.patnamecontrl[3] &= 0x7fff;
			Scl_d_reg.patnamecontrl[3] |= temp;

			Scl_d_reg.patnamecontrl[3] &= 0xbfff;
			Scl_d_reg.patnamecontrl[3] |= flip; /* flip bit set*/

			temp = scfg->platesize;		/* plate size */
			temp = (temp << 6) &  0x00c0;
			Scl_d_reg.platesize &= 0xff3f;
			Scl_d_reg.platesize |= temp;

			temp = scfg->coltype;		/* color type */
			temp = (temp <<5) & 0x0020;
			Scl_d_reg.charcontrl1 &= 0xffdf;
			Scl_d_reg.charcontrl1 |= temp;

			Scl_d_reg.mapoffset0 &= 0x0fff;	/* Init map offset */ 
			mapoffset = &Scl_d_reg.mapoffset0;	/* map offset */ 
			shift = 12;
			map = &Scl_d_reg.normap[6];
			max = 2;
			break;
		case SCL_RBG0:
			temp = scfg->dispenbl;		/* display enable */
			temp = (temp << 4) & 0x0010;
			Scl_s_reg.dispenbl &= 0xffef;
			Scl_s_reg.dispenbl |= temp;

			temp = scfg->charsize;		/* char size */
			temp = (temp << 8) & 0x0100;
			Scl_d_reg.charcontrl1 &= 0xfeff;
			Scl_d_reg.charcontrl1 |= temp;

			Scl_d_reg.patnamecontrl[4] = scfg->patnamecontrl;

			temp = scfg->pnamesize;		/* pattern name size */
			temp = (temp <<15) & 0x8000;
			Scl_d_reg.patnamecontrl[4] &= 0x7fff;
			Scl_d_reg.patnamecontrl[4] |= temp;

			Scl_d_reg.patnamecontrl[4] &= 0xbfff; /* flip bit set*/
			Scl_d_reg.patnamecontrl[4] |= flip; /* flip bit set*/

			temp = scfg->bmpsize;		/* bitmap size */
			temp = (temp << 10) & 0x0400;
			Scl_d_reg.charcontrl1 &= 0xfbff;
			Scl_d_reg.charcontrl1 |= temp;

			temp = scfg->coltype;		/* color type */
			temp = (temp <<12) & 0x7000;
			Scl_d_reg.charcontrl1 &= 0x8fff;
			Scl_d_reg.charcontrl1 |= temp;

			temp = scfg->datatype;		/* data type */
			temp = (temp <<9) & 0x0200;
			Scl_d_reg.charcontrl1 &= 0xfdff;
			Scl_d_reg.charcontrl1 |= temp;

			max = 8;

			if((SclRa == SCL_RBG0) && (SclRb == SCL_RBG0)) {
			    Scl_d_reg.mapoffset1 &= 0xfff0;    /* Init map offset */ 
			    mapoffset = &Scl_d_reg.mapoffset1; /* map offset */ 
			    shift  = 0;
			    map    = &Scl_d_reg.rotmap[0];
			    shift2 = 4;
			    map2   = &Scl_d_reg.rotmap[8];

			    temp = scfg->platesize;		/* plate size */
			    temp = ((temp << 8) &  0x0300) | ((temp << 12) &  0x3000);
			    Scl_d_reg.platesize &= 0xfcff;
			    Scl_d_reg.platesize |= temp;

			    temp = scfg->mapover;/* map over */
			    temp = ((temp << 10) &  0x0c00) | ((temp << 14) &  0xc000);
			    Scl_d_reg.platesize &= 0x33ff;
			    Scl_d_reg.platesize |= temp;
			} else {
			    if(SclRa == SCL_RBG0)
			    {
			        Scl_d_reg.mapoffset1 &= 0xfff0;	/* Init map offset */ 
				mapoffset = &Scl_d_reg.mapoffset1;
				shift  = 0;
				map    = &Scl_d_reg.rotmap[0];

				temp = scfg->platesize;		/* plate size */
				temp = (temp << 8) &  0x0300;
				Scl_d_reg.platesize &= 0xfcff;
				Scl_d_reg.platesize |= temp;

				temp = scfg->mapover;/* map over */
				temp = (temp << 10) &  0x0c00;
				Scl_d_reg.platesize &= 0xf3ff;
				Scl_d_reg.platesize |= temp;
			    } else if(SclRb == SCL_RBG0) {
			        Scl_d_reg.mapoffset1 &= 0xff0f;	/* Init map offset */ 
				mapoffset = &Scl_d_reg.mapoffset1;
				shift  = 4;
				map    = &Scl_d_reg.rotmap[8];

				temp = scfg->platesize;	/* plate size */
				temp = (temp << 12) &  0x3000;
				Scl_d_reg.platesize &= 0xcfff;
				Scl_d_reg.platesize |= temp;

				temp = scfg->mapover;/* map over */
				temp = (temp << 14) &  0xc000;
				Scl_d_reg.platesize &= 0x3fff;
				Scl_d_reg.platesize |= temp;
			    } else
					return;
			}
			break;
		case SCL_RBG1:
		   if( (SclRa == SCL_RBG0) && (SclRb == SCL_RBG1) ) {
			Scl_s_reg.dispenbl &= 0xffdf;
			if(scfg->dispenbl)	/* display enable */
				Scl_s_reg.dispenbl |= 0x0020;

			temp = scfg->charsize;		/* char size */
			temp &= 0x0001;
			Scl_d_reg.charcontrl0 &= 0xfffe;
			Scl_d_reg.charcontrl0 |= temp;

			Scl_d_reg.patnamecontrl[0] = scfg->patnamecontrl;

			temp = scfg->pnamesize;		/* pattern name size */
			temp = (temp <<15) & 0x8000;
			Scl_d_reg.patnamecontrl[0] &= 0x7fff;
			Scl_d_reg.patnamecontrl[0] |= temp;

			Scl_d_reg.patnamecontrl[0] &= 0xbfff;
			Scl_d_reg.patnamecontrl[0] |= flip;
							/* flip bit set */

			temp = scfg->platesize;	/* plate size */
			temp = (temp << 12) &  0x3000;
			Scl_d_reg.platesize &= 0xcfff;
			Scl_d_reg.platesize |= temp;

			temp = scfg->mapover;/* map over */
			temp = (temp << 14) &  0xc000;
			Scl_d_reg.platesize &= 0x3fff;
			Scl_d_reg.platesize |= temp;

			temp = scfg->coltype;		/* color type */
			temp = (temp <<4) & 0x0070;
			Scl_d_reg.charcontrl0 &= 0xff8f;
			Scl_d_reg.charcontrl0 |= temp;

			temp = SCL_CELL;		/* data type */
			temp = (temp <<1) & 0x0002;
			Scl_d_reg.charcontrl0 &= 0xfffd;
			Scl_d_reg.charcontrl0 |= temp;

			max = 8;
			Scl_d_reg.mapoffset1 &= 0xff0f;	/* Init map offset */
			mapoffset = &Scl_d_reg.mapoffset1;
			shift  = 4;
			map    = &Scl_d_reg.rotmap[8];

		    }
		    break;
	}
/*
 *	Set Map Address
 */
	if(scfg->datatype == SCL_BITMAP) {
		mapoffsetW = ((scfg->plate_addr[0] - SCL_VDP2_VRAM)/0x20000) & 0x0007;
		*mapoffset |= mapoffsetW << shift;
	} else {
		if( scfg->pnamesize == 1){
			if(scfg->charsize == 0) {
				boundary = 0x2000;
			} else {
				boundary = 0x800;
			}
		} else {
			if(scfg->charsize == 0) {
				boundary = 0x4000;
			} else {
				boundary = 0x1000;
			}
		}
		mapoffsetW = (0x01c0 & ((scfg->plate_addr[0] - SCL_VDP2_VRAM) / boundary))
			>> 6;
		*mapoffset |= mapoffsetW << shift;
	}

	for(i = 0; i < max; i++) {
		map[i] = (0x003f & ((scfg->plate_addr[i * 2] - SCL_VDP2_VRAM)
				/ boundary));
		temp = (0x003f & ((scfg->plate_addr[i * 2 + 1] - SCL_VDP2_VRAM)
				/ boundary)) << 8;

		map[i] |= (temp & 0x3f00);
	}
	if(map2) {
	    *mapoffset &= 0x000f;
	    *mapoffset |= *mapoffset << shift2;
	    for(i = 0; i < max; i++) {
		map2[i] = (0x003f & ((scfg->plate_addr[i * 2] - SCL_VDP2_VRAM)
				/ boundary));
		temp = (0x003f & ((scfg->plate_addr[i * 2 + 1] - SCL_VDP2_VRAM)
				/ boundary)) << 8;

		map2[i] |= (temp & 0x3f00);
	    }
	}
	if(SclProcess == 0)	SclProcess = 1;
}
