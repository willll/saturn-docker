/*------------------------------------------------------------------------
 *  FILE:	SEGA_SCL.H
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *	Scroll Simulation program header file
 *	Scroll Register Data Structure Definition File
 *
 *  AUTHOR(S):
 *	K.M
 *		
 *  MOD HISTORY:
 *	Written by K.M on 1994-05-20 Ver.1.00
 *	Updated by K.M on 1994-07-01 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */

#ifndef	SEGA_SCR_H
#define SEGA_SCR_H

#include <sega_xpt.h>
#include <sega_def.h>
#include <sega_mth.h> 

#define VBR              0x06000000
#define SCU_BASE         0x25FE0000
#define SCU_INTR_MASK    0x000000a0
#define SCU_INTR_STATUS  0x000000a4
#define SETUINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x300)))
#define GETUINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x304)))
#define SETSINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x310)))
#define GETSINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x314)))
#define VBIVN	0x40
#define VBOVN	0x41
#define HBIVN	0x42

/*********************************************
 *      System Register Cluster	             *
 *      Address ranger 180000H - 180027H     *
 *********************************************/
 typedef struct	SclSysreg{
	Uint16	tvmode;			/* TV Mode */
	Uint16	extenbl;		/* External Input Enable */
	Uint16	tvstatus;		/* TV Status Read Only */
	Uint16	vramsize;		/* VRAM Size */
	Uint16	H_val;			/* H Counter Read Only */
	Uint16	V_val;			/* V Counter Read Only */
	Uint16	vramchg;		/* VRAM Change */
	Uint16	ramcontrl;		/* RAM Control */
	Uint16	vramcyc[8];		/* VRAM Cycle Table x8 */
	Uint16	dispenbl;		/* Display Enable */
	Uint16	mosaic;			/* Mosaic Control */
	Uint16	specialcode_sel;	/* Special Code Selector */
	Uint16	specialcode;		/* Special Code */
 } SclSysreg;

/**********************************************
 *      Scroll Data Set Register Cluster      *
 *      Address ranger 180028H - 18006FH      *
 **********************************************/
typedef struct SclDataset{
	Uint16	charcontrl0;		/* Character Control */
	Uint16	charcontrl1;		/* Character Control */
	Uint16	bmpalnum0;		/* Bitmap Palette Number */
	Uint16	bmpalnum1;		/* Bitmap Palette Number */
	Uint16	patnamecontrl[5];	/* Pattern Name Controlx5 */
	Uint16	platesize;		/* Plate Size */	
	Uint16	mapoffset0;		/* Map Offset */
	Uint16	mapoffset1;		/* Map Offset */
	Uint16	normap[8];		/* Normal Scroll Map x8*/
	Uint16	rotmap[16];		/* Rotate Scroll Map x16*/
} SclDataset;

/***************************************************
 *      Normal Scroll Function Register Cluster    *
 *      Address ranger 180070H - 1800AFH           *
 ***************************************************/
typedef struct SclNorscl{
	Fixed32	n0_move_x;		/* NBG0 Scroll Movement X */
	Fixed32	n0_move_y;		/* NBG0 Scroll Movement Y */
	Fixed32	n0_delta_x;		/* NBG0 Scroll Movement X */
	Fixed32	n0_delta_y;		/* NBG0 Scroll Movement Y */
	Fixed32	n1_move_x;		/* NBG1 Scroll Movement X */
	Fixed32	n1_move_y;		/* NBG1 Scroll Movement Y */
	Fixed32	n1_delta_x;		/* NBG1 Scroll Movement X */
	Fixed32	n1_delta_y;		/* NBG1 Scroll Movement Y */
	Uint16	n2_move_x;		/* NBG2 Scroll Movement X */
	Uint16	n2_move_y;		/* NBG2 Scroll Movement Y */
	Uint16	n3_move_x;		/* NBG3 Scroll Movement X */
	Uint16	n3_move_y;		/* NBG3 Scroll Movement Y */
	Uint16	zoomenbl;		/* Zoom Enable */
	Uint16	linecontrl;		/* Normal Extension Control */
	Uint32	celladdr;		/* Cell Scroll Table Address */
	Uint32	lineaddr[2];		/* Line Scroll Table Address x2 */
	Uint32	linecolmode;		/* Line BG Color Mode Address */
	Uint32	backcolmode;		/* Back BG Color Mode Address */
} SclNorscl;

/****************************************************
 *      Rotate Scroll Function Register Cluster     *
 *      Address ranger 1800B0H - 1800BFH            *
 ****************************************************/
typedef struct SclRotscl{
	Uint16	paramode;		/* Rotate Parameter Mode */
	Uint16	paramcontrl;		/* Rotate Parameter Read Control */
	Uint16	k_contrl;		/* Keisu Table Control */
	Uint16	k_offset;		/* Keisu Addres Offset */
	Uint16	mapover[2];		/* Rotate Scroll Map Over */
	Uint32	paramaddr;		/* Rotate Parameter Tabel Address */
} SclRotscl;

/***************************************************
 *      Window Scroll Setting Register Cluster     *
 *      Address ranger 1800C0H - 1800DFH           *
 ***************************************************/
typedef struct SclWinscl{
					/* 1800C0 */
	Uint16	win0_start[2];		/* Window #0 Start X & Y */
					/* 1800C4 */
	Uint16	win0_end[2];		/* Window #0 End X & Y */
					/* 1800C8 */
	Uint16	win1_start[2];		/* Window #1 Start X & Y */
					/* 1800CC */
	Uint16	win1_end[2];		/* Window #1 End X & Y */
					/* 1800D0 */
	Uint16	wincontrl[4];		/* Window Control */
					/* 1800D8 */
	Uint32 	linewin0_addr;		/* Line Window #0 Table Address */ 
					/* 1800DC */
	Uint32 	linewin1_addr;		/* Line Window #1 Table Address */ 
} SclWinscl;

/***********************************************
 *      Priority Related Register Cluster      *
 ***********************************************/
typedef struct SclPrior{
	Uint16	prnum;
} SclPrior;


/**************************************************
 *      Scroll Parameters Definition              *
 **************************************************/
typedef struct  SclXy {
	 Fixed32         x;
	 Fixed32         y;
} SclXy;

typedef struct  SclXyz {
	 Fixed32         x;
	 Fixed32         y;
	 Fixed32         z;
} SclXyz;

typedef struct  SclXy16 {
        Uint16         x;
        Uint16         y;
} SclXy16;

typedef struct  SclXyz16 {
        Sint16         x;
        Sint16         y;
        Sint16         z;
} SclXyz16;

typedef struct  SclRgb {
	 Sint16		red;
	 Sint16		green;
	 Sint16		blue;
} SclRgb;

typedef struct  SclLineTb {
	 Fixed32	h;
	 Fixed32	v;
	 Fixed32	dh;
} SclLineTb;

typedef struct  SclLineWindowTb {
	 Uint16		start;
	 Uint16		end;
} SclLineWindowTb;

