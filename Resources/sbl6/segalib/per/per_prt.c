/*-----------------------------------------------------------------------------
 *  FILE:   PER_PRT.C
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      イントバックプログラムファイル。
 *
 *  DESCRIPTION:
 *
 *      イントバック系の関数である。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *        <EXTERN>
 *          PER_Init                -   イントバック初期化
 *          PER_GetPer              -   ペリフェラルデータ取得
 *          PER_GetTim              -   時刻取得
 *          PER_GetSys              -   システムデータ取得
 *          PER_IntFunc             -   SMPC割り込み処理
 *
 *        <INTER>
 *          JudgeGetPerNum          -   ペリフェラル取得数判定
 *          JudgeOreg               -   OREG判定
 *          MoveBdryData            -   境界データ転送処理
 *          SetPerData              -   ペリフェラルデータ設定
 *          InitIntBackPer          -   ペリフェラルイントバック初期化
 *          GoIntBack               -   イントバックコマンド発行
 *          AnyInitPerData          -   複数ペリフェラルデータ初期化(PUSH処理)
 *          InitPerData             -   ペリフェラルデータ初期化(PUSH処理も)
 *          InitBdryData            -   境界ワーク初期化
 *          SetPerId                -   ペリフェラルID設定
 *
 *  CAVEATS:
 *      ●マウス特別処理
 *          SMPCマウス取得ではデジタルデータが付加されない。ライブラリではデジ
 *          タルデータを付加する。
 *
 *  AUTHOR(S)
 *
 *      1994-07-21  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *      1994-08-23  N.T Ver.1.03
 *		  1994-11-03  A.S - integrate into vshell4
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "machine.h"
/* smpc conflicts with shtypes.h
#include <sega_xpt.h>
*/
#include "sega_int.h"
/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "per_xpt.h"
#include "per_def.h"
#include "per_smp.h"
#include "per_prt.h"
#ifdef _BUG
#define _STN_PAD
#include "perdxpt.h"
#include "perddgt.h"
#include "perdprt.h"
#endif /* _BUG */

/*
 * GLOBAL DECLARATIONS
 */
Uint8 per_time_out_flg = 0;                     /* タイムアウトフラグ        */
                                                /* 0 = タイムアウトなし      */
                                                /* 0 < タイムアウトあり      */

#ifndef _PER_HOT_ENA
Uint8 per_hot_res = PER_HOT_RES_OFF;            /* ホットリセット状態        */
#endif  /* _PER_HOT_ENA */
Uint8 *per_get_time_adr;                        /* 時刻データ取得先頭アドレス*/
PerGetSys per_get_sys_data;                     /* システムデータ取得格納    */
Uint8 per_set_sys_flg;                          /* システムデータ設定フラグ  */

/*
 * LOCAL DEFINES/MACROS
 */
/* イントバックIREG0 */
#define IREG0_SYS   0x01                        /* システムデータ取得        */
#define IREG0_NSYS  0x00                        /* システムデータ未取得      */

/* イントバックIREG0(コンティニュ時) */
#define IREG0_CONT  (1 << 7)                    /* システムデータ取得        */
#define IREG0_BR    (1 << 6)                    /* システムデータ未取得      */

/* イントバックIREG1 */
/***** ポート1モード */
#define IREG1_P1MD_15   0x00                    /* 15バイトモード            */
#define IREG1_P1MD_255  (1 << 4)                /* 255バイトモード           */
/***** ポート2モード */
#define IREG1_P2MD_15   0x00                    /* 15バイトモード            */
#define IREG1_P2MD_255  (1 << 6)                /* 255バイトモード           */
/***** ペリフェラルデータイネーブル */
#define IREG1_PEN_RET   (1 << 3)                /* ペリフェラルデータを返す  */
#define IREG1_PEN_NRET  0x00                  /* ペリフェラルデータを返さない*/
/***** 取得時間最適化 */
#define IREG1_OPE_ON    0x00                    /* 最適化を行う              */
#define IREG1_OPE_OFF   (1 << 1)                /* 最適化を行わない          */

/* タイムアウト処理 */
#define TIME_OUT_MAX    3                       /* タイムアウトMAX           */

/* ホットリセット処理 */
#define HOT_RES_MAX     3                       /* ホットリセットMAX         */

/* ポインティングデバイス取得処理 (start)*************************************/
#define MV_PNT_UD   10                          /* 必要非移動量(UP,DOWN)     */
#define MV_PNT_RL   10                          /* 必要非移動量(RIGHT,LEFT)  */

#define ID_MMUS 0xe3                            /* ﾒｶﾞﾄﾞﾗｲﾌﾞﾏｳｽID            */

#define PNT_YO  (1 <<  7)                       /* ビット位置(data)          */
#define PNT_XO  (1 <<  6)
#define PNT_YS  (1 <<  5)
#define PNT_XS  (1 <<  4)
#define PNT_S   (1 <<  3)
#define PNT_MID (1 <<  2)
#define PNT_R   (1 <<  1)
#define PNT_L   (1 <<  0)

#define DGT_0_R   (1 <<  7)                     /* ビット位置(data)          */
#define DGT_0_L   (1 <<  6)
#define DGT_0_D   (1 <<  5)
#define DGT_0_U   (1 <<  4)
#define DGT_0_S   (1 <<  3)
#define DGT_0_A   (1 <<  2)
#define DGT_0_C   (1 <<  1)
#define DGT_0_B   (1 <<  0)

#define DGT_1_TR  (1 <<  7)                     /* ビット位置(data)          */
#define DGT_1_X   (1 <<  6)
#define DGT_1_Y   (1 <<  5)
#define DGT_1_Z   (1 <<  4)
#define DGT_1_TL  (1 <<  3)
/* ポインティングデバイス取得処理 (end)***************************************/

/* ペリフェラルデータ初期化 */
/***** 未接続 ********************/
#define INIT_NCON   ~0x0
/***** デジタルデバイス **********/
#define INIT_DGT    ~0x0
/***** アナログデバイス **********/
#define INIT_ANL_X       0x0
#define INIT_ANL_Y       0x0
#define INIT_ANL_Z       0x0
/***** ポインティングデバイス ****/
#define INIT_PNT_DATA   ~0x0
#define INIT_PNT_X       0x0
#define INIT_PNT_Y       0x0
/***** キーボード ****************/
#define INIT_KBD_SKEY   ~0x0
#define INIT_KBD_KEY    ~0x0
/***** メガドライブ3ボタンパッド */
#define INIT_M3BP   ~0x0
/***** メガドライブ6ボタンパッド */
#define INIT_M6BP   ~0x0

/* PER_IntFunc() start *******************************************************/
/* 終了種別 */
#define END_END         0                       /* 終了処理                  */
#define END_BREAK       1                       /* ブレイク処理              */
#define END_CONT        2                       /* コンティニュ処理          */

