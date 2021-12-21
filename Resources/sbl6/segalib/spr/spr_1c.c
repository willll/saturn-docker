/*----------------------------------------------------------------------------
 *  spr_1c.c -- SPR ライブラリ 1C モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  このライブラリはスプライト表示基本処理モジュールで、以下のルーチンを含む。
 *
 *  SPR_Initial             -  スプライト表示基本処理ライブラリの初期化処理
 *  SPR_SetTvMode           -  ＴＶモードのセット
 *  SPR_GetStatus           -  現スプライト制御情報の取得
 *  SPR_SetEraseData        -  フレームバッファのイレーズデータセット
 *  SPR_WaitDrawEnd         -  フレームバッファ描画終了チェック
 *  SPR_SetEosMode          -  スプライト描画偶数・奇数座標選択ビットのセット
 *
 *  このライブラリを使用するには次のインクルードファイルを定義する必要がある。
 *
 *  #include "sega_scl.h"
 *  #include "sega_spr.h"
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_spr.h"
#ifdef _SH
#include <machine.h>
#endif


/* #define  VDP2_INIT */    /* ### */

/*
 * GLOBAL DECLARATIONS
 */
Sint32 SpInitialFlag = 0;                   /* VDP1 Initial Flag             */
Sint32 SpScreenX;                           /* TV Screen X Size              */
Sint32 SpScreenX;                           /* TV Screen X Size              */
Sint32 SpScreenY;                           /* TV Screen Y Size              */
Uint16 SpTvMode;                            /* TV Mode                       */
Uint16 SpFbcrMode;                          /* Frame Buffer Chane Mode       */
Uint16 SpDie;                               /* Double Interlace Enable Flag  */
Uint16 SpDil;                               /* Double Interlace Plot Line    */

/*
 * STATIC DECLARATIONS
 */
static Uint16  SpEraseLTPoint = 0;          /* Erase Area Left-Top Point     */
static Uint16  SpEraseRBPoint = 0;          /* Erase Area Right-Bottom Point */
static Uint16  EraseData   = 0;
static Uint16  EraseLeftX  = 0;
static Uint16  EraseTopY   = 0;
static Uint16  EraseRightX = 0;
static Uint16  EraseBotY   = 0;
static Uint16  BitWidth    = 0;


static XyInt   scrSize[8] = { {320, 224},   /* SPR_TV_320X224  */
                              {320, 240},   /* SPR_TV_320X240  */
                              {352, 224},   /* SPR_TV_352X224  */
                              {352, 240},   /* SPR_TV_352X240  */
                              {640, 224},   /* SPR_TV_640X224  */
                              {640, 240},   /* SPR_TV_640X240  */
                              {704, 224},   /* SPR_TV_704X224  */
                              {704, 240} }; /* SPR_TV_704X240  */
    

/*****************************************************************************
 *
 * NAME:  SPR_Initial()  - Initialize Sprite Environment
 *
 * PARAMETERS :
 *
 *     (1) Uint8  **VRAM       - <o> VRAM アドレスリターンエリアのポインタ
 *     (2) Uint8  **CharVRAM   - <o> キャラクタパターン VRAM アドレスリターン
 *                                   エリアのポインタ
 *
 * DESCRIPTION:
 *
 *     スプライト環境の初期化
 *
 *     (1) スプライトＴＶモードをノーマル（３２０＊２２４）に設定
 *     (2) 偶数・奇数座標選択ビットを０に設定
 *     (3) デフォルトイレーズデータとイレーズエリアの設定
 *         イレーズデータ ＝ 0x8000(黒)
 *         イレーズエリア ＝ 0x0 - 319x223
 *     (4) デフォルトフレーム切り替えモードとしてオートを設定
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
void
SPR_Initial(Uint8 **VRAM)
{
    Uint32  vbr, *irqVector;
    int     iMask;
    Uint32  *l, i;

    /** BEGIN ***************************************************************/
    iMask = get_imask();
    set_imask(15);                           /* interrupt disable           */

    /* FBCR Flag clear */
    SPR_SetEosMode(0);

    /* NTSC, no-turn-around, 16bit/pixel */
    SPR_SetTvMode(SPR_TV_NORMAL, SPR_TV_320X224, 0);

    /* change frame buffer to auto change mode  */
    SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_AUTO | SpFbcrMode);

    /* trig set to auto start drawing */
    SPR_WRITE_REG(SPR_W_PTMR, 0x0002);

    /* set erase write color to Black */
    SPR_WRITE_REG(SPR_W_EWDR, RGB16_COLOR(0,0,0));

    /* set e/w left top point to 0 */
    SPR_WRITE_REG(SPR_W_EWLR, 0x0000);

    /* set e/w right bottom point to ffff */
    SPR_WRITE_REG(SPR_W_EWRR, 0xffff);

#ifdef VDP2_INIT  /* VDP2 + Priority initial */
    /* TV mode  show screen & 320*224 */
    putPortW((Uint16*)(VDP2_REG_BASE + 0x000000), 0x8000);

    /* Sprite Control */
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000e0), 0x0020);

    /* Set Priority */
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f0), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f2), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f4), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f6), 0x0707);
    putPortW((Uint16*)(VDP2_REG_BASE + 0x0000f8), 0x0000);
#endif

    SPR_SetEraseData(RGB16_COLOR(0,0,0), 0, 0, SpScreenX - 1, SpScreenY - 1);

    *VRAM     = (Uint8*)VRAM_ADDR;

    *(*VRAM)     = 0x80;                 /* set END command to VRAM top area */
    *((*VRAM)+1) = 0x00;

    set_imask(iMask);                    /* interrupt enable             */

    SpInitialFlag = 1;                   /* VDP1 Initial Flag ON             */
}