typedef struct SclVramConfig{
	Uint32	ktboffsetA;/* 回転ﾊﾟﾗﾒｰﾀA用係数ﾃｰﾌﾞﾙをﾊﾞﾝｸ中のどこに配置するか */
	Uint32	ktboffsetB;/* 回転ﾊﾟﾗﾒｰﾀB用係数ﾃｰﾌﾞﾙをﾊﾞﾝｸ中のどこに配置するか */
	Uint8	vramModeA; /* VRAM A を２つのバンクに区切るか指定(0/1)  */
	Uint8	vramModeB; /* VRAM B を２つのバンクに区切るか指定(0/1)  */
	Uint8	vramA0;    /* VRAM A or A0 を回転面の何で使用するか指定 */
	Uint8	vramA1;    /* VRAM A1 を回転面の何で使用するか指定      */
	Uint8	vramB0;    /* VRAM B or B0 を回転面の何で使用するか指定 */
	Uint8	vramB1;    /* VRAM B1 を回転面の何で使用するか指定      */
	Uint8	colram;    /* カラーＲＡＭに係数データを置くか？        */
} SclVramConfig;

typedef struct SclRotreg{
	SclXyz		screenst;
	SclXy		screendlt;
	SclXy		delta;
	Fixed32		matrix_a;
	Fixed32		matrix_b;
	Fixed32		matrix_c;
	Fixed32		matrix_d;
	Fixed32		matrix_e;
	Fixed32		matrix_f;
	SclXyz16	viewp;
	Uint16		dummy1;
	SclXyz16	rotatecenter;
	Uint16		dummy2;
	SclXy		move;
	SclXy		zoom;
	Fixed32		k_tab;
	SclXy		k_delta;
	Fixed32		dummy3[8];
} SclRotreg;

typedef struct SclRotparam{
	Uint32		addr;
	SclXyz		screenst;
	SclXy		screendlt;
	SclXy		delta;
	Fixed32		matrix_a;
	Fixed32		matrix_b;
	Fixed32		matrix_c;
	Fixed32		matrix_d;
	Fixed32		matrix_e;
	Fixed32		matrix_f;
	Fixed32		matrix_g;
	Fixed32		matrix_h;
	Fixed32		matrix_i;
	Fixed32		drotangle;
	SclXyz		viewp;
	SclXyz		rotatecenter;
	SclXyz		disprotcenter;
	SclXyz		move;
	SclXy		zoom;
	Fixed32		k_tab;
	SclXy		k_delta;
	Uint16		k_size;
} SclRotparam;

/*******************************************
 *      Scroll Configure Data Structure    *
 *******************************************/
typedef	struct	SclConfig {
	Uint8		dispenbl;
	Uint8		charsize;
	Uint8		pnamesize;
	Uint8		platesize;
	Uint8		bmpsize;
	Uint8		coltype;
	Uint8		datatype;
	Uint8		mapover;
	Uint8		flip;
	Uint16		patnamecontrl;
	Uint32		plate_addr[16];
} SclConfig;

/******************************************************
 *      Line & Cell Scroll Parameter Data Structure   *
 ******************************************************/
#define	SCL_MAXLINE	512
#define	SCL_MAXCELL	64

typedef	struct	SclLineparam{
	Uint8		delta_enbl;
	Uint8		v_enbl;
	Uint8		h_enbl;
	Uint8		cell_enbl;
	Uint8		interval;
	Uint32		line_addr;
	Uint32		cell_addr;
	SclLineTb	line_tbl[SCL_MAXLINE];
	Fixed32		cell_tbl[SCL_MAXCELL];
}	SclLineparam;

typedef	struct	SclLineWin{
	Uint16		winum;
	Uint16		y[2];
	Uint32		addr;
	SclXy16		tbl[SCL_MAXLINE];
}	SclLineWin;


#define SCL_VDP2_VRAM		0x25e00000
#define SCL_VDP2_VRAM_A		0x25e00000
#define SCL_VDP2_VRAM_A0	0x25e00000
#define SCL_VDP2_VRAM_A1	0x25e20000
#define SCL_VDP2_VRAM_B		0x25e40000
#define SCL_VDP2_VRAM_B0	0x25e40000
#define SCL_VDP2_VRAM_B1	0x25e60000
#define	SCL_COLRAM_ADDR		0x25F00000	/* Color RAM */

#define SCL_NON			0   /* RBG0では使用しない        */
#define SCL_RBG0_K 		1   /* RBG0の係数ﾃｰﾌﾞﾙを置く     */
#define SCL_RBG0_PN 		2   /* RBG0のﾊﾟﾀｰﾝﾈｰﾑﾃｰﾌﾞﾙを置く */
#define SCL_RBG0_CHAR 		3   /* RBG0のｷｬﾗｸﾀを置く         */
#define SCL_RBG1_K 		4   /* RBG1の係数ﾃｰﾌﾞﾙを置く     */

#define	SCL_CHAR_SIZE_1X1	0
#define	SCL_CHAR_SIZE_2X2	1

#define	SCL_PN2WORD		0
#define	SCL_PN1WORD		1

#define	SCL_PL_SIZE_1X1		0
#define	SCL_PL_SIZE_2X1		1
#define	SCL_PL_SIZE_2X2		3

#define	SCL_BMP_SIZE_512X256	0
#define	SCL_BMP_SIZE_512X512	1
#define	SCL_BMP_SIZE_1024X256	2
#define	SCL_BMP_SIZE_1024X512	3

#define	SCL_COL_TYPE_16		0
#define	SCL_COL_TYPE_256	1
#define	SCL_COL_TYPE_2048	2
#define	SCL_COL_TYPE_32K	3
#define	SCL_COL_TYPE_1M		4

#define	SCL_CELL		0
#define	SCL_BITMAP		1

#define	SCL_OVER_0		0
#define	SCL_OVER_1		1
#define	SCL_OVER_2		2
#define	SCL_OVER_3		3

#define	SCL_PN_10BIT		0
#define	SCL_PN_12BIT		1

#define	SCL_1_LINE		0
#define	SCL_2_LINE		1
#define	SCL_4_LINE		2

#define	SCL_NON_INTER		0
#define	SCL_SINGLE_INTER	2
#define	SCL_DOUBLE_INTER	3

#define	SCL_224LINE		0
#define	SCL_240LINE		1
#define	SCL_256LINE		2

#define	SCL_NORMAL_A		0
#define	SCL_NORMAL_B		1
#define	SCL_HIRESO_A		2
#define	SCL_HIRESO_B		3
#define	SCL_NORMAL_AE		4
#define	SCL_NORMAL_BE		5
#define	SCL_HIRESO_AE		6
#define	SCL_HIRESO_BE		7

#define	SCL_X_AXIS		1
#define	SCL_Y_AXIS		2

#define	SCL_W0			0
#define	SCL_W1			1

#define FIXED(x)	      ((Fixed32)((x) * 65536.0))
#define MUL_FIXED(a, b)       MTH_Mul(a, b)
#define DIV_FIXED(a, b)       MTH_Div(a, b)

#endif	/* ifndef SEGA_SCR_H */


/*------------------------------------------------------------------------
 *  
 *
 *  DESCRIPTION:
 *
 *          Header file for Priority library internal use.
 *          Each Macro Set a parameter on Register buffer.
 *          the buffer will copy to REAL regster while V-interval.
 *          ここにあるマクロは、レジスタバッファに各コントロール値を
 *          書き込み、書き込んだレジスタバッファのダーティーフラグを
 *          1 にする。
-------------------------------------------------------------------------*/

#ifndef __PRI_GLVAR
#define __PRI_GLVAR

#ifndef __PRI_MACRO
#define __PRI_MACRO