#define BODY_CONECT_MAX 2                       /* 本体コネクタ数MAX         */
#define OREG_MAX        32                      /* OREG MAX                  */
#define REG_OREG_MAX    (PER_REG_OREG + OREG_MAX * 2)

#define BDRY_OREG       2                       /* OREGバウンダリ            */
#define BDRY_WORK       1                       /* ワークバウンダリ          */

/* ポートステータス */
/***** マスク */
#define PS_MSK_ID   0xf0                        /* ﾏﾙﾁﾀｯﾌﾟID(マスクビット)   */
#define PS_MSK_CON  0x0f                        /* コネクタ数(マスクビット)  */
/***** 定数 */
/********* コネクタ数 */
#define PS_CON_NON  0x00                        /* 未接続 or SMPC UNKNOW     */
#define PS_CON_DIR  0x01                        /* 直接ペリフェラル          */
/********* ﾏﾙﾁﾀｯﾌﾟID */
#define PS_ID_DIRNON    0xf0                    /* 未接続 or 直接ペリフェラル*/

/* ペリフェラルID */
/***** マスク */
#define PI_MSK_ID   0xf0                        /* ペリフェラルID(ﾏｽｸﾋﾞｯﾄ)   */
#define PI_MSK_CON  0x0f                        /* コネクタ数(ﾏｽｸﾋﾞｯﾄ)       */
/***** 定数 */
/********* コネクタ数 */
#define PI_CON_NON_PER  0x0f                    /* 未接続 or ペリフェラル    */
#define PI_CON_EXP      0x00                    /* 拡張ﾍﾟﾘﾌｪﾗﾙｻｲｽﾞ有り       */
/********* ﾍﾟﾘﾌｪﾗﾙID */
#define PI_ID_M5            0xe0                /* M5(ﾒｶﾞﾄﾞﾗｲﾌﾞ)ﾍﾟﾘﾌｪﾗﾙ      */
#define PI_ID_NON_UNKNOWN   0xf0                /* 未接続 or UNKNOWN         */
/* PER_IntFunc() end *********************************************************/

/* SMコマンド*/
#define SM_INTBACK  0x10                        /* イントラプトバック        */

/* 関数形式マクロ ************************************************************/
#define GET_PER_DATA(data)  (*(get_per_data_adr + (data) * bdry_size))
#define ARY_REG_OREG(data)  (*(PER_REG_OREG + (data) * 2))
#define ARY_REG_IREG(data)  (*(PER_REG_IREG + (data) * 2))
#define SET_PUSH(old, now)  ((~(old)) | (now))

/*
 * STATIC DECLARATIONS
 */

static Uint8 intback_ireg[3] = {0, 0, 0xf0};    /* イントバック用IREG        */
static Uint8 now_cont;
static PerKind intback_kind;                    /* イントバック種別          */
static Uint8 intback_v_blank;                   /* V-BLANKスキップ数         */
static PerNum intback_num;                      /* 必要ペリフェラル数        */
static PerSize intback_size;                    /* 必要ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀｻｲｽﾞ       */
static PerId intback_id;                        /* ペリフェラルID            */
static void *intback_work;                      /* ワーク領域                */


static int get_per_cnt;                         /* ペリフェラル取得カウンタ  */
static int v_blank_cnt;                         /* V-BLANKカウンタ           */
static void *get_per_adr;                       /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ取得先頭ｱﾄﾞﾚｽ  */
static void *set_per_adr;                       /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ設定先頭ｱﾄﾞﾚｽ  */
static void *change_work;                       /* トグルワーク              */
static Uint8 *bdry_work_adr;                    /* ﾊﾞｳﾝﾀﾞﾘ処理用ﾜｰｸｱﾄﾞﾚｽ     */

static Uint8 bdry_size;                         /* バウンダリサイズ          */
static Uint8 *get_per_data_adr;                 /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ取得対象ｱﾄﾞﾚｽ  */
static Uint8 *set_per_data_adr;                 /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ設定対象ｱﾄﾞﾚｽ  */

static Uint8 set_time_flg;                      /* 時刻データ設定フラグ      */
static Uint8 time_data[2][7];                   /* 時刻データ格納領域        */
static Uint8 *set_time_adr;                     /* 時刻データ設定先頭アドレス*/
                                                /* (取得はグローバル変数)    */

#ifndef _PER_HOT_ENA
static int hot_res_cnt = 0;                     /* ホットリセットカウンタ    */
#endif  /* _PER_HOT_ENA */

static Uint8 *get_oreg_adr;            /* 取得対象OREGアドレス      */
static int body_conect_cnt;                     /* 本体コネクタカウンタ      */
static Uint8 end_flg;                           /* 終了フラグ                */
static int remain_conect_cnt;                   /* 残コネクタ数カウンタ      */

static PerId get_mus_id;                        /* マウスID                  */
static Uint8 get_mus_data[5];                   /* マウスデータ              */
static PerSize backup_size;

static int get_per_size;                        /* 取得ペリフェラルサイズ &  */
                                                /* ペリフェラルデータ取得数  */
static int get_per_skip_size;                   /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀｽｷｯﾌﾟ数        */

static Uint8 get_per_id_flg;                    /* ﾍﾟﾘﾌｪﾗﾙID取得フラグ       */
static Uint8 get_exp_per_size_flg;              /* 拡張ﾍﾟﾘﾌｪﾗﾙｻｲｽﾞ取得フラグ */
static Uint8 set_bdr_flg;                       /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ境界合わせﾌﾗｸﾞ */
static Uint8 get_per_data_flg;                  /* ペリフェラルデータ取得処理*/
static Uint8 set_per_data_flg;                  /* ペリフェラルデータ設定処理*/
static Uint8 skip_per_data_flg;                 /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀｽｷｯﾌﾟ処理      */

#ifdef _BUG
static Uint8 bug_dgt_con[2];                    /* 1Pデジタルデバイス取得ｺﾈｸﾀ*/
PerDgtPadInfo bug_dgt_data;
#endif /* _BUG */


/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */
static void JudgeGetPerNum(void);           /* ﾍﾟﾘﾌｪﾗﾙ取得数判定             */
static void JudgeOreg(void);                /* OREG判定                      */
static void MoveBdryData(Uint8 *);          /* 境界データ転送処理            */
static void SetPerData(void);               /* ペリフェラルデータ設定        */
static void InitIntBackPer(void);           /* ペリフェラルイントバック初期化*/
static Uint32 GoIntBack(void);              /* イントバックコマンド発行      */
static void AnyInitPerData(void);   /* 複数ペリフェラルデータ初期化(PUSH処理)*/
static void InitPerData(void);      /* ペリフェラルデータ初期化(PUSH処理も)  */
static void InitBdryData(void);             /* 境界ワーク初期化              */
static void SetPerId(PerId);                /* ペリフェラルID設定            */

