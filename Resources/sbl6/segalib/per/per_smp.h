/*-----------------------------------------------------------------------------
 *  FILE: per_smp.h
 *
 *	    Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      SMPC制御ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      SMPCのコマンド実行のための関数を実行する。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          低水準マクロ
 *              PER_SMPC_WAIT()         -   SMPC待ち時間処理
 *              PER_SMPC_GO_CMD()       -   SMPCコマンド実行
 *              PER_SMPC_STATS_GET()    -   SMPCステータス取得
 *              PER_SMPC_SET_IREG()     -   SMPC IREGセット
 *              PER_SMPC_NO_IREG()      -   IREG設定無しコマンド実行
 *          高水準マクロ
 *              PER_SMPC_MSH_ON()       -   マスタSH ON
 *              PER_SMPC_SSH_ON()       -   スレーブSH ON
 *              PER_SMPC_SSH_OFF()      -   スレーブSH OFF
 *              PER_SMPC_SND_ON()       -   サウンドON
 *              PER_SMPC_SND_OFF()      -   サウンドOFF
 *              PER_SMPC_CD_ON()        -   CD ON
 *              PER_SMPC_CD_OFF()       -   CD OFF
 *              PER_SMPC_SYS_RES()      -   システム全体リセット
 *              PER_SMPC_NMI_REQ()      -   NMIリクエスト
 *              PER_SMPC_RES_ENA()      -   ホットリセットイネーブル
 *              PER_SMPC_RES_DIS()      -   ホットリセットディセーブル
 *              PER_SMPC_SET_SM()       -   SMPCメモリ設定
 *              PER_SMPC_SET_TIM()      -   時刻設定
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1994-07-24  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef PER_SMP_H
#define PER_SMP_H

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
/*smpc conflicts with shtypes.h
#include "sega_xpt.h"
*/

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "per_xpt.h"

/*
 * GLOBAL DEFINESMACROS DEFINES
 */

/**** コマンド ***************************************************************/
#define PER_SM_MSHON    0x00                    /* マスタSH ON               */
#define PER_SM_SSHON    0x02                    /* スレーブSH ON             */
#define PER_SM_SSHOFF   0x03                    /* スレーブSH OFF            */
#define PER_SM_SNDON    0x06                    /* サウンドON                */
#define PER_SM_SNDOFF   0x07                    /* サウンドOFF               */
#define PER_SM_CDON     0x08                    /* CD ON                     */
#define PER_SM_CDOFF    0x09                    /* CD OFF                    */
#define PER_SM_SYSRES   0x0d                    /* システム全体リセット      */
#define PER_SM_NMIREQ   0x18                    /* NMIリクエスト             */
#define PER_SM_RESENA   0x19                    /* ホットリセットイネーブル  */
#define PER_SM_RESDIS   0x1a                    /* ホットリセットディセーブル*/
#define PER_SM_SETSM    0x17                    /* SMPCメモリ設定            */
#define PER_SM_SETTIM   0x16                    /* 時刻設定                  */

/*****************************************************************************/
/*****************************************************************************/
/**** 低水準マクロ ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_WAIT() - SMPC待ち時間処理
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      SMPCを正しく実行するための、待ち時間を取る。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      PER_SMPC_SET_IREG(),PER_SMPCCmdGo()の前に必ず実行すること。
 *
 ******************************************************************************
 */