#ifndef __PRI_REG
#define __PRI_REG

/*-----< other group regs >----*/
typedef struct {
    volatile Uint16 SpriteControl;		/* 1800E0 */
    volatile Uint16 ShadowControl;		/* 1800E2 */
    volatile Uint16 ColorRamOffset0;		/* 1800E4 */
    volatile Uint16 ColorRamOffset1;		/* 1800E6 */
    volatile Uint16 LineColorEnable;		/* 1800E8 */
    volatile Uint16 SpecialPriorityMode;		/* 1800EA */
    volatile Uint16 ColorMixControl;		/* 1800EC */
    volatile Uint16 SpecialColorMixMode;		/* 1800EE */
} SclOtherPriRegister;

/*----< sprite priority >----*/
typedef struct {
    volatile Uint16 PriorityNumberSP01;		/* 1800F0 */
    volatile Uint16 PriorityNumberSP23;		/* 1800F2 */
    volatile Uint16 PriorityNumberSP45;		/* 1800F4 */
    volatile Uint16 PriorityNumberSP67;		/* 1800F6 */
} SclSpPriNumRegister;

/*----< scroll priority >----*/
typedef struct {
    volatile Uint16 PriorityNumberNBG01;		/* 1800F8 */
    volatile Uint16 PriorityNumberNBG23;		/* 1800FA */
    volatile Uint16 PriorityNumberRBG0;		/* 1800FC */
} SclBgPriNumRegister;

/*----< sprite color Mix >----*/
typedef struct {
    volatile Uint16 ColMixRateSP01;		/* 180100 */
    volatile Uint16 ColMixRateSP23;		/* 180102 */
    volatile Uint16 ColMixRateSP45;		/* 180104 */
    volatile Uint16 ColMixRateSP67;		/* 180106 */
} SclSpColMixRegister;

/*----< scroll color Mix >----*/
typedef struct {
    volatile Uint16 ColMixRateNBG01;		/* 180108 */
    volatile Uint16 ColMixRateNBG23;		/* 18010A */
    volatile Uint16 ColMixRateRBG0;		/* 18010C */
    volatile Uint16 ColMixRateLCBAK;		/* 18010E */
} SclBgColMixRegister;

/*----< color offset >----*/
typedef struct {
    volatile Uint16 ColorOffsetEnable;		/* 180110 */
    volatile Uint16 ColorOffsetSelect;		/* 180112 */
    volatile Uint16 ColorOffsetA_RED;		/* 180114 */
    volatile Uint16 ColorOffsetA_GREEN;		/* 180116 */
    volatile Uint16 ColorOffsetA_BLUE;		/* 180118 */
    volatile Uint16 ColorOffsetB_RED;		/* 18011A */
    volatile Uint16 ColorOffsetB_GREEN;		/* 18011E */
    volatile Uint16 ColorOffsetB_BLUE;		/* 180120 */
} SclColOffsetRegister;

/*---- レジスタバッファのダーティー・フラグ */
typedef struct
{
    volatile Uint32 SclOtherPri:1;
    volatile Uint32 SclSpPriNum:1;
    volatile Uint32 SclBgPriNum:1;
    volatile Uint32 SclSpColMix:1;
    volatile Uint32 SclBgColMix:1;
    volatile Uint32 SclColOffset:1;
} SclPriBuffDirtyFlags;

#endif /* __PRI_REG */


extern SclOtherPriRegister	SclOtherPri;
extern SclSpPriNumRegister	SclSpPriNum;
extern SclBgPriNumRegister	SclBgPriNum;
extern SclSpColMixRegister	SclSpColMix;
extern SclBgColMixRegister	SclBgColMix;
extern SclColOffsetRegister	SclColOffset;
extern SclPriBuffDirtyFlags	SclPriBuffDirty;

/* Following macros never check argument value range.*/

/* VDP-II manual p.129 */
#define SCL_SET_SPCCCS(/* 2 bits */ spcccs) /* スプライト色演算条件 */\
  (SclPriBuffDirty.SclOtherPri = 1,\
   SclOtherPri.SpriteControl \
   = (SclOtherPri.SpriteControl & 0x0FFF) | ((spcccs) << 12))

#define SCL_GET_SPCCCS() \
  ((SclOtherPri.SpriteControl & 0x3000) >> 12)

#define SCL_SET_SPCCN(/* 3 bits */ spccn) /* スプライト色演算条件ナンバ */\
  (SclPriBuffDirty.SclOtherPri = 1,\
   SclOtherPri.SpriteControl \
   = (SclOtherPri.SpriteControl & 0xF0FF) | ((spccn) << 8))

#define SCL_GET_SPCCN() \
  ((SclOtherPri.SpriteControl & 0x0700) >> 8)

#define SCL_SET_SPCLMD(/* 1 bit */ spclmd) /* スプライトの色形式 */\
  (SclPriBuffDirty.SclOtherPri = 1,\
   SclOtherPri.SpriteControl \
   = (SclOtherPri.SpriteControl & 0xFFDF) | ((spclmd) << 5))

#define SCL_GET_SPCLMD() \
  ((SclOtherPri.SpriteControl & 0x0020) >> 5)

#define SCL_SET_SPWINEN(/* 1 bit */ spwinen) /* スプライトウィンドウイネーブル */\
  (SclPriBuffDirty.SclOtherPri = 1,\
   SclOtherPri.SpriteControl \
   = (SclOtherPri.SpriteControl & 0xFFEF) | ((spwinen) << 4))    /* -> p.121 */

#define SCL_GET_SPWINEN() \
  ((SclOtherPri.SpriteControl & 0x0010) >> 4)

#define SCL_SET_SPTYPE(/* 4bits */ sptype) /* スプライトのタイプ指定 */\
  (SclPriBuffDirty.SclOtherPri = 1,\
   SclOtherPri.SpriteControl \
   = (SclOtherPri.SpriteControl & 0xFFF0) | (sptype))

#define SCL_GET_SPTYPE() \
  (SclOtherPri.SpriteControl & 0x000F)

/* VDP-II manual p.131 */
/*-------------------- スプライト用プライオリティレジスタ No.0-7 */

#define SCL_SET_S0PRIN(/* 3 bits */ s0prin) \
  (SclPriBuffDirty.SclSpPriNum = 1,\
   SclSpPriNum.PriorityNumberSP01 \
   = (SclSpPriNum.PriorityNumberSP01 & 0xFFF0) | (s0prin))

#define SCL_GET_S0PRIN() \
  ((SclSpPriNum.PriorityNumberSP01 & 0x0007))

#define SCL_SET_S1PRIN(/* 3 bits */ s1prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP01 \
   = (SclSpPriNum.PriorityNumberSP01 & 0xF0FF) | ((s1prin) << 8))

#define SCL_GET_S1PRIN() \
  ((SclSpPriNum.PriorityNumberSP01 & 0x0700) >> 8)

#define SCL_SET_S2PRIN(/* 3 bits */ s2prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP23 \
   = (SclSpPriNum.PriorityNumberSP23 & 0xFFF0) | ((s2prin)))

#define SCL_GET_S2PRIN() \
  ((SclSpPriNum.PriorityNumberSP23 & 0x0007))

#define SCL_SET_S3PRIN(/* 3 bits */ s3prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP23 \
   = (SclSpPriNum.PriorityNumberSP23 & 0xF0FF) | ((s3prin) << 8))