/*****************************************************************************
 *
 * NAME:  SPR_SetTvMode()  - Set TV Mode
 *
 * PARAMETERS :
 *
 *     (1) Uint16  mode            - <i>  ＴＶモード定義値
 *     (2) Uint16  screenSize      - <i>  画面の解像度
 *     (3) Uint16  doubleInterlace - <i>  倍密インタレースフラグ
 *
 * DESCRIPTION:
 *
 *     ＴＶモードを設定する
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_SetTvMode(Uint16 mode, Uint16 screenSize, Uint16 doubleInterlace)
{
    Uint16  vcnt;

    /** BEGIN ***************************************************************/
    SpTvMode = 0x0007 & mode;
    SPR_WRITE_REG(SPR_W_TVMR, SpTvMode);
    if(screenSize > SPR_TV_704X240)
        screenSize = SPR_TV_320X224;
    SpScreenX = scrSize[screenSize].x;
    SpScreenY = scrSize[screenSize].y;
    if(doubleInterlace == ON) SpScreenY <<= 1;

    if((mode == SPR_TV_HIRESO) || (mode == SPR_TV_ROT8))
        BitWidth = 1;  /* 8 bit mode */
    else
        BitWidth = 0;  /* 16 bit mode */

    if(doubleInterlace)
        SpDie = SPR_FBCR_DIE;
    else
        SpDie = 0;
    SpDil = 0;
}


/*****************************************************************************
 *
 * NAME:  SPR_GetStatus()  - Get Sprite Current status
 *
 * PARAMETERS :
 *
 *     (1) struct SpStatus *spStatus  - <o> リターンステータスエリア
 *
 * DESCRIPTION:
 *
 *     現在のスプライト制御の各種状態を返す
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_GetStatus(SprSpStatus *spStatus)
{

    /** BEGIN ***************************************************************/
    spStatus->frameChgMode   = SpFrameChgMode;
    spStatus->frameEraseMode = SpFrameEraseMode;
    spStatus->vbInterval     = VBInterval;      
    spStatus->eraseData      = EraseData;
    spStatus->eraseLeftX     = EraseLeftX;
    spStatus->eraseTopY      = EraseTopY;
    spStatus->eraseRightX    = EraseRightX;
    spStatus->eraseBotY      = EraseBotY;

}


/*****************************************************************************
 *
 * NAME:  SPR_SetEraseData()  - Set Frame Buffer Erase Data & Area
 *
 * PARAMETERS :
 *
 *     (1) Uint16  eraseData     - <i> RGB イレーズデータ
 *     (2) Uint16  leftX         - <i> イレーズエリア左端Ｘ座標
 *     (3) Uint16  topY          - <i> イレーズエリア上端Ｙ座標
 *     (4) Uint16  rightX        - <i> イレーズエリア右端Ｘ座標
 *     (5) Uint16  botY          - <i> イレーズエリア下端Ｙ座標
 *
 * DESCRIPTION:
 *
 *     スプライトＩＣにイレーズデータとイレーズエリアの設定をする
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_SetEraseData(Uint16 eraseData, Uint16 leftX, Uint16 topY,
                             Uint16 rightX, Uint16 botY)
{
    /** BEGIN ***************************************************************/
     EraseData   = eraseData;
     EraseLeftX  = leftX;
     EraseTopY   = topY;
     EraseRightX = rightX;
     EraseBotY   = botY;

     SPR_WRITE_REG(SPR_W_EWDR, eraseData);      /* set erase write data */

     if(BitWidth)
	 leftX >>= 4;
     else
	 leftX >>= 3;
     if(SpDie) topY >>= 1;
     SpEraseLTPoint = (leftX << 9) + topY;
     SPR_WRITE_REG(SPR_W_EWLR, SpEraseLTPoint); /* set erase screen left top */

     rightX++;
     if(BitWidth)
	 rightX >>= 4;
     else
	 rightX >>= 3;
     if(SpDie) botY >>= 1;
     SpEraseRBPoint = (rightX << 9) + botY;
     SPR_WRITE_REG(SPR_W_EWRR, SpEraseRBPoint);
                                            /* set erase screen right bottom */

}


/*****************************************************************************
 *
 * NAME:  SPR_WaitDrawEnd()  - Wait Frame Baffer Draw End
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     スプライトＩＣがフレームバッファへの描画を終了するのを待つ
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *     本ルーチンはＶ－Ｂｒａｎｋ割り込みルーチンから呼び出される
 *
 *
 *****************************************************************************
 */
void
SPR_WaitDrawEnd(void)
{

    /** BEGIN ***************************************************************/
    while((SPR_READ_REG(SPR_R_EDSR) & 0x0002) == 0);
}


/*****************************************************************************
 *
 * NAME:  SPR_SetEosMode()  - Set EOS Bit
 *
 * PARAMETERS :
 *
 *     (1)  Sint32    eosFlag  - <i>  描画モードワードで速度優先描画を指定した
 *                                    時の元絵テクスチャのサンプリング座標
 *                                    ０＝偶数座標をサンプリング（デフォルト）
 *                                    １＝奇数座標をサンプリング
 *
 *
 * DESCRIPTION:
 *
 *     スプライト描画偶数・奇数座標選択ビットのセット
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *
 *****************************************************************************
 */
void
SPR_SetEosMode(Sint32 eosFlag)
{

    /** BEGIN ***************************************************************/
    if(eosFlag)
        SpFbcrMode = SPR_FBCR_EOS;
    else
        SpFbcrMode = 0;
}


#if 0
static Uint16
getPortW(Uint16 *portAddr)
{
   return *portAddr;
}

static void
putPortW(Uint16 *portAddr, Uint16 c)
{
   *portAddr = c;
}
#endif

/*  end of file */