/******************************************************************************
 *
 * NAME:    PER_Init()          - イントバック初期化
 *
 ******************************************************************************
*/

Uint32 PER_Init(PerKind kind, PerNum num, PerId id, PerSize size,
              Uint32 *work, Uint8 v_blank)
{
    /** BEGIN ****************************************************************/
    /* SMPC初期化 ********************************************************/
    PER_PokeByte(REG_DDRA,0);
    PER_PokeByte(REG_DDRB,0);
    PER_PokeByte(REG_EXLE,0);
    PER_PokeByte(REG_IOSEL,0);
    /* 割り込み初期化 ****************************************************/
    INT_SetScuFunc(INT_SCU_SYS, PER_IntFunc);/* SMPC割り込み処理を登録   */
    INT_ChgMsk(INT_MSK_SYS, INT_MSK_NULL);  /* ｼｽﾃﾑﾏﾈｰｼﾞﾒﾝﾄ割り込みENABLE*/

    intback_kind = kind;                        /* ｲﾝﾄﾊﾞｯｸ種別の登録         */
    switch(intback_kind){                       /* イントバック種別の判定    */
        case PER_KD_SYS:
            intback_ireg[0] = IREG0_SYS;
            intback_ireg[1] = IREG1_PEN_NRET;
            per_set_sys_flg = OFF;              /* ｼｽﾃﾑﾃﾞｰﾀ取得ﾌﾗｸﾞ クリア   */
            return GoIntBack();                 /* イントバックコマンド発行  */
        case PER_KD_PER:
            intback_ireg[0] = IREG0_NSYS;
            intback_ireg[1] = IREG1_PEN_RET | IREG1_OPE_ON;
            break;
        case PER_KD_PERTIM:
            intback_ireg[0] = IREG0_SYS;
            intback_ireg[1] = IREG1_PEN_RET | IREG1_OPE_ON;
            set_time_flg = OFF;                 /* 時刻ﾃﾞｰﾀ取得ﾌﾗｸﾞ クリア   */
            break;
    }
    /* 登録 ******************************************************************/
    intback_size = size;                        /* 必要ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀｻｲｽﾞの登録 */
    intback_num = num;                          /* 必要ペリフェラル数の登録  */
    intback_v_blank = v_blank;                  /* V-BLANKスキップ数の登録   */
    intback_id = id;                            /* ペリフェラルIDの登録      */
    intback_work = (void *)work;                /* ワーク領域の登録          */
    v_blank_cnt = 0;                            /* V-BLANKカウント数のクリア */
    /* イントバックパラメータ設定 ********************************************/
    if(intback_size <= 15){
        intback_ireg[1] |= IREG1_P1MD_15 | IREG1_P2MD_15;
    }else{
        intback_ireg[1] |= IREG1_P1MD_255 | IREG1_P2MD_255;
    }
    /* 時刻データ取得、設定先頭アドレス設定 **********************************/
    set_time_adr = time_data[0];                /* 時刻ﾃﾞｰﾀ取得先頭ｱﾄﾞﾚｽ設定 */
    per_get_time_adr = time_data[1];            /* 時刻ﾃﾞｰﾀ設定先頭ｱﾄﾞﾚｽ設定 */
    /* ペリフェラルデータ取得、設定先頭アドレス設定 **************************/
    get_per_adr = intback_work;             /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ取得先頭ｱﾄﾞﾚｽ設定  */
                                            /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ設定先頭ｱﾄﾞﾚｽ設定  */
    switch(intback_id){                         /* ID によって振り分け       */
        case PER_ID_NCON :
            set_per_adr = (Uint8 *)intback_work + intback_num *
                                                  (intback_size + 1);
            bdry_work_adr = (Uint8 *)intback_work + intback_num *
                                                    (intback_size + 1) * 2;
            break;
        case PER_ID_DGT :
            set_per_adr = PER_DGT(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_DGT(intback_work) + intback_num * 2);
            break;
        case PER_ID_ANL :
            set_per_adr = PER_ANL(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_ANL(intback_work) + intback_num * 2);
            break;
        case PER_ID_PNT :
            set_per_adr = PER_PNT(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_PNT(intback_work) + intback_num * 2);
            break;
        case PER_ID_KBD :
            set_per_adr = PER_KBD(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_KBD(intback_work) + intback_num * 2);
            break;
        case PER_ID_M3BP:
            set_per_adr = PER_M3BP(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_M3BP(intback_work) + intback_num *2);
            break;
        case PER_ID_M6BP:
            set_per_adr = PER_M6BP(intback_work) + intback_num;
            bdry_work_adr = (Uint8 *)(PER_M6BP(intback_work) + intback_num *2);
            break;
    }
    /* ペリフェラルデータ,IDの初期化(取得・設定) *****************************/
    get_per_cnt = 0;
    AnyInitPerData();                           /* 複数ペリフェラル初期化    */

    change_work = set_per_adr;
    set_per_adr = get_per_adr;
    get_per_adr = change_work;

    get_per_cnt = 0;
    AnyInitPerData();                           /* 複数ペリフェラル初期化    */
    /*************************************************************************/
    InitIntBackPer();                           /* ﾍﾟﾘﾌｪﾗﾙｲﾝﾄﾊﾞｯｸ初期化      */
    return GoIntBack();                         /* イントバックコマンド発行  */
}

/******************************************************************************
 *
 * NAME:    PER_GetPer              -   ペリフェラルデータ取得
 *
 * CAVEATS:
 *      ●タイムアウト有無の確認方法
 *          "per_time_out_flg"が"1"以上の場合タイムアウトが起きている。このカ
 *          ウントは指定V-BALNK IN時にタイムアウトが起きている場合にUPする。又
 *          タイムアウトが起きていない場合は、0クリアする。
 *
 ******************************************************************************
*/