#define SCL_GET_S3PRIN() \
  ((SclSpPriNum.PriorityNumberSP23 & 0x0700) >> 8)

#define SCL_SET_S4PRIN(/* 3 bits */ s4prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP45 \
   = (SclSpPriNum.PriorityNumberSP45 & 0xFFF0) | ((s4prin)))

#define SCL_GET_S4PRIN() \
  ((SclSpPriNum.PriorityNumberSP45 & 0x0007))

#define SCL_SET_S5PRIN(/* 3 bits */ s5prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP45 \
   = (SclSpPriNum.PriorityNumberSP45 & 0xF0FF) | ((s5prin) << 8))

#define SCL_GET_S5PRIN() \
  ((SclSpPriNum.PriorityNumberSP45 & 0x0700) >> 8)

#define SCL_SET_S6PRIN(/* 3 bits */ s6prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP67 \
   = (SclSpPriNum.PriorityNumberSP67 & 0xFFF0) | ((s6prin)))

#define SCL_GET_S6PRIN() \
  ((SclSpPriNum.PriorityNumberSP67 & 0x0007))

#define SCL_SET_S7PRIN(/* 3 bits */ s7prin) \
  (SclPriBuffDirty.SclSpPriNum = 1, \
   SclSpPriNum.PriorityNumberSP67 \
   = (SclSpPriNum.PriorityNumberSP67 & 0xF0FF) | ((s7prin) << 8))

#define SCL_GET_S7PRIN() \
  ((SclSpPriNum.PriorityNumberSP67 & 0x0700) >> 8)

/* VDP-II manual p.132 */
/*------------------- スプライト用カラー演算割合レジスタ No.0-7 */

#define SCL_SET_S0CCRT(/* 5bits */ s0ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP01 \
   = (SclSpColMix.ColMixRateSP01 & 0xFF00) | ((s0ccrt)))

#define SCL_GET_S0CCRT() \
  ((SclSpColMix.ColMixRateSP01 & 0x001F) >> 0)

#define SCL_SET_S1CCRT(/* 5bits */ s1ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP01 \
   = (SclSpColMix.ColMixRateSP01 & 0x00FF) | ((s1ccrt) << 8))

#define SCL_GET_S1CCRT() \
  ((SclSpColMix.ColMixRateSP01 & 0x1F00) >> 8)

#define SCL_SET_S2CCRT(/* 5bits */ s2ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP23 \
   = (SclSpColMix.ColMixRateSP23 & 0xFF00) | ((s2ccrt)))

#define SCL_GET_S2CCRT() \
  ((SclSpColMix.ColMixRateSP23 & 0x001F))

#define SCL_SET_S3CCRT(/* 5bits */ s3ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP23 \
   = (SclSpColMix.ColMixRateSP23 & 0x00FF) | ((s3ccrt) << 8))

#define SCL_GET_S3CCRT() \
  ((SclSpColMix.ColMixRateSP23 & 0x1F00) >> 8)

#define SCL_SET_S4CCRT(/* 5bits */ s4ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP45 \
   = (SclSpColMix.ColMixRateSP45 & 0xFF00) | ((s4ccrt)))

#define SCL_GET_S4CCRT() \
  ((SclSpColMix.ColMixRateSP45 & 0x001F))

#define SCL_SET_S5CCRT(/* 5bits */ s5ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP45 \
   = (SclSpColMix.ColMixRateSP45 & 0x00FF) | ((s5ccrt) << 8))

#define SCL_GET_S5CCRT() \
  ((SclSpColMix.ColMixRateSP45 & 0x1F00) >> 8)

#define SCL_SET_S6CCRT(/* 5bits */ s6ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP67 \
   = (SclSpColMix.ColMixRateSP67 & 0xFF00) | ((s6ccrt)))

#define SCL_GET_S6CCRT() \
  ((SclSpColMix.ColMixRateSP67 & 0x001F))

#define SCL_SET_S7CCRT(/* 5bits */ s7ccrt) \
  (SclPriBuffDirty.SclSpColMix = 1, \
   SclSpColMix.ColMixRateSP67 \
   = (SclSpColMix.ColMixRateSP67 & 0x00FF) | ((s7ccrt) << 8))

#define SCL_GET_S7CCRT() \
  ((SclSpColMix.ColMixRateSP67 & 0x1F00) >> 8)

/* VDP-II manual p.138 */
/*-- カラー RAM アドレスオフセット N0|R1, N1|EXBG, N2, N3, R0, SPRITE */

#define SCL_SET_N0CAOS(/* 3bits */ n0caos) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorRamOffset0 \
   = (SclOtherPri.ColorRamOffset0 & 0xFFF0) | ((n0caos)))

#define SCL_GET_N0CAOS() \
  ((SclOtherPri.ColorRamOffset0 & 0x0007))

#define SCL_SET_N1CAOS(/* 3bits */ n1caos) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorRamOffset0 \
   = (SclOtherPri.ColorRamOffset0 & 0xFF0F) | ((n1caos) << 4))

#define SCL_GET_N1CAOS() \
  ((SclOtherPri.ColorRamOffset0 & 0x0070) >> 4)

#define SCL_SET_N2CAOS(/* 3bits */ n2caos) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorRamOffset0 \
   = (SclOtherPri.ColorRamOffset0 & 0xF0FF) | ((n2caos) << 8))

#define SCL_GET_N2CAOS() \
  ((SclOtherPri.ColorRamOffset0 & 0x0700) >> 8)

#define SCL_SET_N3CAOS(/* 3bits */ n3caos) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorRamOffset0 \
   = (SclOtherPri.ColorRamOffset0 & 0x0FFF) | ((n3caos) << 12))

#define SCL_GET_N3CAOS() \
  ((SclOtherPri.ColorRamOffset0 & 0x7000) >> 12)

#define SCL_SET_R0CAOS(/* 3bits */ r0caos) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorRamOffset1 \
   = (SclOtherPri.ColorRamOffset1 & 0xFFF0) | ((r0caos) << 0))

#define SCL_GET_R0CAOS() \
  ((SclOtherPri.ColorRamOffset1 & 0x0007) >> 0)

#define SCL_SET_SPCAOS(/* 3bits */ spcaos) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorRamOffset1 \
   = (SclOtherPri.ColorRamOffset1 & 0xFF0F) | ((spcaos) << 4))

#define SCL_GET_SPCAOS() \
  ((SclOtherPri.ColorRamOffset1 & 0x0070) >> 4)


/*----- ラインカラー画面イネーブル N0|R1, N1|EXBG, N2, N3, R0, SPRITE */

#define SCL_SET_N0LCEN(/* 1bit */ n0lcen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.LineColorEnable \
   = (SclOtherPri.LineColorEnable & 0xFFFE) | ((n0lcen)))

#define SCL_GET_N0LCEN() \
  ((SclOtherPri.LineColorEnable & 0x0001))

#define SCL_SET_N1LCEN(/* 1bit */ n1lcen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.LineColorEnable \
   = (SclOtherPri.LineColorEnable & 0xFFFD) | ((n1lcen) << 1))

#define SCL_GET_N1LCEN() \
  ((SclOtherPri.LineColorEnable & 0x0002) >> 1)

