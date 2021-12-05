/*-----------------------------------------------------------------------------
 *  FILE: per_def.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      SMPCハード関連ヘッダファイル
 *
 *  DESCRIPTION:
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_DEF_H
#define PER_DEF_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/* smpc conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * USER SUPPLIED INCLUDE FILES
 */

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* レジスタアドレス **********************************************************/
#ifndef _DEB
#define REG_PDRA    ((Uint8 *)0x20100075)       /* PDRA                      */
#define REG_DDRA    ((Uint8 *)0x20100079)       /* DDRA                      */
#define REG_PDRB    ((Uint8 *)0x20100077)       /* PDRB                      */
#define REG_DDRB    ((Uint8 *)0x2010007b)       /* DDRB                      */
#define REG_IOSEL   ((Uint8 *)0x2010007d)       /* IOSELB,IOSELA             */
#define REG_EXLE    ((Uint8 *)0x2010007f)       /* EXLEB,EXLEA               */
#else
#define REG_PDRA    ((Uint8 *)0x06030075)       /* PDRA                      */
#define REG_DDRA    ((Uint8 *)0x06030079)       /* DDRA                      */
#define REG_PDRB    ((Uint8 *)0x06030077)       /* PDRB                      */
#define REG_DDRB    ((Uint8 *)0x0603007b)       /* DDRB                      */
#define REG_IOSEL   ((Uint8 *)0x0603007d)       /* IOSELB,IOSELA             */
#define REG_EXLE    ((Uint8 *)0x0603007f)       /* EXLEB,EXLEA               */
#endif /* _DEB */

/* ビット位置 ****************************************************************/
/***** IOSEL */
#define B_IOSELB    (1 << 1)                    /* IOSELB                    */
#define B_IOSELA    (1 << 0)                    /* IOSELA                    */
/***** EXLE */
#define B_EXLEB     (1 << 1)                    /* EXLEB                     */
#define B_EXLEA     (1 << 0)                    /* EXLEA                     */
/***** ステータスレジスタ */
#define B_SR_PDE    (1 << 5)                    /* 残りのPCデータ            */
#define B_SR_RESB   (1 << 4)                    /* ホットリセットボタン状態  */

/* 定数 **********************************************************************/
/***** イントバック関連 */
/********* IREG0 */
#define SM_GET_ON   (0x1)                       /* SMデータを返す            */
#define SM_GET_OFF  (0x0)                       /* SMデータを返さない        */
/********* IREG0 */
/************* ポート1モード */
#define P1MD_15     (0x0 << 4)                  /* 15バイトモード            */
#define P1MD_255    (0x1 << 4)                  /* 255バイトモード           */
#define P1MD_0      (0x3 << 4)                  /* 0バイトモード             */
/************* ポート2モード */
#define P2MD_15     (0x0 << 6)                  /* 15バイトモード            */
#define P2MD_255    (0x1 << 6)                  /* 255バイトモード           */
#define P2MD_0      (0x3 << 6)                  /* 0バイトモード             */
/************* ペリフェラルデータイネーブル */
#define PEN_RET     (0x1 << 3)                  /* PCデータを返す            */
#define PEN_NRET    (0x0 << 3)                  /* PCデータを返さない        */
/************* 取得時間最適化 */
#define OPE_ON      (0x0 << 1)                  /* 最適化を行う              */
#define OPE_OFF     (0x1 << 1)                  /* 最適化を行わない          */
/******** ステータスレジスタ */
#define SR_PDE_ON   (0x1 << 5)                  /* 残りペリフェラルあり */
#define SR_PDE_OFF  (0x0 << 5)                  /* 残りペリフェラルなし */

/*
 * TYPEDEFS
 */
/* デバイスデータ型 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
#endif  /* ifndef PER_DEF_H */