Uint32 PER_GetPer(PerGetPer **output_dt)
{
    /** BEGIN ****************************************************************/
    /* イントバック種別判断 **************************************************/
    if((intback_kind != PER_KD_PER) && (intback_kind != PER_KD_PERTIM)){
        *output_dt = NULL;
        return PER_INT_ERR;
    }
#ifndef _PER_HOT_ENA
    /* ホットリセット処理 ****************************************************/
    if((*PER_REG_SR & B_SR_RESB) == B_SR_RESB){ /* ﾎｯﾄﾘｾｯﾄがONか？           */
        hot_res_cnt ++;                         /* ﾎｯﾄﾘｾｯﾄｶｳﾝﾀ インクリメント*/
        if(hot_res_cnt >= HOT_RES_MAX){         /* MAX以上になったか？       */
            per_hot_res = PER_HOT_RES_ON;       /* ホットリセット状態をON    */
            hot_res_cnt --;
        }
    }else{
        per_hot_res = PER_HOT_RES_OFF;          /* ホットリセット状態をOFF   */
        hot_res_cnt = 0;                        /* ﾎｯﾄﾘｾｯﾄｶｳﾝﾀをクリア       */
    }
#endif  /* _PER_HOT_ENA */
    /* V-BLANKスキップ数判断 *************************************************/
    if(v_blank_cnt < intback_v_blank){
        v_blank_cnt++;                          /* V-BLANKカウント++         */
        return PER_INT_OK;                      /* 正常終了                  */
    }
    v_blank_cnt = 0;                            /* V-BLANKカウント クリア    */
    /* タイムアウト処理 ******************************************************/
    if(get_per_cnt < intback_num){              /* タイムアウトか？          */
        per_time_out_flg ++;                    /* タイムアウトフラグアップ  */
        if(per_time_out_flg >= TIME_OUT_MAX){   /* MAX以上になったか？       */
            AnyInitPerData();                   /* 複数ペリフェラル初期化    */
            per_time_out_flg --;
        }
    }else{
        per_time_out_flg = 0;                   /* タイムアウトフラグクリア  */
    }
#ifdef _BUG
    /* SMPC初期化 ********************************************************/
    if(bug_dgt_con[0] != 0xff){
        PER_PokeByte(REG_IOSEL,1);
        /* プロトコル実行 */
        PER_GoProt(PER_CON_1P);
        PER_GetDgtPadInfo(PER_CON_1P, &bug_dgt_data);
        InitBdryData();
        get_per_cnt = bug_dgt_con[0];
        get_per_data_adr = bdry_work_adr;
        bdry_size = BDRY_WORK;
        *(bdry_work_adr + 0) = (Uint8)(bug_dgt_data.on >> 8);
        *(bdry_work_adr + 1) = (Uint8)(bug_dgt_data.on & 0xff);
        SetPerData();
        /* データ設定 */
        PER_PokeByte(REG_DDRA,0);
        PER_PokeByte(REG_EXLE,0);
        PER_PokeByte(REG_IOSEL,0);
    }
    if(bug_dgt_con[1] != 0xff){
        PER_PokeByte(REG_IOSEL,2);
        /* プロトコル実行 */
        PER_GoProt(PER_CON_2P);
        PER_GetDgtPadInfo(PER_CON_2P, &bug_dgt_data);
        InitBdryData();
        get_per_cnt = bug_dgt_con[1];
        get_per_data_adr = bdry_work_adr;
        bdry_size = BDRY_WORK;
        *(bdry_work_adr + 0) = (Uint8)(bug_dgt_data.on >> 8);
        *(bdry_work_adr + 1) = (Uint8)(bug_dgt_data.on & 0xff);
        SetPerData();
        /* データ設定 */
        PER_PokeByte(REG_DDRB,0);
        PER_PokeByte(REG_EXLE,0);
        PER_PokeByte(REG_IOSEL,0);
    }
#endif /* _BUG */


    /* 時刻データ取得、設定先頭アドレスのトグル ******************************/
    change_work = set_time_adr;
    set_time_adr = per_get_time_adr;
    per_get_time_adr = change_work;
    /* ペリフェラルデータ取得、設定先頭アドレスのトグル **********************/
    change_work = set_per_adr;
    set_per_adr = get_per_adr;
    get_per_adr = change_work;
    /* ペリフェラルデータ取得アドレス設定 ************************************/
    *output_dt = (PerGetPer *)get_per_adr;

    InitIntBackPer();                           /* ﾍﾟﾘﾌｪﾗﾙｲﾝﾄﾊﾞｯｸ初期化      */
    /* イントバックコマンド発行 **********************************************/
    GoIntBack();
}

/******************************************************************************
 *
 * NAME:    PER_IntFunc             -   SMPC割り込み処理
 *
 ******************************************************************************
*/