#define SCL_SET_N2LCEN(/* 1bit */ n2lcen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.LineColorEnable \
   = (SclOtherPri.LineColorEnable & 0xFFFB) | ((n2lcen) << 2))

#define SCL_GET_N2LCEN() \
  ((SclOtherPri.LineColorEnable & 0x0004) >> 2)

#define SCL_SET_N3LCEN(/* 1bit */ n3lcen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.LineColorEnable \
   = (SclOtherPri.LineColorEnable & 0xFFF7) | ((n3lcen) << 3))

#define SCL_GET_N3LCEN() \
  ((SclOtherPri.LineColorEnable & 0x0008) >> 3)

#define SCL_SET_R0LCEN(/* 1bit */ r0lcen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.LineColorEnable \
   = (SclOtherPri.LineColorEnable & 0xFFEF) | ((r0lcen) << 4))

#define SCL_GET_R0LCEN() \
  ((SclOtherPri.LineColorEnable & 0x0010) >> 4)

#define SCL_SET_SPLCEN(/* 1bit */ splcen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.LineColorEnable \
   = (SclOtherPri.LineColorEnable & 0xFFDF) | ((splcen) << 5))

#define SCL_GET_SPLCEN() \
  ((SclOtherPri.LineColorEnable & 0x0020) >> 5)

/*-------- 特殊プライオリティモード N0|R1, N1|EXBG, N2, N3, R0 */

#define SCL_SET_N0SPRM(/* 2bits */ n0sprm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialPriorityMode \
   = (SclOtherPri.SpecialPriorityMode & 0xFFFC) | ((n0sprm)))

#define SCL_GET_N0SPRM() \
  ((SclOtherPri.SpecialPriorityMode & 0x0003))

#define SCL_SET_N1SPRM(/* 2bits */ n1sprm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialPriorityMode \
   = (SclOtherPri.SpecialPriorityMode & 0xFFF3) | ((n1sprm) << 2))

#define SCL_GET_N1SPRM() \
  ((SclOtherPri.SpecialPriorityMode & 0x000C) >> 2)

#define SCL_SET_N2SPRM(/* 2bits */ n2sprm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialPriorityMode \
   = (SclOtherPri.SpecialPriorityMode & 0xFFCF) | ((n2sprm) << 4))

#define SCL_GET_N2SPRM() \
  ((SclOtherPri.SpecialPriorityMode & 0x0030) >> 4)

#define SCL_SET_N3SPRM(/* 2bits */ n3sprm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialPriorityMode \
   = (SclOtherPri.SpecialPriorityMode & 0xFF3F) | ((n3sprm) << 6))

#define SCL_GET_N3SPRM() \
  ((SclOtherPri.SpecialPriorityMode & 0x00C0) >> 6)

#define SCL_SET_R0SPRM(/* 2bits */ r0sprm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialPriorityMode \
   = (SclOtherPri.SpecialPriorityMode & 0xFCFF) | ((r0sprm) << 8))

#define SCL_GET_R0SPRM() \
  ((SclOtherPri.SpecialPriorityMode & 0x0300) >> 8)

/*---------プライオリティー番号 N0|R1, N1|EXBG, N2, N3, R0 */

#define SCL_SET_N0PRIN(/* 3 bits */ n0prin) \
  (SclPriBuffDirty.SclBgPriNum = 1, \
   SclBgPriNum.PriorityNumberNBG01 \
   = (SclBgPriNum.PriorityNumberNBG01 & 0xFF00) | ((n0prin)))

#define SCL_GET_N0PRIN() \
  ((SclBgPriNum.PriorityNumberNBG01 & 0x0007))

#define SCL_SET_N1PRIN(/* 3 bits */ n1prin) \
  (SclPriBuffDirty.SclBgPriNum = 1, \
   SclBgPriNum.PriorityNumberNBG01 \
   = (SclBgPriNum.PriorityNumberNBG01 & 0x00FF) | ((n1prin) << 8))

#define SCL_GET_N1PRIN() \
  ((SclBgPriNum.PriorityNumberNBG01 & 0x0700) >> 8)

#define SCL_SET_N2PRIN(/* 3 bits */ n2prin) \
  (SclPriBuffDirty.SclBgPriNum = 1, \
   SclBgPriNum.PriorityNumberNBG23 \
   = (SclBgPriNum.PriorityNumberNBG23 & 0xFF00) | ((n2prin)))

#define SCL_GET_N2PRIN() \
  ((SclBgPriNum.PriorityNumberNBG23 & 0x0007))

#define SCL_SET_N3PRIN(/* 3 bits */ n3prin) \
  (SclPriBuffDirty.SclBgPriNum = 1, \
   SclBgPriNum.PriorityNumberNBG23 \
   = (SclBgPriNum.PriorityNumberNBG23 & 0x00FF) | ((n3prin) << 8))

#define SCL_GET_N3PRIN() \
  ((SclBgPriNum.PriorityNumberNBG23 & 0x0700) >> 8)

#define SCL_SET_R0PRIN(/* 3 bits */ r0prin) \
  (SclPriBuffDirty.SclBgPriNum = 1, \
   SclBgPriNum.PriorityNumberRBG0 \
   = ((r0prin)))

#define SCL_GET_R0PRIN() \
   ((SclBgPriNum.PriorityNumberRBG0 & 0x0007))

#define SCL_SET_BOKEN(/* 1 bits */ boken) /* ボカシ演算イネーブル */\
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0x7FFF) | ((boken) << 15))

#define SCL_GET_B0KEN() \
  ((SclOtherPri.ColorMixControl & 0x8000) >> 15)

#define SCL_SET_BOKN(/* 3 bits */ bokn) /* ボカシ画面番号 */\
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0x8FFF) | ((bokn) << 12))

#define SCL_GET_BOKN() \
  ((SclOtherPri.ColorMixControl & 0x7000) >> 12)

#define SCL_SET_EXCCEN(/* 1 bit */ exccn) /* 拡張カラー演算イネーブル */\
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFBFF) | ((exccn) << 10))

#define SCL_GET_EXCCEN() \
  ((SclOtherPri.ColorMixControl & 0x0400) >> 10)

#define SCL_SET_CCRTMD(/* 1 bit */ ccrtmd) /* カラー演算割合モード */\
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFDFF) | ((Uint32)(ccrtmd) << 9))

#define SCL_GET_CCRTMD() \
  ((SclOtherPri.ColorMixControl & 0x0200) >> 9)

#define SCL_SET_CCMD(/* 1 bit */ ccmd) /* カラー演算モード */\
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFEFF) | ((Uint32)(ccmd) << 8))

#define SCL_GET_CCMD() \
  ((SclOtherPri.ColorMixControl & 0x0100) >> 8)

/*------ カラー演算イネーブル N0|R1, N1|EXBG, N2, N3, R0, LC, SPRITE */ 
#define SCL_SET_SPCCEN(/* 1 bit */ spccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFBF) | ((spccen) << 6))

#define SCL_GET_SPCCEN() \
  ((SclOtherPri.ColorMixControl & 0x0040) >> 6)

#define SCL_SET_LCCCEN(/* 1 bit */ lcccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFDF) | ((lcccen) << 5))

#define SCL_GET_LCCCEN() \
  ((SclOtherPri.ColorMixControl & 0x0020) >> 5)

#define SCL_SET_R0CCEN(/* 1 bit */ r0ccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFEF) | ((r0ccen) << 4))

