/*----------------------------------------------------------------------------
 *  SCL_vblv.c -- SCL ライブラリ VDP モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by K.M on 1994-11-11 Ver.1.00
 *
 *  このライブラリはＶＤＰ用のＶ－ＢＬＡＮＫ割り込み処理ルーチンで、
 *  以下のルーチンを含む。
 *
 *  SCL_SetFrameInterval  -  フレーム表示インターバル数のセット
 *  SCL_DisplayFrame      -  スプライト、スクロールの表示
 *  SCL_VblInit           -  Ｖ－ＢＬＡＮＫ割り込み処理ワークの初期化
 *  SCL_VblankStart       -  Ｖ－ＢＬＡＮＫ開始割り込み処理ルーチン
 *  SCL_VblankEnd         -  Ｖ－ＢＬＡＮＫ終了割り込み処理ルーチン
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include <machine.h>
#include "sega_xpt.h"
#include "sega_def.h"
#include "sega_mth.h"
#define SEGA_SCL_PROTO
#include "sega_scl.h"
#include "sega_spr.h"


#if 0
/*
 * STATIC DECLARATIONS
 */
static Uint16  SpFrameChgMode    = AUTO_FRAME_CHG; /* Frame Change Mode      */
static Sint32  SpFrameEraseMode  = 1;       /* Frame Erase  Mode             */
static Sint32  VBInterval        = 1;   /* V-Blank Interval for Frame Change */
static Sint32  VBIntervalCounter = 0;   /* V-Blank Interval Counter          */
static Sint32  ReqDisplayFlag1    = 0;   /* Display Request flag              */
static Sint32  ReqDisplayFlag2    = 0;   /* Display Request flag              */
static Sint32  VBIntrDisableDepth = 0;  /* V-Blank Disable Depth             */
static Sint32  frameChgFlag;
#else
/*
 * GLOBAL DECLARATIONS
 */
Uint16  SpFrameChgMode    = AUTO_FRAME_CHG; /* Frame Change Mode      */
Sint32  SpFrameEraseMode  = 1;       /* Frame Erase  Mode             */
Sint32  VBInterval        = 1;   /* V-Blank Interval for Frame Change */
Sint32  VBIntervalCounter = 0;   /* V-Blank Interval Counter          */
volatile Sint32  ReqDisplayFlag1    = 0;   /* Display Request flag              */
volatile Sint32  ReqDisplayFlag2    = 0;   /* Display Request flag              */
Sint32  VBIntrDisableDepth = 0;  /* V-Blank Disable Depth             */
Sint32  frameChgFlag;
#endif

extern Sint32  SpInitialFlag;
extern void   SCL_ScrollShow(void);


/*****************************************************************************
 *
 * NAME:  SCL_SetFrameInterval()  - Set Frame Interval Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16  count       - <i> Ｖ－ＢＬＡＮＫインターバルカウント数
 *                                0      = スプライトのフレーム切り替えをオート
 *                                         にして、インターバルを１にする
 *                                         SCL_DisplayFrame()による同期なし
 *                                1      = スプライトのフレーム切り替えをオート
 *                                         にして、インターバルを１にする
 *                                         SCL_DisplayFrame()による同期あり
 *                                0xffff = インターバルを無効にしてフレーム切り
 *                                         替え要求時に即切り替える
 *                                0xfffe = インターバルを無効にしてフレーム切り
 *                                         替え要求時にＶブランクイレースを行
 *                                         って即切り替える
 *                                その他 = 指定インターバルでフレーム切り替え
 *                                         を行う
 *                                         b15 = 0 : イレースライトを行う
 *                                             = 1 : イレースライトを行わない
 *
 * DESCRIPTION:
 *
 *     フレーム表示のＶ－ＢＬＡＮＫインターバルカウント数を設定する。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_SetFrameInterval(Uint16 count)
{
    /** BEGIN ***************************************************************/
    if(count == 0xffff) {
	SpFrameChgMode = NO_INTER_FRAME_CHG;
        SpFrameEraseMode = 0;          /* Frame Erase Mode Off              */
        SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
                                       /* set frame change to manual mode   */
        VBInterval = count;            /* V-Blank Interval for Frame Change */
    } else
    if(count == 0xfffe) {
	SpFrameChgMode = NO_INTER_VBE_FRAME_CHG;
        SpFrameEraseMode = 0;          /* Frame Erase Mode Off              */
        SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
                                       /* set frame change to manual mode   */
        VBInterval = count;            /* V-Blank Interval for Frame Change */
    } else {
        if(count & 0x8000)
            SpFrameEraseMode = 0;      /* Frame Erase Mode Off              */
        else
            SpFrameEraseMode = 1;      /* Frame Erase Mode On               */
        count &= 0x7fff;
        if(count)
            if(count == 1)
	        SpFrameChgMode = AUTO_FRAME_CHG;
	    else
	        SpFrameChgMode = MANUAL_FRAME_CHG;
        else
	    SpFrameChgMode = AUTO_FRAME_CHG;
        VBInterval = count;            /* V-Blank Interval for Frame Change */
        if(SpFrameChgMode == AUTO_FRAME_CHG)
            SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_AUTO | SpFbcrMode);
                                       /* set frame change to auto mode     */
        else
            SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
                                       /* set frame change to manual mode   */
    }
}