void PER_IntFunc(void)
{
    Uint8 *adr_max;                             /* ｱﾄﾞﾚｽMAX                  */
    Uint32 i;                                   /* ｶｳﾝﾀﾜｰｸ                   */
    Uint32 intr_work;                           /* 割り込み記憶領域          */

    /* 他の割り込み禁止 ******************************************************/
	intr_work = get_imask();
	set_imask(15);

    switch(intback_kind){
    /* システムデータ設定処理 ************************************************/
        case PER_KD_SYS:
        PER_GS_CC(&per_get_sys_data) = ARY_REG_OREG(8);
        PER_GS_AC(&per_get_sys_data) = ARY_REG_OREG(9);
        PER_GS_SS(&per_get_sys_data) = ((Uint16)ARY_REG_OREG(10) << 8) |
                                   ((Uint16)ARY_REG_OREG(11) << 0);
        PER_GS_SM(&per_get_sys_data) = ((Uint32)ARY_REG_OREG(12) << 24) |
                                   ((Uint32)ARY_REG_OREG(13) << 16) |
                                   ((Uint32)ARY_REG_OREG(14) <<  8) |
                                   ((Uint32)ARY_REG_OREG(15) <<  0);
        PER_GS_SMPC_STAT(&per_get_sys_data) = ARY_REG_OREG(0);
        per_set_sys_flg = ON;
        /* 他の割り込み回復 **************************************************/
        set_imask(intr_work);                   /* 元に戻す                  */
        return;                                 /* 正常終了                  */
    /* 時刻設定処理 **********************************************************/
        case PER_KD_PERTIM:
        if(set_time_flg == OFF){
            for(i = 0; i < 7; i++){
                *(set_time_adr + i) = ARY_REG_OREG(7 - i);
            }
        set_time_flg = ON;
        end_flg = END_CONT;
        break;
        }
        /* ペリフェラルデータ設定処理*****************************************/
        case PER_KD_PER:
        /* 初期処理 **********************************************************/
        end_flg = END_END;
        get_oreg_adr = PER_REG_OREG;
        /* OREGデータ加工処理 ************************************************/
        while(body_conect_cnt < BODY_CONECT_MAX){
            if(remain_conect_cnt <= 0){
                remain_conect_cnt = (*get_oreg_adr) & PS_MSK_CON;
                                                    /* ﾏﾙﾁﾀｯﾌﾟｺﾈｸﾀ数取得     */
#ifdef _BUG
                if((remain_conect_cnt == 1) & (*(get_oreg_adr + 2) == 02)){
                    bug_dgt_con[body_conect_cnt] = get_per_cnt;
                }
#endif /* _BUG */
                if(remain_conect_cnt == PS_CON_NON){/* 未接続orSMPCUNKNOWか？*/
                    if(((*get_oreg_adr) & PS_MSK_ID) == PS_ID_DIRNON){
                                                    /* 未接続か？            */
                        SetPerId(PER_ID_NCON);      /* ペリフェラルID設定    */
                        InitPerData();              /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ初期化     */
                    }else{                          /* SMPC UNKOWN           */
                        SetPerId(PER_ID_UNKOWN);    /* ペリフェラルID設定    */
                    }

                    get_per_cnt ++;                 /* 取得数ｶｳﾝﾀ ｲﾝｸﾘﾒﾝﾄ    */
                    JudgeGetPerNum();               /* ペリフェラル取得数判定*/
                    if(end_flg == END_BREAK) break;
                    JudgeOreg();                   /* OREG判定               */
                    if(end_flg == END_CONT) break;
                }else{
                    get_per_id_flg = ON;
                }
                get_oreg_adr += 2;                  /* OREG対象ｱﾄﾞﾚｽ ｲﾝｸﾘﾒﾝﾄ */
            }
            while(remain_conect_cnt > 0){       /* 残りﾍﾟﾘﾌｪﾗﾙがなくなるまで */
                /* ペリフェラルID取得処理 ************************************/
                if(get_per_id_flg == ON){
                    get_per_id_flg = OFF;
                    get_mus_id = PER_ID_NCON;
                    if(((*get_oreg_adr) & PI_MSK_ID) == PI_ID_NON_UNKNOWN){
                                                /* 未接続 or UNKNOWN         */
                        if(((*get_oreg_adr) & PI_MSK_CON) == PI_CON_NON_PER){
                            SetPerId(PER_ID_NCON);
                        }else{
                            SetPerId(PER_ID_UNKOWN);
                        }
                        InitPerData();          /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ初期化         */
                        get_per_cnt ++;         /* ﾍﾟﾘﾌｪﾗﾙ取得数ｶｳﾝﾀ ｲﾝｸﾘﾒﾝﾄ */
                        JudgeGetPerNum();    /* ペリフェラル取得数判定    */
                        if(end_flg == END_BREAK) break;
                        get_per_id_flg = ON;
                        remain_conect_cnt --;   /* 残ｺﾈｸﾀ数ｶｳﾝﾀ ｲﾝｸﾘﾒﾝﾄ      */
                    }else{                      /* 未接続 and UNKNOWNでない  */

                        if(((*get_oreg_adr) & PI_MSK_ID) == PI_ID_M5){
                            if(*get_oreg_adr == ID_MMUS){
                                SetPerId(PER_ID_PNT);
                                get_mus_id = PER_ID_PNT;
                            }else{
                                SetPerId(*get_oreg_adr);
                            }
                        }else{
                            SetPerId((*get_oreg_adr) & PI_MSK_ID);
                            get_mus_id = (*get_oreg_adr) & PI_MSK_ID;
                        }

                        if(((*get_oreg_adr) & PI_MSK_CON) == PI_CON_EXP){
                            get_exp_per_size_flg = ON;
                        }else{
                            get_per_size = (*get_oreg_adr) & PI_MSK_CON;
                        }
                        /* mouse special (start)*/
                        if(get_mus_id == PER_ID_PNT){
                            backup_size = intback_size;
                            if(intback_size < 5){
                                intback_size = 3;
                            }else{
                                intback_size -= 2;
                            }
                        }
                        /* mouse special (end) */
                        set_bdr_flg = ON;       /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ境界合わせﾌﾗｸﾞ */
                    }
                    get_oreg_adr += 2;
                    JudgeOreg();               /* OREG判定                  */
                    if(end_flg == END_CONT) break;
                }
                /* 拡張ペリフェラルデータサイズ取得処理 **********************/
                if(get_exp_per_size_flg == ON){
                    get_exp_per_size_flg = OFF;
                    get_per_size = *get_oreg_adr;
                    get_oreg_adr += 2;
                    JudgeOreg();               /* OREG判定                  */
                    if(end_flg == END_CONT) break;
                }
                /* ペリフェラルデータ境界合わせ処理 **************************/
                if(set_bdr_flg == ON){
                    set_bdr_flg = OFF;
                    if(get_per_size >= intback_size){   /* 取得ﾃﾞｰﾀｻｲｽﾞが大か*/
                        if((get_oreg_adr + get_per_size * 2) >
                           REG_OREG_MAX){       /* 境界をまたぐか？          */
                            skip_per_data_flg = ON;
                            if((get_oreg_adr + intback_size * 2) >
                                REG_OREG_MAX){  /* 境界をまたぐか？(intback) */
                                get_per_data_flg = ON;
                                get_per_skip_size = get_per_size - intback_size;
                                get_per_size = intback_size - (REG_OREG_MAX -
                                                get_oreg_adr) / 2;
                                adr_max = REG_OREG_MAX;
                                MoveBdryData(adr_max);
                                end_flg = END_CONT;
                                break;
                            }else{
                                set_per_data_flg = ON;
                                get_per_skip_size = get_per_size -
                                    (REG_OREG_MAX - get_oreg_adr) / 2;
                                adr_max = get_oreg_adr + (intback_size * 2);
                                MoveBdryData(adr_max);
                                get_oreg_adr = REG_OREG_MAX;
                            }
                        }else{
                            if(get_mus_id == PER_ID_PNT){
                                adr_max = get_oreg_adr + (intback_size * 2);
                                MoveBdryData(adr_max);  /* 境界データ転送処理*/
                                get_per_size = 0;       /* 残りデータサイズ=0*/
                            }else{
                                bdry_size = BDRY_OREG;
                                get_per_data_adr = get_oreg_adr;
                                get_oreg_adr += get_per_size * 2;
                            }
                            set_per_data_flg = ON;      /* 設定ON            */
                        }
                    }else{                      /* 取得ﾃﾞｰﾀｻｲｽﾞが小か？      */
                        if((get_oreg_adr + get_per_size * 2) >
                           REG_OREG_MAX){       /* 境界をまたぐか？          */
                            get_per_data_flg = ON;  /* 取得ON                */
                            get_per_size -= (REG_OREG_MAX - get_oreg_adr) / 2;
                                                /* 残りデータサイズ          */
                            adr_max = REG_OREG_MAX;
                            MoveBdryData(adr_max);   /* 境界データ転送処理   */
                            end_flg = END_CONT;
                            break;
                        }else{
                            set_per_data_flg = ON;      /* 設定ON            */
                            adr_max = get_oreg_adr + (get_per_size * 2);
                            MoveBdryData(adr_max);   /* 境界データ転送処理   */
                            get_per_size = 0;       /* 残りデータサイズ = 0  */
                        }
                    }
                }
                /* ペリフェラルデータ取得処理 ********************************/
                if(get_per_data_flg == ON){
                    get_per_data_flg = OFF;
                    if(get_per_size >= OREG_MAX){
                        get_per_data_flg = ON;
                        adr_max = REG_OREG_MAX;
                        get_per_size -= OREG_MAX;
                    }else{
                        set_per_data_flg = ON;
                        adr_max = PER_REG_OREG + (get_per_size * 2);
                        get_per_size = 0;
                    }
                    while(get_oreg_adr < adr_max){
                        *set_per_data_adr = *get_oreg_adr;
                        set_per_data_adr ++;
                        get_oreg_adr += 2;
                    }
                    JudgeOreg();
                    if(end_flg == END_CONT) break;
                }
                /* ペリフェラルデータ設定処理 ********************************/
                if(set_per_data_flg == ON){
                    set_per_data_flg = OFF;
                    SetPerData();             /* ペリフェラルデータ設定    */
                    get_per_cnt ++;             /* ﾍﾟﾘﾌｪﾗﾙ取得数ｶｳﾝﾀ ｲﾝｸﾘﾒﾝﾄ */
                    JudgeGetPerNum();        /* ペリフェラル取得数判定    */
                    if(end_flg == END_BREAK) break;
                    if(skip_per_data_flg == OFF){
                        remain_conect_cnt --;
                        get_per_id_flg = ON;
                    }
                    JudgeOreg();               /* OREG判定                  */
                    if(end_flg == END_CONT) break;
                }
                /* ペリフェラルデータスキップ処理 ****************************/
                if(skip_per_data_flg == ON){
                    skip_per_data_flg = OFF;
                    if(get_per_skip_size > OREG_MAX){
                        get_per_data_flg = ON;
                        get_oreg_adr = REG_OREG_MAX;
                        get_per_skip_size -= OREG_MAX;
                    }else{
                        get_oreg_adr += get_per_skip_size * 2;
                        get_per_id_flg = ON;
                        remain_conect_cnt --;
                        get_per_skip_size = 0;
                    }
                    JudgeOreg();
                    if(end_flg == END_CONT) break;
                }
            }
            if(end_flg != END_END){
                break;
            }else{
                body_conect_cnt ++;
            }
        }
        break;
    }
    
    switch(end_flg){
        case END_END:                           /* データ取得終了処理        */
            AnyInitPerData();                   /* 複数ペリフェラル初期化    */
        case END_BREAK:                         /* ブレイク処理              */
            *PER_REG_IREG = (now_cont & IREG0_CONT) | IREG0_BR;
            break;
        case END_CONT:                          /* コンティニュ処理          */
            now_cont = ~now_cont;
            *PER_REG_IREG = (now_cont & IREG0_CONT);
            break;
    }
    /* 他の割り込み回復 ******************************************************/
    set_imask(intr_work);                       /* 元に戻す                  */
}
/******************************************************************************
 *
 * NAME:    JudgeGetPerNum          -   ペリフェラル取得数判定
 *
 ******************************************************************************
*/
static void JudgeGetPerNum(void)            /* ﾍﾟﾘﾌｪﾗﾙ取得数判定             */
{
    if(get_per_cnt >= intback_num){
        end_flg = END_BREAK;
    }
}