#define SCL_GET_R0CCEN() \
  ((SclOtherPri.ColorMixControl & 0x0010) >> 4)

#define SCL_SET_N3CCEN(/* 1 bit */ n3ccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFF7) | ((n3ccen) << 3))

#define SCL_GET_N3CCEN() \
  ((SclOtherPri.ColorMixControl & 0x0008) >> 3)

#define SCL_SET_N2CCEN(/* 1 bit */ n2ccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFFB) | ((n2ccen) << 2))

#define SCL_GET_N2CCEN() \
  ((SclOtherPri.ColorMixControl & 0x0004) >> 2)

#define SCL_SET_N1CCEN(/* 1 bit */ n1ccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFFD) | ((n1ccen) << 1))
  
#define SCL_GET_N1CCEN() \
  ((SclOtherPri.ColorMixControl & 0x0002) >> 1)

#define SCL_SET_N0CCEN(/* 1 bit */ n0ccen) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ColorMixControl \
   = (SclOtherPri.ColorMixControl & 0xFFFE) | ((n0ccen)))

#define SCL_GET_N0CCEN() \
  ((SclOtherPri.ColorMixControl & 0x0001))


/*-------------- 特殊カラー演算モード N0|R1, N1|EXBG, N2, N3, R0 */
#define SCL_SET_N0SCCM(/* 2 bits */ n0sccm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialColorMixMode \
   = (SclOtherPri.SpecialColorMixMode & 0xFFFC) | ((n0sccm)))

#define SCL_GET_N0SCCM() \
  ((SclOtherPri.SpecialColorMixMode & 0x0003))

#define SCL_SET_N1SCCM(/* 2 bits */ n1sccm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialColorMixMode \
   = (SclOtherPri.SpecialColorMixMode & 0xFFF3) | ((n1sccm) << 2))

#define SCL_GET_N1SCCM() \
  ((SclOtherPri.SpecialColorMixMode & 0x000C) >> 2)

#define SCL_SET_N2SCCM(/* 2 bits */ n2sccm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialColorMixMode \
   = (SclOtherPri.SpecialColorMixMode & 0xFFCF) | ((n2sccm) << 4))

#define SCL_GET_N2SCCM() \
  ((SclOtherPri.SpecialColorMixMode & 0x0030) >> 4)

#define SCL_SET_N3SCCM(/* 2 bits */ n3sccm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialColorMixMode \
   = (SclOtherPri.SpecialColorMixMode & 0xFF3F) | ((n3sccm) << 6))

#define SCL_GET_N3SCCM() \
  ((SclOtherPri.SpecialColorMixMode & 0x00C0) >> 6)

#define SCL_SET_R0SCCM(/* 2 bits */ r0sccm) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.SpecialColorMixMode \
  = (SclOtherPri.SpecialColorMixMode & 0xFCFF) | ((r0sccm) << 8))

#define SCL_GET_R0SCCM() \
  ((SclOtherPri.SpecialColorMixMode & 0x0300) >> 8)

/*------ スクロール面用カラー演算割合 N0|R1, N1|EXBG, N2, N3, R0, LC, BACK */
#define SCL_SET_N0CCRT(/* 5 bits */ n0ccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateNBG01 \
   = (SclBgColMix.ColMixRateNBG01 & 0xFF00) | ((n0ccrt)))

#define SCL_GET_N0CCRT() \
  ((SclBgColMix.ColMixRateNBG01 & 0x001F))

#define SCL_SET_N1CCRT(/* 5 bits */ n1ccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateNBG01 \
   = (SclBgColMix.ColMixRateNBG01 & 0x00FF) | ((n1ccrt) << 8))

#define SCL_GET_N1CCRT() \
  ((SclBgColMix.ColMixRateNBG01 & 0x1F00) >> 8)

#define SCL_SET_N2CCRT(/* 5 bits */ n2ccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateNBG23 \
   = (SclBgColMix.ColMixRateNBG23 & 0xFF00) | ((n2ccrt)))

#define SCL_GET_N2CCRT() \
  ((SclBgColMix.ColMixRateNBG23 & 0x001F))

#define SCL_SET_N3CCRT(/* 5 bits */ n3ccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateNBG23 \
   = (SclBgColMix.ColMixRateNBG23 & 0x00FF) | ((n3ccrt) << 8))

#define SCL_GET_N3CCRT() \
  ((SclBgColMix.ColMixRateNBG23 & 0x1F00) >> 8)

#define SCL_SET_R0CCRT(/* 5 bits */ r0ccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateRBG0 \
   = ((r0ccrt)))

#define SCL_GET_R0CCRT() \
  ((SclBgColMix.ColMixRateRBG0 & 0x001F))

#define SCL_SET_LCCCRT(/* 5 bits */ lcccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateLCBAK \
   = (SclBgColMix.ColMixRateLCBAK & 0xFF00) | ((lcccrt)))

#define SCL_GET_LCCCRT() \
  ((SclBgColMix.ColMixRateLCBAK & 0x001F))

#define SCL_SET_BKCCRT(/* 5 bits */ bkccrt) \
  (SclPriBuffDirty.SclBgColMix = 1, \
   SclBgColMix.ColMixRateLCBAK \
   = (SclBgColMix.ColMixRateLCBAK & 0x00FF) | ((bkccrt) << 8))

#define SCL_GET_BKCCRT() \
  ((SclBgColMix.ColMixRateLCBAK & 0x1F00) >> 8)

/*-- カラーオフセットイネーブル N0|R1, N1|EXBG, N2, N3, R0, BACK, SPRITE */

#define SCL_SET_N0COEN(/* 1 bit */ n0coen) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetEnable \
   = (SclColOffset.ColorOffsetEnable & 0xFFFE) | ((n0coen)))

#define SCL_GET_N0COEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0001))

#define SCL_SET_N1COEN(/* 1 bit */ n1coen) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetEnable \
   = (SclColOffset.ColorOffsetEnable & 0xFFFD) | ((n1coen) << 1))

#define SCL_GET_N1COEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0002) >> 1)

#define SCL_SET_N2COEN(/* 1 bit */ n2coen) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetEnable \
   = (SclColOffset.ColorOffsetEnable & 0xFFFB) | ((n2coen) << 2))

#define SCL_GET_N2COEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0004) >> 2)

#define SCL_SET_N3COEN(/* 1 bit */ n3coen) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetEnable \
   = (SclColOffset.ColorOffsetEnable & 0xFFF7) | ((n3coen) << 3))

#define SCL_GET_N3COEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0008) >> 3)

#define SCL_SET_R0COEN(/* 1 bit */ r0coen) \
   (SclPriBuffDirty.SclColOffset = 1, \
    SclColOffset.ColorOffsetEnable \
    = (SclColOffset.ColorOffsetEnable & 0xFFEF) | ((r0coen) << 4))

#define SCL_GET_R0COEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0010) >> 4)

#define SCL_SET_BKCOEN(/* 1 bit */ bkcoen) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetEnable \
   = (SclColOffset.ColorOffsetEnable & 0xFFDF) | ((bkcoen) << 5))

#define SCL_GET_BKCOEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0020) >> 5)

#define SCL_SET_SPCOEN(/* 1 bit */ spcoen) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetEnable \
   = (SclColOffset.ColorOffsetEnable & 0xFFBF) | ((spcoen) << 6))