/*****************************************************************************
 *
 * NAME:  SCL_DisplayFrame()  - Display Frame
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 *
 * DESCRIPTION:
 *
 *     フレーム表示要求フラグをオンにして、Ｖ－ＢＬＡＮＫ割り込みルーチン
 *     によるスプライトおよびスクロールのフレーム表示完了を待つ。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_DisplayFrame(void)
{

    /** BEGIN ***************************************************************/
    if(VBInterval) {
        ReqDisplayFlag1 = 1;
        while(ReqDisplayFlag1);
        while(ReqDisplayFlag2);
    }
}


/*****************************************************************************
 *
 * NAME:  SCL_VblInit()  - Initial VDP V-Blank Routine
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ＶＤＰ Ｖ－ＢＬＡＮＫ割り込みルーチン用ワークエリアの初期化。
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_VblInit(void)
{

    /** BEGIN ***************************************************************/
	SpFrameChgMode     = AUTO_FRAME_CHG;
	SpFrameEraseMode   = 1;
	VBInterval         = 0;
	VBIntervalCounter  = 0;
	ReqDisplayFlag1    = 0;
	ReqDisplayFlag2    = 0;
	VBIntrDisableDepth = 0;
	frameChgFlag       = 0;
}


/*****************************************************************************
 *
 * NAME:  SCL_VblankStart()  - V-Blank Start Interrupt Routine for VDP
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 *
 * DESCRIPTION:
 *
 *     Ｖ－Ｂｌａｎｋスタート割り込み処理として以下の処理を行う
 *     (1) スプライトフレームバッファのイレーズ操作
 *     (2) フレーム表示要求フラグがオンでＶ－Ｂｌａｎｋインターバル数
 *         に達した場合、スクロールパラメータの転送とスプライトのフレーム
 *         を切り替える
 *
 *     [本ルーチンが参照・設定するグローバル変数]
 *
 *     (1) IntervalCounter -  Ｖ－Ｂｌａｎｋ割り込み数のカウンタ。
 *                            VBL_SetFrameInterval() および本ルーチンがクリア。
 *
 *     (2) Interval        -  フレーム表示インターバルＶ－Ｂｌａｎｋ
 *                               割り込み数。 VBL_SetFrameInterval() が設定。
 *
 *     (3) SpFrameChgMode  -  スプライトフレームチェンジモード
 *                               VBL_SetFrameInterval() が設定。
 *
 *     (4) ReqDisplayFlag1 -  フレーム表示要求フラグ。
 *                               DisplayFrame() が設定。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *     ユーザがＶ－Ｂｌａｎｋ割り込みルーチンを登録してフレーム表示切り替え
 *     を行う場合、本割り込みルーチンを参考に作成すること
 *
 *****************************************************************************
 */
void SCL_VblankStart(void)
{
    /** BEGIN ***************************************************************/
/*    frameChgFlag = 0;*/
    if(SpFrameChgMode == NO_INTER_FRAME_CHG) {
        if(ReqDisplayFlag1) {
            SCL_ScrollShow();             /* スクロールへのパラメータ設定    */
            frameChgFlag = 2;
        }
    } else  if(SpFrameChgMode == NO_INTER_VBE_FRAME_CHG) {
        if(ReqDisplayFlag1) {             /* set V-Blank Erase ON            */
            SPR_WRITE_REG(SPR_W_TVMR, SpTvMode | 0x0008);
            SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
            SCL_ScrollShow();             /* スクロールへのパラメータ設定    */
            frameChgFlag = 3;
        }
    } else  if(SpFrameChgMode == AUTO_FRAME_CHG) {
	SCL_ScrollShow();
        ReqDisplayFlag1 = 0;
    }else {
        VBIntervalCounter++;
        if(SpFrameEraseMode == ON) {
            if((VBInterval - 1) <= VBIntervalCounter) {
               SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_ERASE | SpFbcrMode);
                                                /* 次フレームイレーズオン*/
            }
        }
        if(VBInterval <= VBIntervalCounter) {
            if(ReqDisplayFlag1) {
                SCL_ScrollShow();          /* スクロールへのパラメータ設定   */
                frameChgFlag = 1;
            }
            VBIntervalCounter = 0;
        }
    }
}


/*****************************************************************************
 *
 * NAME:  SCL_VblankEnd()  - V-Blank End Interrupt Routine for VDP
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 *
 * DESCRIPTION:
 *
 *     何もしない。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_VblankEnd(void)
{
	Uint16  tvStat;
    
	/** BEGIN ***************************************************************/
	if(SpDie) {
		tvStat = SPR_SCLREAD_REG(SPR_R_TVSTAT);
		if(tvStat & 2)
			SPR_WRITE_REG(SPR_W_FBCR, 0x000c | SpFbcrMode);
	        else
			SPR_WRITE_REG(SPR_W_FBCR, 0x0008 | SpFbcrMode);
                         /* スプライトのフレームバッファの切り替え */
			frameChgFlag = 0;
			ReqDisplayFlag1 = 0;
	} else if(frameChgFlag) {
/*		if((frameChgFlag == 2) && (SpInitialFlag != 0))  SPR_WaitDrawEnd();*/
		if((frameChgFlag == 2) && (SpInitialFlag != 0) 
	       	&& ((SPR_READ_REG(SPR_R_EDSR) & 0x0002) == 0) )	ReqDisplayFlag2 = 1;
		else{
			if(frameChgFlag == 3)
				SPR_WRITE_REG(SPR_W_TVMR, SpTvMode);
				/* set V-Blank Erase OFF    */
			else
				SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
	                                  /* スプライトのフレームバッファの切り替え */
			frameChgFlag = 0;
			ReqDisplayFlag2 = 0;
		}
		ReqDisplayFlag1 = 0;
	}
}

/*  end of file */
