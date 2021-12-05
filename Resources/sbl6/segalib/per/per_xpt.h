/*-----------------------------------------------------------------------------  *  FILE: per_xpt.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      システム／ペリフェラルライブラリ共通ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      システム／ペリフェラルライブラリを構成するプログラムファイルのコンパイ
 *      ル時に必ずインクルードすること。
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_XPT_H
#define PER_XPT_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/* conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/**** レジスタアドレス *******************************************************/
#ifndef _DEB
#define PER_REG_COMREG  ((Uint8 *)0x2010001f)   /* コマンドレジスタ          */
#define PER_REG_SR      ((Uint8 *)0x20100061)   /* ステータスレジスタ        */
#define PER_REG_SF      ((Uint8 *)0x20100063)   /* ステータスフラグ          */
#define PER_REG_IREG    ((Uint8 *)0x20100001)   /* IREG                      */
#define PER_REG_OREG    ((Uint8 *)0x20100021)   /* OREG                      */
#else
#define PER_REG_COMREG  ((Uint8 *)0x0603001f)   /* コマンドレジスタ          */
#define PER_REG_SR      ((Uint8 *)0x06030061)   /* ステータスレジスタ        */
#define PER_REG_SF      ((Uint8 *)0x06030063)   /* ステータスフラグ          */
#define PER_REG_IREG    ((Uint8 *)0x06030001)   /* IREG                      */
#define PER_REG_OREG    ((Uint8 *)0x06030021)   /* OREG                      */
#endif /* _DEB */
/**** ビット位置 *************************************************************/
#define PER_B_SF        0x1                     /* ステータスフラグ          */

/**** 関数形式マクロ *********************************************************/
#define PER_PokeByte(address,data)  (*(Uint8 *)(address) = (Uint8)(data))
                                                    /* ﾊﾞｲﾄﾃﾞｰﾀｱﾄﾞﾚｽ直接書き-*/
                                                    /* 込み                  */
#define PER_PeekByte(address)   (*(Uint8 *)(address))
                                                    /* ﾊﾞｲﾄﾃﾞｰﾀｱﾄﾞﾚｽ直接読み-*/
                                                    /* 込み                  */

/*
 * TYPEDEFS
 */

#endif  /* ifndef PER_XPT_H */
