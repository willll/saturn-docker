/*-----------------------------------------------------------------------------
 *  FILE: per_prt.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      ペリフェラルプロトコル実行ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      ペリフェラルプロトコル実行プログラムの関数を使用するプログラムへインク
 *      ルードすること。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *        <EXTERN>
 *          PER_GET_TIM             -   時刻取得
 *          PER_GET_SYS             -   システムデータ取得
 *          PER_GET_HOT_RES         -   ホットリセット取得
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_PRT_H
#define PER_PRT_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/* conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * USER SUPPLIED INCLUDE FILES
 */

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* ペリフェラルID */
#define PER_ID_NCON     0xff                    /* 未接続                    */
#define PER_ID_UNKOWN   0xf0                    /* SMPC UNKNOWN              */
#define PER_ID_DGT      0x00                    /* デジタルデバイス          */
#define PER_ID_ANL      0x10                    /* アナログデバイス          */
#define PER_ID_PNT      0x20                    /* ポインティングデバイス    */
#define PER_ID_KBD      0x30                    /* キーボード　　　          */
#define PER_ID_M3BP     0xe1                    /* メガドライブ3ボタンパッド */
#define PER_ID_M6BP     0xe2                    /* メガドライブ6ボタンパッド */

/* ペリフェラルサイズ */
#define PER_SIZE_DGT    2                       /* デジタルデバイス          */
#define PER_SIZE_ANL    5                       /* アナログデバイス          */
#define PER_SIZE_PNT    5                       /* ポインティングデバイス    */
#define PER_SIZE_KBD    4                       /* キーボード　　　          */
#define PER_SIZE_M3BP   1                       /* メガドライブ3ボタンパッド */
#define PER_SIZE_M6BP   2                       /* メガドライブ6ボタンパッド */

/* イントバック種別 */
#define PER_KD_SYS      0                       /* システムデータ取得        */
#define PER_KD_PER      1                       /* ペリフェラルデータ取得    */
#define PER_KD_PERTIM   2                       /* ペリフェラルデータ取得＋  */
                                                /* 時刻データ取得            */
/* ビット位置 ****************************************************************/
/* デジタルデバイスデータ型	 */
#define	PER_DGT_U   (1 << 12)                   /* UP                        */
#define	PER_DGT_D   (1 << 13)                   /* DOWN                      */
#define	PER_DGT_R   (1 << 15)                   /* RIGHT                     */
#define	PER_DGT_L   (1 << 14)                   /* LEFT                      */
#define	PER_DGT_A   (1 << 10)                   /* A                         */
#define	PER_DGT_B   (1 << 8)                    /* B                         */
#define	PER_DGT_C   (1 << 9)                    /* C                         */
#define	PER_DGT_S   (1 << 11)                   /* START                     */
#define	PER_DGT_X   (1 << 6)                    /* X                         */
#define	PER_DGT_Y   (1 << 5)                    /* Y                         */
#define	PER_DGT_Z   (1 << 4)                    /* Z                         */
#define	PER_DGT_TR  (1 << 7)                    /* トリガ RIGHT              */
#define	PER_DGT_TL  (1 << 3)                    /* トリガ LEFT               */

/* ポインティングデバイスデータ型 */
/***** データ */
#define PER_PNT_R   (1 << 1)                    /* RIGHT                     */
#define PER_PNT_L   (1 << 0)                    /* LEFT                      */
#define PER_PNT_MID (1 << 2)                    /* MIDDLE                    */
#define PER_PNT_CNT (1 << 3)                    /* CENTER                    */
#define PER_PNT_XO  (1 << 6)                    /* X軸オーバフロー           */
#define PER_PNT_YO  (1 << 7)                    /* Y軸オーバフロー           */

/* キーボードデバイスデータ型 */
/***** 特殊キー */
#define PER_KBD_CL  (1 << 6)                    /* Caps Lock                 */
#define PER_KBD_NL  (1 << 5)                    /* Num Lock                  */
#define PER_KBD_SL  (1 << 4)                    /* Scrool Lock               */
#define PER_KBD_MK  (1 << 3)                    /* Make                      */
#define PER_KBD_BR  (1 << 0)                    /* Break                     */

/* メガドライブ3ボタンパッドデータ型 */
#define	PER_M3BP_U  PER_DGT_U                   /* UP                        */
#define	PER_M3BP_D  PER_DGT_D                   /* DOWN                      */
#define	PER_M3BP_R  PER_DGT_R                   /* RIGHT                     */
#define	PER_M3BP_L  PER_DGT_L                   /* LEFT                      */
#define	PER_M3BP_A  PER_DGT_A                   /* A                         */
#define	PER_M3BP_B  PER_DGT_B                   /* B                         */
#define	PER_M3BP_C  PER_DGT_C                   /* C                         */
#define	PER_M3BP_S  PER_DGT_S                   /* START                     */