#define SCL_GET_SPCOEN() \
  ((SclColOffset.ColorOffsetEnable & 0x0040) >> 6)

/*--- カラーオフセットレジスタ選択 N0|R1, N1|EXBG, N2, N3, R0, BACK, SPRITE */

#define SCL_SET_N0COSL(/* 1 bit */ n0cosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFFE) | (n0cosl))

#define SCL_GET_N0COSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0001))

#define SCL_SET_N1COSL(/* 1 bit */ n1cosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFFD) | ((n1cosl) << 1))

#define SCL_GET_N1COSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0002) >> 1)

#define SCL_SET_N2COSL(/* 1 bit */ n2cosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFFB) | ((n2cosl) << 2))

#define SCL_GET_N2COSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0004) >> 2)

#define SCL_SET_N3COSL(/* 1 bit */ n3cosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFF7) | ((n3cosl) << 3))

#define SCL_GET_N3COSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0008) >> 3)

#define SCL_SET_R0COSL(/* 1 bit */ r0cosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFEF) | ((r0cosl) << 4))

#define SCL_GET_R0COSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0010) >> 4)

#define SCL_SET_BKCOSL(/* 1 bit */ bkcosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFDF) | ((bkcosl) << 5))

#define SCL_GET_BKCOSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0020) >> 5)

#define SCL_SET_SPCOSL(/* 1 bit */ spcosl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetSelect \
   = (SclColOffset.ColorOffsetSelect & 0xFFBF) | ((spcosl) << 6))

#define SCL_GET_SPCOSL() \
  ((SclColOffset.ColorOffsetSelect & 0x0040) >> 6)

/* カラーオフセットレジスタ A */
#define SCL_SET_COARD(/* 9 bit */ coard) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetA_RED \
   = (coard))

#define SCL_GET_COARD() \
  (SclColOffset.ColorOffsetA_RED & 0x01FF)

#define SCL_SET_COAGR(/* 9 bit */ coagr) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetA_GREEN \
   = (coagr))

#define SCL_GET_COAGR() \
  (SclColOffset.ColorOffsetA_GREEN & 0x01FF)

#define SCL_SET_COABL(/* 9 bit */ coabl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetA_BLUE \
   = (coabl))

#define SCL_GET_COABL() \
  (SclColOffset.ColorOffsetA_BLUE & 0x01FF)

/* カラーオフセットレジスタ B*/
#define SCL_SET_COBRD(/* 9 bit */ cobrd) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetB_RED \
   = (cobrd))

#define SCL_GET_COBRD() \
   (SclColOffset.ColorOffsetB_RED & 0x01FF)

#define SCL_SET_COBGR(/* 9 bit */ cobgr) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetB_GREEN \
   = (cobgr))

#define SCL_GET_COBGR() \
  (SclColOffset.ColorOffsetB_GREEN & 0x01FF)

#define SCL_SET_COBBL(/* 9 bit */ cobbl) \
  (SclPriBuffDirty.SclColOffset = 1, \
   SclColOffset.ColorOffsetB_BLUE \
   = (cobbl))

#define SCL_GET_COBBL() \
  (SclColOffset.ColorOffsetB_BLUE & 0x01FF)

/*----------- シャドウコントロール N0|R1, N1|EXBG, N2, N3, R0, BACK */

#define SCL_SET_N0SDEN(/* 1 bit */ n0sden) \
   (SclPriBuffDirty.SclOtherPri = 1, \
    SclOtherPri.ShadowControl \
    = (SclOtherPri.ShadowControl & 0xFFFE) | ((n0sden)))

#define SCL_GET_N0SDEN() \
  ((SclOtherPri.ShadowControl & 0x0001))

#define SCL_SET_N1SDEN(/* 1 bit */ n1sden) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ShadowControl \
   = (SclOtherPri.ShadowControl & 0xFFFD) | ((n1sden) << 1))

#define SCL_GET_N1SDEN() \
  ((SclOtherPri.ShadowControl & 0x0002) >> 1)

#define SCL_SET_N2SDEN(/* 1 bit */ n2sden) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ShadowControl \
   = (SclOtherPri.ShadowControl & 0xFFFB) | ((n2sden) << 2))

#define SCL_GET_N2SDEN() \
  ((SclOtherPri.ShadowControl & 0x0004) >> 2)

#define SCL_SET_N3SDEN(/* 1 bit */ n3sden) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ShadowControl \
   = (SclOtherPri.ShadowControl & 0xFFF7) | ((n3sden) << 3))

#define SCL_GET_N3SDEN() \
  ((SclOtherPri.ShadowControl & 0x0008) >> 3)

#define SCL_SET_R0SDEN(/* 1 bit */ r0sden) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ShadowControl \
   = (SclOtherPri.ShadowControl & 0xFFEF) | ((r0sden) << 4))

#define SCL_GET_R0SDEN() \
  ((SclOtherPri.ShadowControl & 0x0010) >> 4)

#define SCL_SET_BKSDEN(/* 1 bit */ bksden) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ShadowControl \
   = (SclOtherPri.ShadowControl & 0xFFDF) | ((bksden) << 5))

#define SCL_GET_BKSDEN() \
  ((SclOtherPri.ShadowControl & 0x0020) >> 5)

/*--------------------------------------- 透明シャドウセレクト */
#define SCL_SET_TPSDSL(/* 1 bit */ tpsden) \
  (SclPriBuffDirty.SclOtherPri = 1, \
   SclOtherPri.ShadowControl \
   = (SclOtherPri.ShadowControl & 0xFEFF) | ((tpsden) << 8))

#define SCL_GET_TPSDSL() \
  ((SclOtherPri.ShadowControl & 0x0100) >> 8)

/*---------------------------カラー RAM モード */
/* 0x1800E 番地にあるので スクロールライブラリで設定する */

/* 値の定義 */

#define SCL_RBG0	0x00000001
#define SCL_RBG1	0x00000002
#define SCL_NBG0	0x00000004
#define SCL_NBG1	0x00000008
#define SCL_NBG2	0x00000010
#define SCL_NBG3	0x00000020

#define SCL_EXBG	0x00000080
#define SCL_SPR 	0x00000100
#define SCL_SP0 	0x00000100
#define SCL_SP1 	0x00000200
#define SCL_SP2 	0x00000400
#define SCL_SP3 	0x00000800
#define SCL_SP4 	0x00001000
#define SCL_SP5 	0x00002000
#define SCL_SP6 	0x00004000
#define SCL_SP7 	0x00008000
#define SCL_RP  	0x00010000
#define SCL_RP_R	0xfffeffff
#define SCL_CC  	0x00020000
#define SCL_LNCL	0x00040000
#define SCL_BACK	0x00080000

#define SCL_RBG_TB_A	SCL_RBG0
#define SCL_RBG_TB_B	SCL_RBG1

#define SCL_CRM15_1024 0
#define SCL_CRM15_2048 1
#define SCL_CRM24_1024 2

/* スプライトのタイプ指定 */
#define SCL_TYPE0	0
#define SCL_TYPE1	1
#define SCL_TYPE2	2
#define SCL_TYPE3	3
#define SCL_TYPE4	4
#define SCL_TYPE5	5
#define SCL_TYPE6	6
#define SCL_TYPE7	7
#define SCL_TYPE8	8
#define SCL_TYPE9	9
#define SCL_TYPEA	10
#define SCL_TYPEB	11
#define SCL_TYPEC	12
#define SCL_TYPED	13
#define SCL_TYPEE	14
#define SCL_TYPEF	15