/******************************************************************************
 *
 * NAME:    JudgeOreg               -   OREG判定
 *
 ******************************************************************************
*/
static void JudgeOreg(void)                 /* OREG判定                      */
{
    if(get_oreg_adr >= REG_OREG_MAX){ /* 全OREG加工終了か？    */
        if((*PER_REG_SR & B_SR_PDE) == SR_PDE_ON){
                                           /* 残りペリフェラルがあるか？ */
            end_flg = END_CONT;
        }
    }
}

/******************************************************************************
 *
 * NAME:    MoveBdryData            -   境界データ転送処理
 *
 ******************************************************************************
*/
static void MoveBdryData(Uint8 *adr_max)    /* 境界データ転送処理            */
{
    InitBdryData();                             /* 境界ワーク初期化          */
    get_per_data_adr = bdry_work_adr;
    set_per_data_adr = bdry_work_adr;
    bdry_size = BDRY_WORK;
    if(get_mus_id == PER_ID_PNT){               /* マウス特別処理            */
        if(backup_size < 5){
            get_per_data_adr = &get_mus_data[0];
            set_per_data_adr = &get_mus_data[2];
        }else{
            set_per_data_adr += 2;
        }
    }
    while(get_oreg_adr < adr_max){
        *set_per_data_adr = *get_oreg_adr;
        set_per_data_adr ++;
        get_oreg_adr += 2;
    }
}