/* メガドライブ6ボタンパッドデータ型 */
#define	PER_M6BP_U  PER_DGT_U                    /* UP                        */
#define	PER_M6BP_D  PER_DGT_D                    /* DOWN                      */
#define	PER_M6BP_R  PER_DGT_R                    /* RIGHT                     */
#define	PER_M6BP_L  PER_DGT_L                    /* LEFT                      */
#define	PER_M6BP_A  PER_DGT_A                    /* A                         */
#define	PER_M6BP_B  PER_DGT_B                    /* B                         */
#define	PER_M6BP_C  PER_DGT_C                    /* C                         */
#define	PER_M6BP_S  PER_DGT_S                    /* START                     */
#define	PER_M6BP_X  PER_DGT_X                    /* X                         */
#define	PER_M6BP_Y  PER_DGT_Y                    /* Y                         */
#define	PER_M6BP_Z  PER_DGT_Z                    /* Z                         */
#define	PER_M6BP_MD PER_DGT_TR                   /* MODE                      */

/* システムデータ出力 */
/* システムステータス */
#define PER_SS_DOTSEL   (1 <<  6)               /* カートリッジコード        */
#define PER_SS_SYSRES   (1 <<  1)               /* エリアコード              */
#define PER_SS_MSHNMI   (1 <<  3)               /* システムステータス        */
#define PER_SS_SNDRES   (1 <<  0)               /* SMPCメモリ                */
#define PER_SS_CDRES    (1 << 14)               /* SMPCステータス            */

/* SMPCメモリ */
/***** マスク */
#define PER_MSK_LANGU   (0xf << 0)              /* 言語マスク                */
#define PER_MSK_SE      (0x1 << 8)              /* SE                        */
#define PER_MSK_STEREO  (0x1 << 9)              /* STEREO or MONO            */
#define PER_MSK_HELP    (0x1 << 10)             /* HELP                      */
/***** 言語 */
#define PER_ENGLISH     0x0                     /* English(英語)             */
#define PER_DEUTSCH     0x1                     /* Deutsch(ドイツ語)         */
#define PER_FRANCAIS    0x2                     /* francais(フランス語)      */
#define PER_ESPNOL      0x3                     /* Espnol(スペイン語)        */
#define PER_ITALIANO    0x4                     /* Italiano(イタリア語)      */
#define PER_JAPAN       0x5                     /* Japan(日本語)             */

/* SMPCステータス */
#define PER_SS_RESET    (1 <<  6)               /* リセットマスク状態        */
#define PER_SS_SETTIM   (1 <<  7)               /* 時刻設定状態              */

/* アクセスマクロ ************************************************************/
/* デジタルデバイスデータ型 */
#define PER_DGT(data)   ((PerDgtInfo *)(data))

/* アナログデバイスデータ型  */
#define PER_ANL(data)   ((PerAnlInfo *)(data))

/* ポインティングデバイスデータ型 */
#define PER_PNT(data)   ((PerPntInfo *)(data))

/* キーボードデバイスデータ型 */
#define PER_KBD(data)   ((PerKbdInfo *)(data))

/* メガドライブ3ボタンパッドデータ型 */
#define PER_M3BP(data)  ((PerM3bpInfo *)(data))

/* メガドライブ6ボタンパッドデータ型 */
#define PER_M6BP(data)  ((PerM6bpInfo *)(data))

/* システムデータ出力 */
#define PER_GS_CC(data)         ((data)->cc)    /* カートリッジコード        */
#define PER_GS_AC(data)         ((data)->ac)    /* エリアコード              */
#define PER_GS_SS(data)         ((data)->ss)    /* システムステータス        */
#define PER_GS_SM(data)         ((data)->sm)    /* SMPCメモリ                */
#define PER_GS_SMPC_STAT(data)  ((data)->stat)  /* SMPCステータス            */

/* 定数 **********************************************************************/
#define PER_HOT_RES_ON  0x1                     /* ホットリセットON          */
#define PER_HOT_RES_OFF 0x0                     /* ホットリセットOFF         */

/* イントバック実行状態      */
#define PER_INT_OK      0x0                     /* 正常                      */
#define PER_INT_ERR     0x1                     /* エラー                    */

/******************************************************************************
 *
 * NAME:    PER_GET_TIM             -   時刻取得
 *
 ******************************************************************************
 */
#define PER_GET_TIM()  (per_get_time_adr)

/******************************************************************************
 *
 * NAME:    PER_GET_SYS             -   システムデータ取得
 *
 ******************************************************************************
 */

