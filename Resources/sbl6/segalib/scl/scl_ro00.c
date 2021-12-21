/*------------------------------------------------------------------------
 *  FILE:	SCL_Ro00.c
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
 *	Updated by K.M on 1994-06-30 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#define SEGA_SCL_PROTO

#include <sega_scl.h> 


	Fixed32	SclCsx[2],SclCsy[2];

extern	Uint32	SclCurSclNum;
extern	Uint16	SclProcess;

/* 画面サイズデフォルト */
extern	Uint16	SclDisplayX;
extern	Uint16	SclDisplayY;

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;

/*******************************************************
 *  回転マトリックスパラメータテーブルバッファ         *
 *******************************************************/
extern	SclRotreg	SclRotregBuff[2];

/*******************************************************
 *  回転マトリックスパラメータ係数テーブルバッファ     *
 *******************************************************/
extern	Uint16	SclK_TableBuff[2][820];
extern	Uint32	SclK_TableNum[];
extern	Uint16	SclK_TableFlag[];

extern	Uint32	SclRbgKtbAddr[];
extern	Sint32	SclRbgKtbOffset[];
extern	Uint16	SclRotXySw[];
extern	Uint8	SclRa,SclRb;
extern	Uint32	SclRotateTableAddress;

/* 回転パラメータ用角度データ */
extern	Fixed32	SclRotateXy[];
extern	Fixed32	SclRotateZ[];
extern	Fixed32	SclRotateDisp[];
extern	Fixed32	SclRotateMoveZ[];

extern	Uint16	SclRPMD;	/* 回転パラメータモード */

extern	Uint16	SclRotateTableMode;

/* trig functions */
extern	Fixed32	fsin(Fixed32);
extern	Fixed32	fcos(Fixed32);

static	Fixed32	currentMatrix[2][9];


	void	SCL_Rotate(Fixed32 xy,Fixed32 z,Fixed32 disp);
	void	SCL_RotateZD(Fixed32 dAngle);

static	void	SCL_RotateX(Fixed32 xAngle);
static	void	SCL_RotateY(Fixed32 yAngle);
static	void	SCL_RotateZ(Fixed32 zAngle);
	void	SCL_RotateZD(Fixed32 dAngle);


static	Fixed32	Fsin(Fixed32	a)
{
	Uint16	sign;
	Fixed32	val;

	sign = 0;
	if(a & 0x80000000){
		sign = 1;
		a = - a;
	}
	a = a % FIXED(360); 	/* 0 <= a < 360  */

	if(FIXED(270) <= a){	/*  270 <= a < 360  */
		val = -fsin(FIXED(360) - a);
	}else if(FIXED(180) <= a){	/*  180 <= a < 270  */
		val = -fsin(a - FIXED(180));
	}else if(FIXED(90) <= a){	/*  90 <= a < 180  */
		val = fsin(FIXED(180) - a);
	}else{			/* 0 <= a < 90 */
		val = fsin(a);
	}

	if(val==0x0000ffff)	val = FIXED(1);

	if(sign)	val = -val;
	return(val);
}

static	Fixed32	Fcos(Fixed32	a)
{
	Uint16	sign;
	Fixed32	val;

	sign = 0;
	if(a & 0x80000000){
		sign = 0;
		a = - a;
	}
	a = a % FIXED(360);		/* 0 <= a < 360  */

	if(a == 0)	return( FIXED(1) );

	if(FIXED(270) <= a){		/*  270 <= a < 360  */
		val = fcos(FIXED(360) - a);
	}else if(FIXED(180) < a){	/*  180 <  a < 270  */
		val = -fcos(a - FIXED(180));
	}else if(FIXED(180) == a){	/*  a == 180  */
		val = - FIXED(1);
	}else if(FIXED(90) <= a){	/*  90 <= a < 180  */
		val = -fcos(FIXED(180) - a);
	}else{				/* 0 <= a < 90 */
		val = fcos(a);
	}
	return(val);
}