/******************************************************************************
 *
 * NAME:    SetPerData              -   ペリフェラルデータ設定
 *
 ******************************************************************************
*/
static void SetPerData(void)                /* ペリフェラルデータ設定        */
{                                           /* (PUSHも)                      */
    Uint32 i;

    /* ペリフェラルID = マウス 処理 ******************************************/
    if(get_mus_id == PER_ID_PNT){
            GET_PER_DATA(0) = 0xff;
            GET_PER_DATA(1) = 0xff;
            if((GET_PER_DATA(2) & PNT_YO) == PNT_YO){   /* オーバフローか？  */
                if((GET_PER_DATA(2) & PNT_YS) == PNT_YS){
                    GET_PER_DATA(0) &= ~DGT_0_D;
                }else{
                    GET_PER_DATA(0) &= ~DGT_0_U;
                }
            }else{
                if(GET_PER_DATA(4) > MV_PNT_UD){
                    if((GET_PER_DATA(2) & PNT_YS) == PNT_YS){
                        GET_PER_DATA(0) &= ~DGT_0_D;
                    }else{
                        GET_PER_DATA(0) &= ~DGT_0_U;
                    }
                }
            }
            if((GET_PER_DATA(2) & PNT_XO) == PNT_XO){   /* オーバフローか？  */
                if((GET_PER_DATA(2) & PNT_XS) == PNT_XS){
                    GET_PER_DATA(0) &= ~DGT_0_L;
                }else{
                    GET_PER_DATA(0) &= ~DGT_0_R;
                }
            }else{
                if(GET_PER_DATA(3) > MV_PNT_RL){
                    if((GET_PER_DATA(2) & PNT_XS) == PNT_XS){
                        GET_PER_DATA(0) &= ~DGT_0_L;
                    }else{
                        GET_PER_DATA(0) &= ~DGT_0_R;
                    }
                }
            }

            if((GET_PER_DATA(2) & PNT_L) == PNT_L){      /* LEFT             */
                GET_PER_DATA(0) &= ~DGT_0_A;
            }
            if((GET_PER_DATA(2) & PNT_MID) == PNT_MID){  /* MIDDLE           */
                GET_PER_DATA(0) &= ~DGT_0_B;
            }
            if((GET_PER_DATA(2) & PNT_R) == PNT_R){      /* RIGHT            */
                GET_PER_DATA(0) &= ~DGT_0_C;
            }
            if((GET_PER_DATA(2) & PNT_S) == PNT_S){      /* START            */
                GET_PER_DATA(0) &= ~DGT_0_S;
            }
        intback_size = backup_size;
    }
    /* 取得ペリフェラルID別処理 **********************************************/
    switch(intback_id){
        case PER_ID_NCON :
            for(i = 0; i < intback_size; i++){
                *((Uint8 *)set_per_adr + get_per_cnt * (intback_size + 1)
                   + 1 + i) = GET_PER_DATA(i);
            }
            break;
        case PER_ID_DGT :
            PER_DGT(set_per_adr)[get_per_cnt].data =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            /* PUSH ******************************************************/
            PER_DGT(set_per_adr)[get_per_cnt].push =
                SET_PUSH(PER_DGT(get_per_adr)[get_per_cnt].data,
                PER_DGT(set_per_adr)[get_per_cnt].data);
            break;

        case PER_ID_ANL :
            PER_ANL(set_per_adr)[get_per_cnt].data.dgt =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            PER_ANL(set_per_adr)[get_per_cnt].data.x =
                (Sint16)GET_PER_DATA(2);
            PER_ANL(set_per_adr)[get_per_cnt].data.y =
                (Sint16)GET_PER_DATA(3);
            PER_ANL(set_per_adr)[get_per_cnt].data.z =
                (Sint16)GET_PER_DATA(4);
            /* PUSH ******************************************************/
            PER_ANL(set_per_adr)[get_per_cnt].push.dgt =
                SET_PUSH(PER_ANL(get_per_adr)[get_per_cnt].data.dgt,
                PER_ANL(set_per_adr)[get_per_cnt].data.dgt);
            PER_ANL(set_per_adr)[get_per_cnt].push.x =
                PER_ANL(get_per_adr)[get_per_cnt].data.x -
                PER_ANL(set_per_adr)[get_per_cnt].data.x;
            PER_ANL(set_per_adr)[get_per_cnt].push.y =
                PER_ANL(get_per_adr)[get_per_cnt].data.y -
                PER_ANL(set_per_adr)[get_per_cnt].data.y;
            PER_ANL(set_per_adr)[get_per_cnt].push.z =
                PER_ANL(get_per_adr)[get_per_cnt].data.z -
                PER_ANL(set_per_adr)[get_per_cnt].data.z;
            break;

        case PER_ID_PNT :
            PER_PNT(set_per_adr)[get_per_cnt].data.dgt =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            PER_PNT(set_per_adr)[get_per_cnt].data.data =
                ~((Uint16)GET_PER_DATA(2));

            PER_PNT(set_per_adr)[get_per_cnt].data.x =
                (Sint16)GET_PER_DATA(3) *
                (((PER_PNT(set_per_adr)[get_per_cnt].data.data &
                  PNT_XS) == PNT_XS) ? 1 : -1);
            PER_PNT(set_per_adr)[get_per_cnt].data.y =
                (Sint16)GET_PER_DATA(4) *
                (((PER_PNT(set_per_adr)[get_per_cnt].data.data &
                  PNT_YS) == PNT_YS) ? 1 : -1);
            /* PUSH ******************************************************/
            PER_PNT(set_per_adr)[get_per_cnt].push.dgt =
                SET_PUSH(PER_PNT(get_per_adr)[get_per_cnt].data.dgt,
                PER_PNT(set_per_adr)[get_per_cnt].data.dgt);
            break;
        case PER_ID_KBD :
            PER_KBD(set_per_adr)[get_per_cnt].data.dgt =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            PER_KBD(set_per_adr)[get_per_cnt].data.skey =
                ~((Uint8)GET_PER_DATA(2));
            PER_KBD(set_per_adr)[get_per_cnt].data.key =
                (Uint8)GET_PER_DATA(3);
            /* PUSH ******************************************************/
            PER_KBD(set_per_adr)[get_per_cnt].push.dgt =
                SET_PUSH(PER_KBD(get_per_adr)[get_per_cnt].data.dgt,
                PER_KBD(set_per_adr)[get_per_cnt].data.dgt);
            break;
        case PER_ID_M3BP:
            PER_M3BP(set_per_adr)[get_per_cnt].data =
                (PerDgtData)GET_PER_DATA(0);
            /* PUSH ******************************************************/
            PER_M3BP(set_per_adr)[get_per_cnt].push =
                SET_PUSH(PER_M3BP(get_per_adr)[get_per_cnt].data,
                PER_M3BP(set_per_adr)[get_per_cnt].data);
            break;
        case PER_ID_M6BP:
            PER_M6BP(set_per_adr)[get_per_cnt].data =
                ((PerDgtData)GET_PER_DATA(0) << 8) |
                (PerDgtData)GET_PER_DATA(1);
            /* PUSH ******************************************************/
            PER_M6BP(set_per_adr)[get_per_cnt].push =
                SET_PUSH(PER_M6BP(get_per_adr)[get_per_cnt].data,
                PER_M6BP(set_per_adr)[get_per_cnt].data);
            break;
    }
}
/******************************************************************************
 *
 * NAME:    InitIntBackPer          -   ペリフェラルイントバック初期化
 *
 ******************************************************************************
*/
static void InitIntBackPer(void)            /* ペリフェラルイントバック初期化*/
{                                           /*********************************/
    /* 初期化 ****************************************************************/
    get_per_id_flg = ON;       
    get_exp_per_size_flg = OFF;
    set_bdr_flg = OFF;         
    get_per_data_flg = OFF;    
    set_per_data_flg = OFF;    
    skip_per_data_flg = OFF;   
    set_time_flg = OFF;                         /* 時刻取得フラグクリア      */
    get_per_cnt = 0;                            /* ペリフェラル取得数クリア  */
    body_conect_cnt = 0;                        /* 本体コネクタカウンタ      */
    remain_conect_cnt = 0;                      /* 残コネクタカウンタ        */
    now_cont = 0;                               /* コンティニュ記憶領域初期化*/
#ifdef _BUG
    bug_dgt_con[0] = 0xff;
    bug_dgt_con[1] = 0xff;
#endif /* _BUG */
}

/******************************************************************************
 *
 * NAME:    GoIntBack               -   イントバックコマンド発行
 *
 ******************************************************************************
*/
static Uint32 GoIntBack(void)               /* イントバックコマンド発行      */
{                                           /*********************************/
    if((PER_PeekByte(PER_REG_SF) & PER_B_SF) == PER_B_SF){
                                            /* SMPCコマンド実行中か？        */
        return PER_INT_ERR;                 /* エラーリターン                */
    }
    PER_PokeByte(PER_REG_SF, PER_B_SF);     /* SF<-"1"                  */\

    ARY_REG_IREG(0) = intback_ireg[0];
    ARY_REG_IREG(1) = intback_ireg[1];
    ARY_REG_IREG(2) = intback_ireg[2];
    
    PER_PokeByte(PER_REG_COMREG,SM_INTBACK);/* SMPCコマンド実行              */
    return PER_INT_OK;                      /* 正常リターン                  */
}

