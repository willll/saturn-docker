/*------------------------------------------------------------------------
 *  FILE:	SCL_Fu05.c
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
 *	Written by K.M on 1994-06-22 Ver.1.00
 *	Updated by K.M on 1994-06-22 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 


/* ���W�X�^�o�b�t�@ */
extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;

/* ���C���X�N���[���A�Z���X�N���[���֘A */
extern	Uint32	SclAddrLsTbl[];
extern	Uint32	SclAddrCsTbl[];
extern	Uint16	SclLengthLsTbl;
extern	Uint16	SclLengthCsTbl;

static	Fixed32	ls_tbl[3 * SCL_MAXLINE];
static	Fixed32	cs_tbl[SCL_MAXCELL];

extern	Uint16	SclDisplayX;
extern	Uint16	SclDisplayY;

extern	Uint16	SclProcess;
extern	Uint32	SclCurSclNum;

/*------------------------------------------------------------------------
 *
 * NAME : SCL_InitLineParamTb
 *
 * PARAMETERS
 *	param1 -	pointer to line scroll parameter
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
void SCL_InitLineParamTb(SclLineparam *lp)
{
	Uint16	i;

	lp->h_enbl = OFF;
	lp->v_enbl = OFF;
	lp->delta_enbl = OFF;
	lp->cell_enbl = OFF;
	lp->line_addr = 0x00;
	lp->cell_addr = 0x00;
	lp->interval  = 0;

	for(i = 0; i< SCL_MAXLINE; i++) {
		if(i < SCL_MAXCELL) {
			lp->cell_tbl[i] = 0;
		}
		lp->line_tbl[i].h  = FIXED(0);
		lp->line_tbl[i].v  = FIXED(i);
		lp->line_tbl[i].dh = FIXED(1);
	}
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_SetLineParam
 *
 * PARAMETERS
 *	param1 -	pointer to line scroll parameter
 *	param2 -
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
void SCL_SetLineParam(SclLineparam *lp)
{
	Uint16	step = 0;
	Uint16	temp, i, k;
	Uint32	*addr, *addr1;
	if((SclCurSclNum !=SCL_NBG0) & (SclCurSclNum != SCL_NBG1))
		return;
	temp = lp->cell_enbl & 0x0001;
	temp |= (lp->h_enbl << 1) & 0x0002;
	temp |= (lp->v_enbl << 2) & 0x0004;
	temp |= (lp->delta_enbl << 3) & 0x0008;
	temp |= (lp->interval<< 4) & 0x0030;
	addr = &Scl_n_reg.lineaddr[0];
	addr1 = &Scl_n_reg.celladdr;
	if(SclCurSclNum == SCL_NBG1) {
		temp = (temp <<8) & 0xff00;
		addr = &Scl_n_reg.lineaddr[1];
	}
	Scl_n_reg.linecontrl = temp;
	SclAddrLsTbl[0] = lp->line_addr;
	SclAddrLsTbl[1] = (Uint32 )&ls_tbl[0];
	SclAddrCsTbl[0] = lp->cell_addr;
	SclAddrCsTbl[1] = (Uint32 )&cs_tbl[0];
	*addr = (lp->line_addr / 2) & 0x0007ffff;
	*addr1 = (lp->cell_addr / 2) & 0x0007ffff;
/*
 *	Set Line Scroll Table Data 
 */
	if(lp->interval == 0)
		step = 1;
	if(lp->interval == 1)
		step = 2;
	if(lp->interval == 2)
		step = 4;
	if(lp->interval == 3)
		step = 8;
	k = 0;
	for(i = 0; i < SCL_MAXLINE; i += step) {
		if(lp->h_enbl)
			ls_tbl[k++] = lp->line_tbl[i].h;
		if(lp->v_enbl)
			ls_tbl[k++] = lp->line_tbl[i].v;
		if(lp->delta_enbl)
			ls_tbl[k++] = lp->line_tbl[i].dh;
	}
	SclLengthLsTbl = k;
	k = 0;
	for(i = 0; i < SCL_MAXCELL; i += 1) {
		if(lp->cell_enbl)
			cs_tbl[k++] = lp->cell_tbl[i];
	}
	SclLengthCsTbl = k;
	SclProcess = 2;
}