#define PER_GET_SYS()   ((per_set_sys_flg == OFF) ? NULL : &per_get_sys_data)

#ifndef _PER_HOT_ENA
/******************************************************************************
 *
 * NAME:    PER_GET_HOT_RES         -   ホットリセット取得
 *
 ******************************************************************************
 */
#define PER_GET_HOT_RES()  (per_hot_res)
#endif  /* _PER_HOT_ENA */

/*
 * TYPEDEFS
 */
/* デバイスデータ型 */
typedef Uint16  PerDgtData;                     /* デジタルデバイスデータ型  */

typedef struct  {                               /* アナログデバイスデータ型  */
    PerDgtData  dgt;                            /* デジタルデバイスデータ型  */
    Sint16      x;                              /* X軸絶対値(0～255)         */
    Sint16      y;                              /* Y軸絶対値(0～255)         */
    Sint16      z;                              /* Z軸絶対値(0～255)         */
}PerAnlData;

typedef struct  {                       /* ポインティングデバイスデータ型    */
    PerDgtData  dgt;                            /* デジタルデバイスデータ型  */
    Uint16      data;                           /* データ                    */
    Sint16      x;                              /* X軸移動量(-128～127)      */
    Sint16      y;                              /* Y軸移動量(-128～127)      */
}PerPntData;

typedef struct  {                           /* キーボードデバイスデータ型    */
    PerDgtData  dgt;                            /* デジタルデバイスデータ型  */
    Uint8       skey;                           /* 特殊キー                  */
    Uint8       key;                            /* キー                      */
}PerKbdData;

typedef Uint8   PerM3bpData;            /* メガドライブ3ボタンパッドデータ型 */
typedef Uint16  PerM6bpData;            /* メガドライブ6ボタンパッドデータ型 */
typedef Uint8   PerId;                  /* ペリフェラルID                    */
typedef Uint8   PerSize;                /* ペリフェラルサイズ                */
typedef Uint8   PerKind;                        /* イントバック種別          */
typedef Uint16  PerNum;                         /* 必要ペリフェラル数        */

/* ペリフェラルデータ出力 */
typedef struct  {                               /* デジタルデバイス          */
    PerDgtData  data;                           /* 現在ペリフェラルデータ    */
    PerDgtData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerDgtInfo;

typedef struct  {                               /* アナログデバイス          */
    PerAnlData  data;                           /* 現在ペリフェラルデータ    */
    PerAnlData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerAnlInfo;

typedef struct  {                               /* ポインティングデバイス    */
    PerPntData  data;                           /* 現在ペリフェラルデータ    */
    PerPntData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerPntInfo;

typedef struct  {                               /* キーボードデバイス        */
    PerKbdData  data;                           /* 現在ペリフェラルデータ    */
    PerKbdData  push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerKbdInfo;

typedef struct  {                               /* メガドライブ3ボタンパッド */
    PerM3bpData data;                           /* 現在ペリフェラルデータ    */
    PerM3bpData push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerM3bpInfo;

typedef struct  {                               /* メガドライブ6ボタンパッド */
    PerM6bpData data;                           /* 現在ペリフェラルデータ    */
    PerM6bpData push;                           /* 前回未押下現在押下ボタン  */
    PerId       id;                             /* ペリフェラルID            */
}PerM6bpInfo;

typedef struct  {                               /* システム出力              */
    Uint8   cc;                                 /* カートリッジコード        */
    Uint8   ac;                                 /* エリアコード              */
    Uint16  ss;                                 /* システムステータス        */
    Uint32  sm;                                 /* SMPCメモリ                */
    Uint8   stat;                               /* SMPCステータス            */
}PerGetSys;

typedef void PerGetPer;                         /* ペリフェラルデータ出力    */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
#ifndef _PER_HOT_ENA
extern Uint8 per_hot_res;                       /* ホットリセット状態        */
#endif  /* _PER_HOT_ENA */
extern Uint8 *per_get_time_adr;                 /* 時刻データ取得先頭アドレス*/
extern PerGetSys per_get_sys_data;              /* システムデータ取得格納    */
extern Uint8 per_set_sys_flg;                   /* システムデータ設定フラグ  */
extern Uint8 per_time_out_flg;                  /* タイムアウトフラグ        */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
Uint32 PER_Init(PerKind, PerNum, PerId, PerSize, Uint32 *, Uint8);
                                                /* イントバック初期化        */
Uint32 PER_GetPer(PerGetPer **);                /* ペリフェラルデータ取得    */
PerGetSys PER_GetSys(void);                     /* システムデータ取得        */
void PER_IntFunc(void);                         /* SMPC割り込み処理          */

#endif  /* ifndef PER_PRT_H */