/******************************************************************************
 *
 * NAME:    AnyInitPerData          -   複数ペリフェラルデータ初期化(PUSH処理)
 *
 ******************************************************************************
*/

static void AnyInitPerData(void)    /* 複数ペリフェラルデータ初期化(PUSH処理)*/
{
    while(get_per_cnt < intback_num){
        SetPerId(PER_ID_NCON);                  /* ﾍﾟﾘﾌｪﾗﾙID設定             */
        InitPerData();                          /* ﾍﾟﾘﾌｪﾗﾙﾃﾞｰﾀ初期化         */
        get_per_cnt ++;
    }
}

/******************************************************************************
 *
 * NAME:    InitPerData             -   ペリフェラルデータ初期化(PUSH処理も)
 *
 ******************************************************************************
*/

static void InitPerData(void)       /* ペリフェラルデータ初期化(PUSH処理も)  */
{
    Uint32 i;
    switch(intback_id){
        case PER_ID_NCON :
            for(i = 0; i < intback_size; i++){
                *((Uint8 *)set_per_adr + get_per_cnt * (intback_size + 1)
                   + 1 + i) = INIT_NCON;
            }
            break;
        case PER_ID_DGT :
            PER_DGT(set_per_adr)[get_per_cnt].data = INIT_DGT;
            /* PUSH ******************************************************/
            PER_DGT(set_per_adr)[get_per_cnt].push = INIT_DGT;
            break;

        case PER_ID_ANL :
            PER_ANL(set_per_adr)[get_per_cnt].data.dgt = INIT_DGT;
            PER_ANL(set_per_adr)[get_per_cnt].data.x = INIT_ANL_X;
            PER_ANL(set_per_adr)[get_per_cnt].data.y = INIT_ANL_Y;
            PER_ANL(set_per_adr)[get_per_cnt].data.z = INIT_ANL_Z;
            /* PUSH ******************************************************/
            PER_ANL(set_per_adr)[get_per_cnt].push.dgt = INIT_DGT;
            PER_ANL(set_per_adr)[get_per_cnt].push.x = INIT_ANL_X;
            PER_ANL(set_per_adr)[get_per_cnt].push.y = INIT_ANL_Y;
            PER_ANL(set_per_adr)[get_per_cnt].push.z = INIT_ANL_Z;
            break;

        case PER_ID_PNT :
            PER_PNT(set_per_adr)[get_per_cnt].data.data = INIT_PNT_DATA;
            PER_PNT(set_per_adr)[get_per_cnt].data.x = INIT_PNT_X;
            PER_PNT(set_per_adr)[get_per_cnt].data.y = INIT_PNT_Y;
            PER_PNT(set_per_adr)[get_per_cnt].data.dgt = INIT_DGT;
            /* PUSH ******************************************************/
            PER_PNT(set_per_adr)[get_per_cnt].push.data = INIT_PNT_DATA;
            PER_PNT(set_per_adr)[get_per_cnt].push.x = INIT_PNT_X;
            PER_PNT(set_per_adr)[get_per_cnt].push.y = INIT_PNT_Y;
            PER_PNT(set_per_adr)[get_per_cnt].push.dgt = INIT_DGT;
            break;
        case PER_ID_KBD :
            PER_KBD(set_per_adr)[get_per_cnt].data.dgt = INIT_DGT;
            PER_KBD(set_per_adr)[get_per_cnt].data.skey = INIT_KBD_SKEY;
            PER_KBD(set_per_adr)[get_per_cnt].data.key = INIT_KBD_KEY;
            /* PUSH ******************************************************/
            PER_KBD(set_per_adr)[get_per_cnt].push.dgt = INIT_DGT;
            PER_KBD(set_per_adr)[get_per_cnt].push.skey = INIT_KBD_SKEY;
            PER_KBD(set_per_adr)[get_per_cnt].push.key = INIT_KBD_KEY;
            break;
        case PER_ID_M3BP:
            PER_M3BP(set_per_adr)[get_per_cnt].data = INIT_DGT;
            /* PUSH ******************************************************/
            PER_M3BP(set_per_adr)[get_per_cnt].push = INIT_DGT;
            break;
        case PER_ID_M6BP:
            PER_M6BP(set_per_adr)[get_per_cnt].data = INIT_DGT;
            /* PUSH ******************************************************/
            PER_M6BP(set_per_adr)[get_per_cnt].push = INIT_DGT;
            break;
    }
}

/******************************************************************************
 *
 * NAME:    InitBdryData            -   境界ワーク初期化
 *
 ******************************************************************************
*/

static void InitBdryData(void)              /* 境界ワーク初期化              */
{
    Uint32 i;

    switch(intback_id){             /* ID によって振り分け       */
        case PER_ID_NCON :
            for(i = 0; i < intback_size; i++){
                *(bdry_work_adr + i) = ~0x0;
            }
            break;
        case PER_ID_DGT :
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            break;
        case PER_ID_ANL :
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            *(bdry_work_adr + 2) = 0x0;
            *(bdry_work_adr + 3) = 0x0;
            *(bdry_work_adr + 4) = 0x0;
            break;
        case PER_ID_PNT :
            *(bdry_work_adr + 0) = 0x0;
            *(bdry_work_adr + 1) = 0x0;
            *(bdry_work_adr + 2) = 0x0;
            break;
        case PER_ID_KBD :
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            *(bdry_work_adr + 2) = 0x0;
            *(bdry_work_adr + 3) = 0x0;
            break;
        case PER_ID_M3BP:
            *(bdry_work_adr + 0) = ~0x0;
            break;
        case PER_ID_M6BP:
            *(bdry_work_adr + 0) = ~0x0;
            *(bdry_work_adr + 1) = ~0x0;
            break;
    }
}

/******************************************************************************
 *
 * NAME:    SetPerId                -   ペリフェラルID設定
 *
 ******************************************************************************
*/

static void SetPerId(PerId id)              /* ペリフェラルID設定            */
{
    switch(intback_id){             /* ID によって振り分け       */
        case PER_ID_NCON :
            *((Uint8 *)set_per_adr + get_per_cnt * (intback_size + 1)) = id;
            break;
        case PER_ID_DGT :
            PER_DGT(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_ANL :
            PER_ANL(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_PNT :
            PER_PNT(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_KBD :
            PER_KBD(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_M3BP:
            PER_M3BP(set_per_adr)[get_per_cnt].id = id;
            break;
        case PER_ID_M6BP:
            PER_M6BP(set_per_adr)[get_per_cnt].id = id;
            break;
    }
}