#define PER_SMPC_WAIT(x)                        /* SMPC待ち時間処理         */\
            do{                                                               \
                while((PER_PeekByte(PER_REG_SF) & PER_B_SF) == PER_B_SF);\
                                                /* SFが"1"の間繰り返し      */\
                PER_PokeByte(PER_REG_SF, PER_B_SF);\
                                                /* SF<-"1"                  */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_GO_CMD() - SMPCコマンド実行
 *
 * PARAMETERS :
 *      (1) Uint8   smpc_cmd    - <i>   SMPCマクロ名
 *
 * DESCRIPTION:
 *      取得したSMPCマクロ名をSMPCのCOMREG（コマンドレジスタ）へセットする。
 *  （セットすることにより、SMPCハードがCOMREGに書かれたコマンドを実行する）
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define PER_SMPC_GO_CMD(smpc_cmd)                 /* SMPCコマンド実行         */\
            do{                                                               \
             PER_PokeByte(PER_REG_COMREG, smpc_cmd);/* COMREGにコマンドWRITE*/\
             while(PER_PeekByte(PER_REG_SF) & PER_B_SF);\
                                                    /* SFが"1"でなくなるまで*/\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_STATS_GET()  - SMPCステータス取得
 *
 * PARAMETERS :
 *      (1) Uint8   stats_reg   - <o>   コマンド実行ステータス
 *
 * DESCRIPTION:
 *      コマンド実行後のステータスを取得する
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define PER_SMPC_STATS_GET(stats_reg)           /* SMPCステータス取得       */\
            do{                                                               \
                stats_reg = PER_PeekByte(PER_SR);                             \
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_IREG()   - SMPC IREGセット
 *
 * PARAMETERS :
 *      (1) Uint32  ireg_no     - <o>   IREG番号
 *      (2) Uint8   ireg_prm    - <i>   IREGセット値
 *
 * DESCRIPTION:
 *      指定されたIREG番号アドレスにIREG値をセットする。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_IREG(ireg_no, ireg_prm)    /* SMPC IREGセット          */\
            do{                                                               \
                PER_PokeByte((PER_REG_IREG + (ireg_no * 2)), ireg_prm);   \
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_NO_IREG()    - IREG設定無しコマンド実行
 *
 ******************************************************************************
 */

#define PER_SMPC_NO_IREG(com)\
            do{                                                               \
                PER_SMPC_WAIT();                /* SMPC待ち時間処理         */\
                PER_SMPC_GO_CMD(com);           /* SMPCコマンド実行         */\
            }while(FALSE)

/*****************************************************************************/
/*****************************************************************************/
/**** 高水準マクロ ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_MSH_ON()     - マスタSH ON
 *
 ******************************************************************************
 */

#define PER_SMPC_MSH_ON(x)\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_MSHON);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_ON()     - スレーブSH ON
 *
 ******************************************************************************
 */

#define PER_SMPC_SSH_ON(x)\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHON);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_OFF()    - スレーブSH OFF
 *
 ******************************************************************************
 */

#define PER_SMPC_SSH_OFF(x)\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHOFF);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_ON()     - サウンドON
 *
 ******************************************************************************
 */

#define PER_SMPC_SND_ON(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDON);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_OFF()    - サウンドOFF
 *
 ******************************************************************************
 */

#define PER_SMPC_SND_OFF(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDOFF);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_ON()      - CD ON
 *
 ******************************************************************************
 */

#define PER_SMPC_CD_ON(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDON);    /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_OFF()     - CD OFF
 *
 ******************************************************************************
 */

#define PER_SMPC_CD_OFF(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDOFF);   /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SYS_RES()    - システム全体リセット
 *
 ******************************************************************************
 */

#define PER_SMPC_SYS_RES(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SYSRES);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_NMI_REQ()    - NMIリクエスト
 *
 ******************************************************************************
 */

#define PER_SMPC_NMI_REQ(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_NMIREQ);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_ENA()    - ホットリセットイネーブル
 *
 ******************************************************************************
 */

#define PER_SMPC_RES_ENA(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESENA);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_DIS()    - ホットリセットディセーブル
 *
 ******************************************************************************
 */

#define PER_SMPC_RES_DIS(x)\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESDIS);  /* IREG設定無しコマンド実行 */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_SM()     - SMPCメモリ設定
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_SM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC待ち時間処理         */\
                PER_SMPC_SET_IREG(0, (ireg) >> 24); /* IREG0セット          */\
                PER_SMPC_SET_IREG(1, (ireg) >> 16); /* IREG0セット          */\
                PER_SMPC_SET_IREG(2, (ireg) >>  8); /* IREG0セット          */\
                PER_SMPC_SET_IREG(3, (ireg) >>  0); /* IREG0セット          */\
                PER_SMPC_GO_CMD(PER_SM_SETSM);    /* SMPCコマンド実行         */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_TIM()    - 時刻設定
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_TIM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC待ち時間処理         */\
                PER_SMPC_SET_IREG(6, *(ireg));    /* IREG6セット              */\
                PER_SMPC_SET_IREG(5, *(ireg + 1));/* IREG5セット              */\
                PER_SMPC_SET_IREG(4, *(ireg + 2));/* IREG4セット              */\
                PER_SMPC_SET_IREG(3, *(ireg + 3));/* IREG3セット              */\
                PER_SMPC_SET_IREG(2, *(ireg + 4));/* IREG2セット              */\
                PER_SMPC_SET_IREG(1, *(ireg + 5));/* IREG1セット              */\
                PER_SMPC_SET_IREG(0, *(ireg + 6));/* IREG0セット              */\
                PER_SMPC_GO_CMD(PER_SM_SETTIM);   /* SMPCコマンド実行         */\
            }while(FALSE)

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */

#endif  /* ifndef PER_SMP_H */