/*****************************************************************************
 * 回転パラメータテーブルの大きさ及びＶＲＡＭ上のどこに置くかを指定する
 *****************************************************************************/
Uint32 SCL_InitRotateTable(Uint32 Address,Uint16 Mode,Uint32 rA,Uint32 rB)
{
    Uint32  	AddressW;
    Fixed32	r;

    if( (rA != SCL_RBG0) && (rA != SCL_NON) && !((rA == SCL_SPR) && (rB == SCL_RBG0)) )
	return(2);
    if( (rB != SCL_RBG0) && (rB != SCL_RBG1) && (rB != SCL_NON) )
	return(2);

    SclRa = rA;
    SclRb = rB;

    if( (rA == SCL_RBG0) && (rB == SCL_RBG0) )		SclRPMD = 2;
    else if( (rA == SCL_NON) && (rB == SCL_RBG0) )	SclRPMD = 1;
/*    else if( (rA == SCL_SPR) && (rB == SCL_RBG0) )	SclRPMD = 3;*/
    else						SclRPMD = 0;

    Scl_r_reg.paramode = SclRPMD;/* Rotation parameters mode bit */


    r=FIXED(0);

    SclRotateDisp[0]=FIXED(0);
    SclRotateDisp[1]=FIXED(0);

    SclRotateXy[0] = FIXED(0);
    SclRotateZ[0]  = FIXED(0);
    SclRotateXy[1] = FIXED(0);
    SclRotateZ[1]  = FIXED(0);

    SclCsx[0] = FIXED( SclDisplayX/2 );
    SclCsy[0] = FIXED( SclDisplayY/2 );

    SclCsx[1] = SclCsx[0];
    SclCsy[1] = SclCsy[0];

    SclRotateTableAddress = Address;

    SclRotateTableMode = Mode;

    SclRotregBuff[0].screenst.x  
	= SclCsx[0] - MUL_FIXED(Fcos(r),SclCsx[0])-MUL_FIXED((-Fsin(r)),SclCsy[0]);
    SclRotregBuff[0].screenst.y  
	= SclCsy[0] - MUL_FIXED(Fsin(r),SclCsx[0])-MUL_FIXED((Fcos(r)),SclCsy[0]);
    SclRotregBuff[0].screenst.z  =  FIXED(0);
    SclRotregBuff[0].screendlt.x = -Fsin(r);
    SclRotregBuff[0].screendlt.y =  Fcos(r);
    SclRotregBuff[0].delta.x     =  Fcos(r);
    SclRotregBuff[0].delta.y     =  Fsin(r);

    SclRotregBuff[0].matrix_a    = FIXED(1);
    SclRotregBuff[0].matrix_b    = FIXED(0);
    SclRotregBuff[0].matrix_c    = FIXED(0);
    SclRotregBuff[0].matrix_d    = FIXED(0);
    SclRotregBuff[0].matrix_e    = FIXED(1);
    SclRotregBuff[0].matrix_f    = FIXED(0);

    SclRotregBuff[0].viewp.x     = SclDisplayX/2;
    SclRotregBuff[0].viewp.y     = SclDisplayY/2;
    SclRotregBuff[0].viewp.z     = 400;

    SclRotregBuff[0].rotatecenter.x = SclDisplayX/2;
    SclRotregBuff[0].rotatecenter.y = SclDisplayY/2;
    SclRotregBuff[0].rotatecenter.z = 0;

    SclRotregBuff[0].move.x      = FIXED(0);
    SclRotregBuff[0].move.y      = FIXED(0);
    SclRotregBuff[0].zoom.x      = FIXED(1);
    SclRotregBuff[0].zoom.y      = FIXED(1);

    if(Mode == 2)
    {
	SclRotregBuff[1].screenst.x  = SclRotregBuff[0].screenst.x;
	SclRotregBuff[1].screenst.y  = SclRotregBuff[0].screenst.y;
	SclRotregBuff[1].screenst.z  = SclRotregBuff[0].screenst.z;
	SclRotregBuff[1].screendlt.x = SclRotregBuff[0].screendlt.x;
	SclRotregBuff[1].screendlt.y = SclRotregBuff[0].screendlt.y;
	SclRotregBuff[1].delta.x     = SclRotregBuff[0].delta.x;
	SclRotregBuff[1].delta.y     = SclRotregBuff[0].delta.y;

	SclRotregBuff[1].matrix_a    = SclRotregBuff[0].matrix_a;
	SclRotregBuff[1].matrix_b    = SclRotregBuff[0].matrix_b;
	SclRotregBuff[1].matrix_c    = SclRotregBuff[0].matrix_c;
	SclRotregBuff[1].matrix_d    = SclRotregBuff[0].matrix_d;
	SclRotregBuff[1].matrix_e    = SclRotregBuff[0].matrix_e;
	SclRotregBuff[1].matrix_f    = SclRotregBuff[0].matrix_f;

	SclRotregBuff[1].viewp.x     = SclRotregBuff[0].viewp.x;
	SclRotregBuff[1].viewp.y     = SclRotregBuff[0].viewp.y;
	SclRotregBuff[1].viewp.z     = SclRotregBuff[0].viewp.z;

	SclRotregBuff[1].rotatecenter.x = SclRotregBuff[0].rotatecenter.x;
	SclRotregBuff[1].rotatecenter.y = SclRotregBuff[0].rotatecenter.y;
	SclRotregBuff[1].rotatecenter.z = SclRotregBuff[0].rotatecenter.z;

	SclRotregBuff[1].move.x      = SclRotregBuff[0].move.x;
	SclRotregBuff[1].move.y      = SclRotregBuff[0].move.y;
	SclRotregBuff[1].zoom.x      = SclRotregBuff[0].zoom.x;
	SclRotregBuff[1].zoom.y      = SclRotregBuff[0].zoom.y;
    }

    /* アドレスデータをレジスタにセットする形式に変換 */
    /* 詳しくは、VDP2ﾕｰｻﾞｰｽﾞﾏﾆｭｱﾙ P158 ｢回転ﾊﾟﾗﾒｰﾀﾃｰﾌﾞﾙｱﾄﾞﾚｽﾚｼﾞｽﾀ｣を参照 */
    AddressW = ((Address & 0x0007ff80)>>1) + (Address & 0x0000003e)/4;
    Scl_r_reg.paramaddr = AddressW;

    return(0);
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_Rotate
 *
 * PARAMETERS
 *	param1 - XorY Angle
 *	param2 - Z Angle
 *	param3 - Display(Z) Angle
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
void   SCL_Rotate(Fixed32 xy,Fixed32 z,Fixed32 disp)
{
	Uint16	TbNum;

	switch(SclCurSclNum) {
		case SCL_RBG_TB_A:
			TbNum = 0;
			break;
		case SCL_RBG_TB_B:
			TbNum = 1;
			break;
		default:
			return;
			break;
	}

	currentMatrix[TbNum][0] = FIXED(1);
	currentMatrix[TbNum][1] = FIXED(0);
	currentMatrix[TbNum][2] = FIXED(0);
	currentMatrix[TbNum][3] = FIXED(0);
	currentMatrix[TbNum][4] = FIXED(1);
	currentMatrix[TbNum][5] = FIXED(0);
	currentMatrix[TbNum][6] = FIXED(0);
	currentMatrix[TbNum][7] = FIXED(0);
	currentMatrix[TbNum][8] = FIXED(1);

	if(SclRbgKtbAddr[TbNum]) {
		SclRotateXy[TbNum] += xy;
	}else {
		SclRotateXy[TbNum] = 0;
	}

	SclRotateZ[TbNum]  += z;

	if(!SclRotateXy[TbNum] && xy) {
		/* xyを加算した結果角度が０度になった場合 */
		switch(SclCurSclNum) {
			case SCL_RBG_TB_A:
				Scl_r_reg.k_contrl &= 0xff00;
				break;
			case SCL_RBG_TB_B:
				Scl_r_reg.k_contrl &= 0x00ff;
				break;
		}
	}

	/* 画面回転 */
	if(disp)	SCL_RotateZD(disp);

	if(SclRbgKtbOffset[TbNum] && SclRotateDisp[TbNum]) {
		SclRotateXy[TbNum] = 0;
	}

	/* マトリックスによる倍率設定 */
	if(SclRotateXy[TbNum] || SclRotateMoveZ[TbNum]) {
		currentMatrix[TbNum][0] = SclRotregBuff[TbNum].zoom.x;
		currentMatrix[TbNum][4] = SclRotregBuff[TbNum].zoom.y;
	} else if( (TbNum==0 && (Scl_r_reg.k_contrl & 0x00ff) )
	    		|| (TbNum==1 && (Scl_r_reg.k_contrl & 0xff00)) ) {
		currentMatrix[TbNum][0] = SclRotregBuff[TbNum].zoom.x;
		currentMatrix[TbNum][4] = SclRotregBuff[TbNum].zoom.y;
		SclRotregBuff[TbNum].matrix_a  = SclRotregBuff[TbNum].zoom.x;
		SclRotregBuff[TbNum].matrix_e  = SclRotregBuff[TbNum].zoom.y;
	}

	if(SclRotateZ[TbNum])	SCL_RotateZ(SclRotateZ[TbNum]);

	if( SclRotateXy[TbNum] || SclRotateMoveZ[TbNum] ) {
		if(SclRotXySw[TbNum]==0 || SclRbgKtbOffset[TbNum])
			SCL_RotateX(SclRotateXy[TbNum]);
		else	SCL_RotateY(SclRotateXy[TbNum]);
	}
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_RotateX
 *
 * PARAMETERS
 *	param1 - 
 *	param2 - 
 *
 * DESCRIPTION
 *
 * | M00 M01 M02 | | 1    0    0 |
 * | M10 M11 M12 |.| 0  cos -sin |
 * | M20 M21 M22 | | 0  sin  cos |
 *
 *		         | M00 M01*cos+M02*sin -M01*sin+M02*cos |
 *		       = | M10 M11*cos+M12*sin -M11*sin+M12*cos |
 *		         | M20 M21*cos+M22*sin -M21*sin+M22*cos |
 *
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 *
 * CAVEATS
 * 
 *------------------------------------------------------------------------
 */
static	void  SCL_RotateX(Fixed32 xAngle)
{
    Fixed32	sinNum, cosNum;
    Fixed32	M01, M02, M11, M12, M21, M22;
    Fixed32	Zp,Zs_Zp,Zs_ZpW;
    Sint32	i;
    Uint16	TbNum;

    switch(SclCurSclNum)
    {
	case SCL_RBG_TB_A:
		TbNum = 0;
		Scl_r_reg.k_contrl &= 0xff00;
		Scl_r_reg.k_contrl |= 0x0003;
		break;
	case SCL_RBG_TB_B:
		TbNum = 1;
		Scl_r_reg.k_contrl &= 0x00ff;
		Scl_r_reg.k_contrl |= 0x0300;
		break;
	default:
		return;
		break;
    }

    sinNum = Fsin(xAngle);
    cosNum = Fcos(xAngle);
    M01 = currentMatrix[TbNum][1];
    M02 = currentMatrix[TbNum][2];
    M11 = currentMatrix[TbNum][3+1];
    M12 = currentMatrix[TbNum][3+2];
    M21 = currentMatrix[TbNum][6+1];
    M22 = currentMatrix[TbNum][6+2];
    currentMatrix[TbNum][1]   =  MUL_FIXED(M01, cosNum) + MUL_FIXED(M02, sinNum);
    currentMatrix[TbNum][2]   = -MUL_FIXED(M01, sinNum) + MUL_FIXED(M02, cosNum);
    currentMatrix[TbNum][3+1] =  MUL_FIXED(M11, cosNum) + MUL_FIXED(M12, sinNum);
    currentMatrix[TbNum][3+2] = -MUL_FIXED(M11, sinNum) + MUL_FIXED(M12, cosNum);
    currentMatrix[TbNum][6+1] =  MUL_FIXED(M21, cosNum) + MUL_FIXED(M22, sinNum);
    currentMatrix[TbNum][6+2] = -MUL_FIXED(M21, sinNum) + MUL_FIXED(M22, cosNum);

    SclRotregBuff[TbNum].matrix_a = currentMatrix[TbNum][0];/* 拡大縮小の為 */
    SclRotregBuff[TbNum].matrix_b = currentMatrix[TbNum][1];
    SclRotregBuff[TbNum].matrix_c = currentMatrix[TbNum][2];
    SclRotregBuff[TbNum].matrix_e = currentMatrix[TbNum][3+1];
    SclRotregBuff[TbNum].matrix_f = currentMatrix[TbNum][3+2];

    Zp = currentMatrix[TbNum][6+0] 
	    * (SclRotregBuff[TbNum].viewp.x - SclRotregBuff[TbNum].rotatecenter.x)
	+ currentMatrix[TbNum][6+1] 
	    * (SclRotregBuff[TbNum].viewp.y - SclRotregBuff[TbNum].rotatecenter.y)
	+ currentMatrix[TbNum][6+2] 
	    * (SclRotregBuff[TbNum].viewp.z - SclRotregBuff[TbNum].rotatecenter.z)
	+ FIXED(SclRotregBuff[TbNum].rotatecenter.z) + SclRotateMoveZ[TbNum];

    Zs_ZpW = currentMatrix[TbNum][6+0] * (- SclRotregBuff[TbNum].viewp.x)
	   + currentMatrix[TbNum][6+2] * (- SclRotregBuff[TbNum].viewp.z);

    if(SclRotateDisp[TbNum])
    {
	SclRotregBuff[TbNum].k_tab     = FIXED(300)+SclRotregBuff[TbNum].screenst.y;
	SclRotregBuff[TbNum].k_delta.x = Fcos(SclRotateDisp[TbNum]);
	SclRotregBuff[TbNum].k_delta.y = Fsin(SclRotateDisp[TbNum]);
	SclK_TableNum[TbNum] = 819;

	for(i=0;i < SclK_TableNum[TbNum];i++)
	{
	    Zs_Zp = currentMatrix[TbNum][6+1] * (i - 300  - SclRotregBuff[TbNum].viewp.y)
	    		+ Zs_ZpW;

	    if(Zs_Zp / 256)
	    {
		if( (- DIV_FIXED(Zp,Zs_Zp) > FIXED(4) ) || (- DIV_FIXED(Zp,Zs_Zp) < 0) )
		    SclK_TableBuff[TbNum][i] = 0x8000;
		else
		    SclK_TableBuff[TbNum][i] = ((Uint16)((- DIV_FIXED(Zp,Zs_Zp)) >> 6)) & 0x7fff;
	    }
	    else	SclK_TableBuff[TbNum][i] = 0x8000;
	}
    } else {
	if(SclRbgKtbOffset[TbNum]!=-1 && SclRbgKtbOffset[TbNum])
	    SclRotregBuff[TbNum].k_tab     = SclRotregBuff[TbNum].screenst.y + FIXED(SclRbgKtbOffset[TbNum]/2);
	else
	    SclRotregBuff[TbNum].k_tab     = FIXED(0)+SclRotregBuff[TbNum].screenst.y;
	SclRotregBuff[TbNum].k_delta.x = Fcos(FIXED(0));
	SclRotregBuff[TbNum].k_delta.y = Fsin(FIXED(0));
	SclK_TableNum[TbNum] = SclDisplayY;

	for(i=0;i < SclK_TableNum[TbNum];i++)
	{
	    Zs_Zp = currentMatrix[TbNum][6+1] * (i - SclRotregBuff[TbNum].viewp.y) + Zs_ZpW;

	    if(Zs_Zp / 256)
	    {
		if( (- DIV_FIXED(Zp,Zs_Zp) > FIXED(4) ) || (- DIV_FIXED(Zp,Zs_Zp) < 0) )
		    SclK_TableBuff[TbNum][i] = 0x8000;
		else
		    SclK_TableBuff[TbNum][i] = ((Uint16)((- DIV_FIXED(Zp,Zs_Zp)) >> 6)) & 0x7fff;
	    }
	    else	SclK_TableBuff[TbNum][i] = 0x8000;
	}
    }

    SclK_TableFlag[TbNum] = ON;

    return;
}

/*------------------------------------------------------------------------
 *
 * NAME : SCL_RotateY
 *
 * PARAMETERS
 *	param1 - 
 *	param2 -
 *
 * DESCRIPTION
 *
 * | M00 M01 M02 | |  cos 0  sin |
 * | M10 M11 M12 |.|    0 1    0 |
 * | M20 M21 M22 | | -sin 0  cos |
 *
 *			|  M00*cos-M02*sin  M01 M00*sin+M02*cos |
 *		    =	|  M10*cos-M12*sin  M11 M10*sin+M12*cos |
 *			|  M20*cos-M22*sin  M21 M20*sin+M22*cos |
 *
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 * 
 * CAVEATS
 *
 *------------------------------------------------------------------------
 */
static	void  SCL_RotateY(Fixed32 yAngle)
{
    Fixed32	sinNum, cosNum;
    Fixed32	M00, M02, M10, M12, M20, M22;
    Fixed32	Zp,Zs_Zp,Zs_ZpW;
    Fixed32	i;
    Uint16	TbNum;

    switch(SclCurSclNum)
    {
	case SCL_RBG_TB_A:
		TbNum = 0;
		Scl_r_reg.k_contrl &= 0xff00;
		Scl_r_reg.k_contrl |= 0x0003;
		break;
	case SCL_RBG_TB_B:
		TbNum = 1;
		Scl_r_reg.k_contrl &= 0x00ff;
		Scl_r_reg.k_contrl |= 0x0300;
		break;
	default:
		return;
		break;
    }

    sinNum = Fsin(yAngle);
    cosNum = Fcos(yAngle);
    M00 = currentMatrix[TbNum][0+0];
    M02 = currentMatrix[TbNum][0+2];
    M10 = currentMatrix[TbNum][3+0];
    M12 = currentMatrix[TbNum][3+2];
    M20 = currentMatrix[TbNum][6+0];
    M22 = currentMatrix[TbNum][6+2];
    currentMatrix[TbNum][0+0] = MUL_FIXED(M00, cosNum) - MUL_FIXED(M02, sinNum);
    currentMatrix[TbNum][0+2] = MUL_FIXED(M00, sinNum) + MUL_FIXED(M02, cosNum);
    currentMatrix[TbNum][3+0] = MUL_FIXED(M10, cosNum) - MUL_FIXED(M12, sinNum);
    currentMatrix[TbNum][3+2] = MUL_FIXED(M10, sinNum) + MUL_FIXED(M12, cosNum);
    currentMatrix[TbNum][6+0] = MUL_FIXED(M20, cosNum) - MUL_FIXED(M22, sinNum);
    currentMatrix[TbNum][6+2] = MUL_FIXED(M20, sinNum) + MUL_FIXED(M22, cosNum);

    SclRotregBuff[TbNum].matrix_a = currentMatrix[TbNum][0];
    SclRotregBuff[TbNum].matrix_c = currentMatrix[TbNum][2];
    SclRotregBuff[TbNum].matrix_d = currentMatrix[TbNum][3+0];
    SclRotregBuff[TbNum].matrix_e = currentMatrix[TbNum][3+1];
    SclRotregBuff[TbNum].matrix_f = currentMatrix[TbNum][3+2];

    Zp = currentMatrix[TbNum][6+0]
	    * (SclRotregBuff[TbNum].viewp.x - SclRotregBuff[TbNum].rotatecenter.x)
	+ currentMatrix[TbNum][6+1]
	    * (SclRotregBuff[TbNum].viewp.y - SclRotregBuff[TbNum].rotatecenter.y)
	+ currentMatrix[TbNum][6+2]
	    * (SclRotregBuff[TbNum].viewp.z - SclRotregBuff[TbNum].rotatecenter.z)
	+ FIXED(SclRotregBuff[TbNum].rotatecenter.z) + SclRotateMoveZ[TbNum];

    Zs_ZpW = currentMatrix[TbNum][6+1] * (- SclRotregBuff[TbNum].viewp.y)
	   + currentMatrix[TbNum][6+2] * (- SclRotregBuff[TbNum].viewp.z);


    if(SclRotateDisp[TbNum])
    {
	SclRotregBuff[TbNum].k_tab     = FIXED(300)+SclRotregBuff[TbNum].screenst.x;
	SclRotregBuff[TbNum].k_delta.x = Fcos(SclRotateDisp[TbNum]+FIXED(90) );
	SclRotregBuff[TbNum].k_delta.y = Fsin(SclRotateDisp[TbNum]+FIXED(90) );
	SclK_TableNum[TbNum] = 819;

	for(i=0;i < SclK_TableNum[TbNum];i++)
	{
	    Zs_Zp = currentMatrix[TbNum][6+0] * (i - 300 - SclRotregBuff[TbNum].viewp.x) + Zs_ZpW;
	    if(Zs_Zp / 256)
	    {
		if( (- DIV_FIXED(Zp,Zs_Zp) > FIXED(4)) | (- DIV_FIXED(Zp,Zs_Zp) < 0) )
		    SclK_TableBuff[TbNum][i] = 0x8000;
		else
		    SclK_TableBuff[TbNum][i] = ((Uint16)((- DIV_FIXED(Zp,Zs_Zp)) >> 6)) & 0x7fff;
	    }
	    else	SclK_TableBuff[TbNum][i] = 0x8000;
	}
    } else {
	SclRotregBuff[TbNum].k_tab     = FIXED(0)+SclRotregBuff[TbNum].screenst.x;
	SclRotregBuff[TbNum].k_delta.x = Fcos(FIXED(90));
	SclRotregBuff[TbNum].k_delta.y = Fsin(FIXED(90));
	SclK_TableNum[TbNum] = SclDisplayX;

	for(i=0;i < SclK_TableNum[TbNum];i++)
	{
	    Zs_Zp = currentMatrix[TbNum][6+0] * (i - SclRotregBuff[TbNum].viewp.x) + Zs_ZpW;
	    if(Zs_Zp / 256)
	    {
		if( (- DIV_FIXED(Zp,Zs_Zp) > FIXED(4)) | (- DIV_FIXED(Zp,Zs_Zp) < 0) )
		    SclK_TableBuff[TbNum][i] = 0x8000;
		else
		    SclK_TableBuff[TbNum][i] = ((Uint16)((- DIV_FIXED(Zp,Zs_Zp)) >> 6)) & 0x7fff;
	    }
	    else	SclK_TableBuff[TbNum][i] = 0x8000;
	}
    }

    SclK_TableFlag[TbNum] = ON;

    return;
}


/*------------------------------------------------------------------------
 *
 * NAME : SCL_RotateZ
 *
 * PARAMETERS
 *	param1 - 
 *	param2 -
 *
 * DESCRIPTION
 *
 * | M00 M01 M02 | | cos -sin  0 |
 * | M10 M11 M12 |.| sin  cos  0 |
 * | M20 M21 M22 | |   0    0  1 |
 *
 *				| M00*cos+M01*sin -M00*sin+M01*cos M02 |
 *			      =	| M10*cos+M11*sin -M10*sin+M11*cos M12 |
 *				| M20*cos+M21*sin -M20*sin+M21*cos M22 |
 *
 * PRECONDITIONS
 *
 * POSTCONDITIONS
 * 
 * CAVEATS
 * 
 *------------------------------------------------------------------------
 */
static	void  SCL_RotateZ(Fixed32 zAngle)
{
    Fixed32	sinNum, cosNum;
    Fixed32	M00, M01, M10, M11, M20, M21;
    Uint16	TbNum;

    switch(SclCurSclNum)
    {
	case SCL_RBG_TB_A:
		TbNum = 0;
		break;
	case SCL_RBG_TB_B:
		TbNum = 1;
		break;
	default:
		return;
		break;
    }

    sinNum = Fsin(zAngle);
    cosNum = Fcos(zAngle);
    M00 = currentMatrix[TbNum][0];
    M01 = currentMatrix[TbNum][1];
    M10 = currentMatrix[TbNum][3+0];
    M11 = currentMatrix[TbNum][3+1];
    M20 = currentMatrix[TbNum][6+0];
    M21 = currentMatrix[TbNum][6+1];
    currentMatrix[TbNum][0]   =  MUL_FIXED(M00, cosNum) + MUL_FIXED(M01, sinNum);
    currentMatrix[TbNum][1]   = -MUL_FIXED(M00, sinNum) + MUL_FIXED(M01, cosNum);
    currentMatrix[TbNum][3+0] =  MUL_FIXED(M10, cosNum) + MUL_FIXED(M11, sinNum);
    currentMatrix[TbNum][3+1] = -MUL_FIXED(M10, sinNum) + MUL_FIXED(M11, cosNum);
    currentMatrix[TbNum][6+0] =  MUL_FIXED(M20, cosNum) + MUL_FIXED(M21, sinNum);
    currentMatrix[TbNum][6+1] = -MUL_FIXED(M20, sinNum) + MUL_FIXED(M21, cosNum);

    SclRotregBuff[TbNum].matrix_a = currentMatrix[TbNum][0];
    SclRotregBuff[TbNum].matrix_b = currentMatrix[TbNum][1];
    SclRotregBuff[TbNum].matrix_d = currentMatrix[TbNum][3+0];
    SclRotregBuff[TbNum].matrix_e = currentMatrix[TbNum][3+1];

    return;
}


void   SCL_RotateZD(Fixed32 r)
{
	Uint16	TbNum;

	switch(SclCurSclNum)
	{
		case SCL_RBG_TB_A:
			TbNum = 0;
			break;
		case SCL_RBG_TB_B:
			TbNum = 1;
			break;
		default:
			return;
			break;
	}

	if(SclRbgKtbOffset[TbNum] && (SclRotateXy[TbNum] || SclRotateMoveZ[TbNum]) ) {
		SclRotateDisp[TbNum] = 0;
		return;
	}

	SclRotateDisp[TbNum] += r;

	if(SclRotateDisp[TbNum] >= FIXED(360)) SclRotateDisp[TbNum]-=FIXED(360);
	if(SclRotateDisp[TbNum] < FIXED(0))    SclRotateDisp[TbNum]+=FIXED(360);

	SclRotregBuff[TbNum].screenst.x
			= SclCsx[TbNum] - MUL_FIXED(Fcos(SclRotateDisp[TbNum]),SclCsx[TbNum])
				- MUL_FIXED((-Fsin(SclRotateDisp[TbNum])),SclCsy[TbNum]);
	SclRotregBuff[TbNum].screenst.y
			= SclCsy[TbNum] - MUL_FIXED(Fsin(SclRotateDisp[TbNum]),SclCsx[TbNum])
				- MUL_FIXED((Fcos(SclRotateDisp[TbNum])),SclCsy[TbNum]);
	SclRotregBuff[TbNum].screenst.z  = FIXED(0);
	SclRotregBuff[TbNum].screendlt.x = -Fsin(SclRotateDisp[TbNum]);
	SclRotregBuff[TbNum].screendlt.y =  Fcos(SclRotateDisp[TbNum]);
	SclRotregBuff[TbNum].delta.x     =  Fcos(SclRotateDisp[TbNum]);
	SclRotregBuff[TbNum].delta.y     =  Fsin(SclRotateDisp[TbNum]);
}