#define SCL_PALETTE	0
#define SCL_MIX 	1

#define SCL_MSB_SHADOW	0
#define SCL_SP_WINDOW	1

#define SCL_IF_BEHIND	0
#define SCL_IF_EQUAL	1
#define SCL_IF_FRONT	2
#define SCL_MSB_ON	3

#define SCL_OFFSET_A	0
#define SCL_OFFSET_B	1

#endif /* __PRI_MACRO */

extern SclOtherPriRegister  SclOtherPri;  /* いろいろ */

extern SclSpPriNumRegister  SclSpPriNum;  /* スプライトのプライオリティ番号 */

extern SclBgPriNumRegister  SclBgPriNum;  /* スクロールのプライオリティ番号 */

extern SclSpColMixRegister  SclSpColMix;  /* スプライトの色演算 */

extern SclBgColMixRegister  SclBgColMix;  /* スクロールの色演算 */

extern SclColOffsetRegister SclColOffset; /* カラーオフセット */

extern SclPriBuffDirtyFlags SclPriBuffDirty; /* レジスタバッファのダーティー・フラグ */

/*--- REAL Priority Registers (Pointer) ---*/

extern SclOtherPriRegister  *SclRealOtherPri;

extern SclSpPriNumRegister  *SclRealSpPriNum;

extern SclBgPriNumRegister  *SclRealBgPriNum;

extern SclSpColMixRegister  *SclRealSpColMix;

extern SclBgColMixRegister  *SclRealBgColMix;

extern SclColOffsetRegister *SclRealColOffset;

#endif /* __PRI_GLVAR */

/* ＶＤＰ２ライブラリのプロトタイプ宣言 */
#ifndef	SEGA_SCL_PROTO
#define SEGA_SCL_PROTO

extern	void	SCL_Vdp2Init(void);
extern	void	SCL_SetDisplayMode(Uint8 interlace,Uint8 vertical,Uint8 horizontal);
extern	void	SCL_InitVramConfigTb(SclVramConfig *tp);
extern	void	SCL_InitConfigTb(SclConfig *scfg);
extern	void	SCL_InitLineParamTb(SclLineparam *lp);
extern	void	SCL_SetVramConfig(SclVramConfig *tp);
extern	void	SCL_SetConfig(Uint16 sclnum, SclConfig *scfg);
extern	void	SCL_SetCycleTable(Uint16 *tp);
extern	void	SCL_Open(Uint32 sclnum);
extern	void	SCL_Close(void);
extern	void	SCL_SetLineParam(SclLineparam *lp);
extern	void	SCL_MoveTo(Fixed32 x,Fixed32 y,Fixed32 z);
extern	void	SCL_Move(Fixed32 x,Fixed32 y,Fixed32 z);
extern	void	SCL_Scale(Fixed32 Sx, Fixed32 Sy);
extern	void	SCL_SetColRamMode(Uint32 ComRamMode);
extern	Uint32	SCL_InitRotateTable(Uint32 Address,Uint16 Mode,Uint32 rA,Uint32 rB);
extern	void	SCL_RotateTo(Fixed32 xy,Fixed32 z,Fixed32 disp,Uint16 mode);
extern	void	SCL_Rotate(Fixed32 xy,Fixed32 z,Fixed32 disp);
extern	void	SCL_SetRotateViewPoint(Uint16 x,Uint16 y,Uint16 z);
extern	void    SCL_SetRotateZoom(Fixed32 x,Fixed32 y);
extern	void	SCL_SetRotateCenter(Uint16 x,Uint16 y,Uint16 z);
extern	void	SCL_SetRotateCenterDisp(Uint16 x,Uint16 y);
extern	void	SCL_SetCoefficientData(Uint32 sclnum,Uint16 *data,Uint16 x,Uint16 y);
extern	void	SCL_SetLncl(Uint32 addr,Uint16 tbnum,Uint16 *palNumTb);
extern	void	SCL_SetBack(Uint32 addr,Uint16 dataSize,Uint16 *dataTb);
extern	void	SCL_SetMosaic(Uint32 surface,Uint8 x,Uint8 y);
extern	void	SCL_SetSpriteMode(Uint8 Type, Uint8 ColMode, Uint8 WinMode);
extern	Uint32	SCL_GetColRamMode(void);
extern	void	SCL_SetColRam(Uint32 Object, Uint32 Index,Uint32 num,void *Color);
extern	Uint32	SCL_AllocColRam(Uint32 Surface, Uint32 NumOfColors, Uint8 transparent);
extern	void	SCL_FreeColRam(Uint32 Surface);
extern	void	SCL_SetAutoColChg(Uint32 Surface,Uint32 Interval,Uint32 Index,
			Uint32 NumOfCol,Uint32 NumOfTbl,Uint32 *ChangeTbl);
extern	Uint32	SCL_GetColRamOffset(Uint32 Object);
extern	void	SCL_SetPriority(Uint32 Object, Uint8 Priority);
extern	Uint8	SCL_GetPriority(Uint32 Object);
extern	void	SCL_SetColMixMode(Uint32 ColMixPriority, Uint8 Mode);
extern	void	SCL_SetColMixRate(Uint32 Surfaces, Uint8 Rate);
extern	void	SCL_SetAutoColMix(Uint32 Surfaces,Uint32 Interval,Uint32 Time,
			Uint8 StartRate,Uint8 EndRate);
extern	void	SCL_SetColOffset(Uint32 OffsetReg, Uint32 Surfaces,
			Sint16 red, Sint16 green, Sint16 blue);
extern	void	SCL_IncColOffset(Uint32 OffsetReg,Sint16 red,Sint16 green,Sint16 blue);
extern	void	SCL_SetAutoColOffset(Uint32 OffsetReg,Uint32 Interval,Uint32 Time,
				SclRgb *start,SclRgb *end);
extern	void	SCL_EnableBlur(Uint32 Surface);
extern	void	SCL_DisableBlur(void);
extern	void	SCL_EnableLineCol(Uint32 Surface);
extern	void	SCL_DisableLineCol(Uint32 Surface);
extern	void	SCL_SetWindow(Uint8 win,Uint32 logic,Uint32 enable,Uint32 area,
			Uint16 sx,Uint16 sy,Uint16 ex,Uint16 ey);
extern	void	SCL_SetLineWindow(Uint8 win,Uint32 logic,Uint32 enable,Uint32 area,
			Uint32 addr,Uint32 sy,Uint32 tbSize,SclLineWindowTb *tp);
extern	void	SCL_SetSpriteWindow(Uint32 logic,Uint32 enable,Uint32 area);
extern	void	SCL_SetFrameInterval(Uint16 count);
extern	void	SCL_DisplayFrame(void);
extern	void	SCL_VblankStart(void);
extern	void	SCL_VblankEnd(void);
extern	void	SCL_AbortAutoVe(void);

/* 画面サイズデフォルト */
extern	Uint16	SclDisplayX;
extern	Uint16	SclDisplayY;

/* レジスタバッファ */
extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;
extern	Uint16		SclProcess;

#endif /* SEGA_SCL_PROTO */
