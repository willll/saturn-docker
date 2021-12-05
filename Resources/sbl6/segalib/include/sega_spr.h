/*----------------------------------------------------------------------------
 *  sega_spr.h -- SPR ライブラリ インクルードファイル
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-07-25 Ver.1.00
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  スプライト表示用インクルードファイルで使用スプライト表示ライブラリの種別に
 *  より以下のように定義します。
 *
 *  （１）スプライト表示基本処理のみの場合
 *
 *        #include "sega_spr.h"
 *
 *  （２）スプライト表示拡張処理の場合
 *
 *        #define  _SPR2_
 *        #include "sega_spr.h"
 *
 *  （３）スプライト３Ｄ表示処理の場合
 *
 *        #define  _SPR3_
 *        #include "sega_spr.h"
 *
 *
 *----------------------------------------------------------------------------
 */

#ifndef SEGA_SPR_H
#define SEGA_SPR_H

#include        "sega_xpt.h"
#include        "sega_def.h"
#include        "sega_mth.h"

/*-----------------------------------
 * Select 3D Execute Machine
 *---------------------------------*/
#define  _PC_         0
#define  _INDIGO2_    1
#define  _SH_         2


/*
 * Set Machine Environment
 */
#define NO_EXIST    0
#define    EXIST    1

#if defined(_PC)
   #define _MACHINE  _PC_
   #define _DSP      NO_EXIST
   #define _MATH        EXIST
#endif

#if defined(_INDIGO2)
   #define _MACHINE  _INDIGO2_
   #define _DSP      NO_EXIST
   #define _MATH        EXIST
#endif

#if defined(_SH)
   #define _MACHINE  _SH_
   #define _DSP         EXIST
   #define _MATH     NO_EXIST
#endif

#ifdef _SPR3_
   #define _SPR2_
#endif

/*
 * EXTERNAL FUNCTION PROTOTYPE DECLARATIONS
 */
typedef void PARA_RTN(void *parm);
void  SPR_InitSlaveSH(void);
void  SPR_RunSlaveSH(PARA_RTN *routine, void *parm);
void  SPR_WaitEndSlaveSH(void);

/**************** SEGA_SP1.H *************************************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define  FIXED32_MIN         0x80000000
#define  FIXED32_MAX         0x7fffffff

#define  VRAM_SIZE           512*1024   /* 512 Kbyte                        */
#define  FBUF_SIZE           256*1024   /* 256 Kbyte * 2 bank               */

#define  AUTO_FRAME_CHG           0     /* auto frame change mode           */
#define  MANUAL_FRAME_CHG         1     /* manual frame change mode         */
#define  NO_INTER_FRAME_CHG       2     /* no interval frame change mode    */
#define  NO_INTER_VBE_FRAME_CHG   3     /* v-blank erase no interval frm chg*/

#define  SPR_FBCR_MANUAL     0x0003     /* frame buffer manual change       */
#define  SPR_FBCR_AUTO       0x0000     /* frame buffer auto change         */
#define  SPR_FBCR_ERASE      0x0002     /* frame buffer next erase on       */
#define  SPR_FBCR_DIE        0x0008     /* double interlace on              */
#define  SPR_FBCR_DIL        0x0004     /* double interlace plot line       */
#define  SPR_FBCR_EOS        0x0010     /* even/odd data sampling select bit*/

#define  SPR_TV_NORMAL       0          /* Normal TV mode                   */
#define  SPR_TV_HIRESO       1          /* High Resolution TV mode          */
#define  SPR_TV_ROT16        2          /* Rotate 16 bit TV mode            */
#define  SPR_TV_ROT8         3          /* Rotate 8 bit TV mode             */
#define  SPR_TV_HDTV         4          /* HDTV TV mode                     */

#define  SPR_TV_320X224      0          /* Screen size 320 x 224            */
#define  SPR_TV_320X240      1          /* Screen size 320 x 240            */
#define  SPR_TV_352X224      2          /* Screen size 352 x 224            */
#define  SPR_TV_352X240      3          /* Screen size 352 x 240            */
#define  SPR_TV_640X224      4          /* Screen size 640 x 224            */
#define  SPR_TV_640X240      5          /* Screen size 640 x 240            */
#define  SPR_TV_704X224      6          /* Screen size 704 x 224            */
#define  SPR_TV_704X240      7          /* Screen size 704 x 240            */

#define  SPR_W_TVMR          0x00000000 /* TV mode selection                */
#define  SPR_W_FBCR          0x00000002 /* Frame buf change mode            */
#define  SPR_W_PTMR          0x00000004 /* Draw start control               */
#define  SPR_W_EWDR          0x00000006 /* Erase write data                 */
#define  SPR_W_EWLR          0x00000008 /* Erase write area left top point  */
#define  SPR_W_EWRR          0x0000000A /* Erase write area right bot point */
#define  SPR_W_ENDR          0x0000000C /* Stop VDP1                        */
#define  SPR_R_EDSR          0x00000010 /* Transfer complete status         */
#define  SPR_R_LOPR          0x00000012 /* End command table address        */
#define  SPR_R_COPR          0x00000014 /* Executing command table address  */
#define  SPR_R_MODR          0x00000016 /* Write register Mode status       */

#define  SPR_R_EXTEN         0x00000002 /* VDP2 External Signal Enable Reg  */
#define  SPR_R_TVSTAT        0x00000004 /* VDP2 TV Status Reg               */
#define  SPR_R_VCNT          0x0000000A /* VDP2 V Counter Reg               */

#define  VRAM_ADDR           0x25C00000 /* Sprite IC VRAM area address      */
#define  FBUF_ADDR           0x25C80000 /* Sprite frame buffer address      */
#define  VDP1_REG_BASE       0x25D00000 /* Sprite IC Register base address  */
#define  VDP2_REG_BASE       0x25F80000 /* Sprite IC Register base address  */

struct SprSpStatus {
    Uint16  frameChgMode;             /* Frame Change Mode                 */
    Uint16  frameEraseMode;           /* Frame Erase  Mode                 */
    Uint16  vbInterval;               /* V-Blank Interval for Frame Change */
    Uint16  eraseData;
    Uint16  eraseLeftX;
    Uint16  eraseTopY;
    Uint16  eraseRightX;
    Uint16  eraseBotY;
};

typedef struct SprSpStatus SprSpStatus;

#define SPR_WRITE_REG(reg, val) \
      *(Uint16*)(VDP1_REG_BASE+(reg)) = (val)
#define SPR_READ_REG(reg) \
      (*(volatile Uint16*)(VDP1_REG_BASE+(reg)))
#define SPR_SCLWRITE_REG(reg, val) \
      *(Uint16*)(VDP2_REG_BASE+(reg)) = (val)
#define SPR_SCLREAD_REG(reg) \
      (*(volatile Uint16*)(VDP2_REG_BASE+(reg)))

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
extern Sint32  SpScreenX;               /* TV Screen X Size                  */
extern Sint32  SpScreenY;               /* TV Screen Y Size                  */
extern volatile Uint16  SpFrameChgMode;          /* Frame Change Mode                 */
extern volatile Sint32  SpFrameEraseMode;        /* Frame Erase  Mode                 */
extern volatile Sint32  VBInterval;              /* V-Blank Interval for Frame Change */
extern volatile Sint32  VBIntervalCounter;       /* V-Blank Interval Counter          */
extern volatile Sint32  ReqDisplayFlag;          /* Display Request flag              */
extern Uint16  SpTvMode;                /* TV Mode                           */
extern Uint16  SpFbcrMode;              /* Frame Buffer Chane Mode           */
extern Uint16  SpDie;                   /* Double Interlace Enable Flag      */
extern Uint16  SpDil;                   /* Double Interlace Plot Line        */

/*
 * EXTERNAL FUNCTION PROTOTYPE DECLARATIONS
 */
void SPR_Initial (Uint8 **VRAM);
void SPR_SetTvMode(Uint16 mode, Uint16 screenSize, Uint16 doubleInterlace);
void SPR_GetStatus (SprSpStatus *spStatus);
void SPR_SetEraseData (Uint16 eraseData, Uint16 leftX,  Uint16 topY,
                                         Uint16 rightX, Uint16 botY);
void SPR_WaitDrawEnd (void);
void SPR_SetEosMode(Sint32 eosFlag);


#ifdef _SPR2_
/**************** SEGA_SP2.H *************************************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define  CTRL_END            0x8000     /* control word end bit mask        */
#define  CTRL_SKIP           0x4000     /* control word skip bit mask       */
#define  CTRL_JUMP           0x3000     /* control word jump bit mask       */
#define  CTRL_ZOOM           0x0f00     /* control word zoom point bit mask */
#define  CTRL_FUNC           0x000f     /* control word function bit mask   */
#define  CTRL_DIR            0x0030     /* control word char read bit mask  */

#define  DRAW_COMPO          0x0003     /* draw mode word color compose mask*/
#define  DRAW_GOURAU         0x0004     /* draw mode word gouraud shading   */
#define  DRAW_GOURAU_GRAY    0x0804     /* draw mode word gray gour shading */
#define  DRAW_COLOR          0x0038     /* draw mode word color mode mask   */
#define  DRAW_MESH           0x0100     /* draw mode word mesh on           */

#define  COMPO_REP           0x0000     /* color compose reprace mode       */
#define  COMPO_SHADOW        0x0001     /* color compose shadow mode        */
#define  COMPO_HARF          0x0002     /* color compose harf luminance mode*/
#define  COMPO_TRANS         0x0003     /* color compose trunslucent mode   */

#define  FUNC_NORMALSP       0x0000     /* draw normal sprite function      */
#define  FUNC_SCALESP        0x0001     /* draw scaled sprite function      */
#define  FUNC_DISTORSP       0x0002     /* draw distorted sprite function   */
#define  FUNC_POLYGON        0x0004     /* draw polygon function            */
#define  FUNC_POLYLINE       0x0005     /* draw polyline function           */
#define  FUNC_LINE           0x0006     /* draw line function               */
#define  FUNC_SCLIP          0x0009     /* set system clipping function     */
#define  FUNC_UCLIP          0x0008     /* set user clipping function       */
#define  FUNC_LCOORD         0x000a     /* set local coordinate function    */
#define  FUNC_TEXTURE        0x0004     /* texture command group mask       */

#define  JUMP_NEXT           0x0000     /* jump next command                */
#define  JUMP_ASSIGN         0x1000     /* jump assign command              */
#define  JUMP_CALL           0x2000     /* call assign command              */
#define  JUMP_RETURN         0x3000     /* return command subroutine        */

#define  ZOOM_NOPOINT        0x0000     /* zoom no point                    */
#define  ZOOM_TL             0x0500     /* zoom point top left              */
#define  ZOOM_TM             0x0600     /* zoom point top middle            */
#define  ZOOM_TR             0x0700     /* zoom point top right             */
#define  ZOOM_ML             0x0900     /* zoom point middle left           */
#define  ZOOM_MM             0x0a00     /* zoom point center                */
#define  ZOOM_MR             0x0b00     /* zoom point middle right          */
#define  ZOOM_BL             0x0c00     /* zoom point bottom left           */
#define  ZOOM_BM             0x0e00     /* zoom point bottom middle         */
#define  ZOOM_BR             0x0f00     /* zoom point bottom right          */

#define  DIR_NOREV           0x0000     /* char read not reverse            */
#define  DIR_TBREV           0x0020     /* char read top/bottom reverse     */
#define  DIR_LRREV           0x0010     /* char read left/right reverse     */
#define  DIR_LRTBREV         0x0030     /* char read left/right/top/bot rev */

#define  ECD_DISABLE         0x0080     /* ECD disabe & SPD enable          */
#define  SPD_DISABLE         0x0040     /* ECD enable & SPD disable         */
#define  ECDSPD_DISABLE      0x00c0     /* ECD & SPD disable                */
#define  UCLPIN_ENABLE       0x0400     /* CLIP IN enable                   */
#define  UCLPOUT_ENABLE      0x0600     /* CLIP OUT enable                  */
#define  HSS_ENABLE          0x1000     /* HSS enable                       */
#define  PCLP_ENABLE         0x0800     /* PCLP disable                     */

#define  COLOR_0             0x0000     /* 4 bit/pixel & 16 color bank mode */
#define  COLOR_1             0x0008     /* 4 bit/pixel & 16 color lookup tbl*/
#define  COLOR_2             0x0010     /* 8 bit/pixel &  64 color bank mode*/
#define  COLOR_3             0x0018     /* 8 bit/pixel & 128 color bank mode*/
#define  COLOR_4             0x0020     /* 8 bit/pixel & 256 color bank mode*/
#define  COLOR_5             0x0028     /* 16 bit/pixel & RGB mode          */

#define  NO_GOUR             0xffff     /* no use Gouraud Shading Table No. */

#define  SPR_2DRAW_PRTY_ON   1          /* command draw priority on         */
#define  SPR_2DRAW_PRTY_OFF  0          /* command draw priority off        */
#define  SPR_2MOST_FAR       0          /* draw sprite most far position    */
#define  SPR_2MOST_NEAR      0x7fffffff /* draw sprite most near position   */


/*
 * STRUCTURE DECLARATIONS
 */
struct SprSpCmd {                       /* Sprite Command Table             */
    Uint16  control;                    /* control word                     */
    Uint16  link;                       /* command link                     */
    Uint16  drawMode;                   /* draw mode                        */
    Uint16  color;                      /* color info.                      */
    Uint16  charAddr;                   /* character address                */
    Uint16  charSize;                   /* character size                   */
    Sint16  ax;                         /* point A x                        */
    Sint16  ay;                         /* point A y                        */
    Sint16  bx;                         /* point B x                        */
    Sint16  by;                         /* point B y                        */
    Sint16  cx;                         /* point C x                        */
    Sint16  cy;                         /* point C y                        */
    Sint16  dx;                         /* point D x                        */
    Sint16  dy;                         /* point D y                        */
    Uint16  grshAddr;                   /* gouraud shading table address    */
    Uint16  dummy;                      /* dummy area                       */
};

struct SprGourTbl {
    Uint16  entry[4];
};

struct SprLookupTbl {
    Uint16  entry[16];
};

/*
 * TYPEDEFS
 */
typedef Uint16 Vaddr;                       /* Relative Address in The VRAM  */
typedef struct SprSpCmd     SprSpCmd;
typedef struct SprGourTbl   SprGourTbl;
typedef struct SprLookupTbl SprLookupTbl;
/* internal data type */
typedef	struct SprCharTbl     SprCharTbl;
typedef	struct SprCmdChain    SprCmdChain;
typedef struct SprDrawPrtyBlk SprDrawPrtyBlk;
typedef	struct Spr2WorkArea   Spr2WorkArea;
typedef	struct SprAreaSize    SprAreaSize;

struct SprCharTbl {                         /* Character Entry Table         */
    Vaddr     addr;                         /* Character Relative Address    */
    Uint16    xySize;                       /* Charcter Size (X * Y)         */
    Uint16    mode;                         /* Charcter Mode (0-5)           */
    Uint16    color;                        /* Color Bank or LookUp Tbl No   */
    Uint16    size;                         /* Charcter Size (by the 32 byte)*/
};

struct SprCmdChain {            /* コマンドチェイン管理テーブル              */
    Uint16      nextNo;         /* 次のコマンドチェインエントリ番号          */
    Uint16      cmdNo;          /* 対応するコマンド番号                      */
};

struct SprDrawPrtyBlk {         /* 描画プライオリティブロックテーブル        */
    Uint16      topNo;          /* 先頭コマンドチェインエントリ番号          */
    Uint16      botNo;          /* 末尾コマンドチェインエントリ番号          */
};

//struct Spr2WorkArea {                    /* SP2 Work Area Define             */
//    Uint16         commandMax;           /* Command Max                      */
//    Uint16         gourTblMax;           /* Gour Tbl Max                     */
//    Uint16         lookupTblMax;         /* Lookup Tbl Max                   */
//    Uint16         charMax;              /* Char Max                         */
//    Uint16         drawPrtyBlkMax;       /* Draw Priority Block Max          */
//    SprCharTbl     *charTbl;             /* Character Tbl Pointer            */
//    SprCmdChain    *cmdChain;            /* Command Draw Priority Chain Tbl  */
//    SprDrawPrtyBlk *drawPrtyBlk;         /* Draw Priority Block Start        */
//};

struct Spr2WorkArea {                    /* SP2 Work Area Define             */
    Uint16         commandMax;           /* Command Max                      */
    Uint16         gourTblMax;           /* Gour Tbl Max                     */
    Uint16         lookupTblMax;         /* Lookup Tbl Max                   */
    Uint16         charMax;              /* Char Max                         */
    Uint16         drawPrtyBlkMax;       /* Draw Priority Block Max          */
    SprCharTbl     *charTbl;             /* Character Tbl Pointer            */
    SprSpCmd       *mstCmd;              /* Master Command Buffer            */
    SprDrawPrtyBlk *mstDrawPrtyBlk;      /* Master Draw Priority Block       */
    SprSpCmd       *slvCmd;              /* Slave Command Buffer             */
    SprDrawPrtyBlk *slvDrawPrtyBlk;      /* Slave Draw Priority Block        */
    SprGourTbl     *gourTbl;             /* Gour Tbl Max                     */
};

struct SprAreaSize {                     /* SP2 Work Area Size               */
    Uint16         commandMax;           /* Command Max                      */
    Uint16         gourTblMax;           /* Gour Tbl Max                     */
    Uint16         lookupTblMax;         /* Lookup Tbl Max                   */
    Uint16         charMax;              /* Char Max                         */
    Uint16         drawPrtyBlkMax;       /* Draw Priority Block Max          */
    Uint16         useCommandCount;      /* Using Command Block Count        */
    Uint16         charRemainBlkCount;   /* Char Total Remain Null Blk Count */
    Uint16         charNullBlkMaxSize;   /* Char Null Block Max Size         */
};

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
extern Sint32  SpLCoordX;                   /* Local Origin X Position       */
extern Sint32  SpLCoordY;                   /* Local Origin Y Position       */


#define   SPR_2DefineWork(sp2WorkArea, sp2CommandMax, sp2GourTblMax, \
		          sp2LookupTblMax, sp2CharMax, sp2DrawPrtyBlkMax) \
static SprCharTbl      sp2CharTbl      [sp2CharMax]; \
static SprCmdChain     sp2CmdChain     [sp2CommandMax]; \
static SprDrawPrtyBlk  sp2DrawPrtyBlk  [sp2DrawPrtyBlkMax]; \
static Spr2WorkArea sp2WorkArea  = {  \
        sp2CommandMax, sp2GourTblMax, sp2LookupTblMax, sp2CharMax, \
        sp2DrawPrtyBlkMax, sp2CharTbl, sp2CmdChain, sp2DrawPrtyBlk };


/*
 * PROTOTYPE DECLARATIONS
 */
void   SPR_2Initial (Spr2WorkArea  *workArea);
void   SPR_2SetTvMode(Uint16 mode, Uint16 screenSize, Uint16 doubleInterlace);
void   SPR_2FrameChgIntr (Uint16);
void   SPR_2FrameEraseData (Uint16);
void   SPR_2SetGourTbl (Uint16, SprGourTbl *);
void   SPR_2SetLookupTbl (Uint16, SprLookupTbl *);
//void   SPR_2SetChar (Uint16, Uint16, Uint16, Uint16, Uint16, Uint8 *);
int		SPR_2SetChar(Uint16, Uint16, Uint16, Uint16, Uint16, Uint8 *);
void   SPR_2ClrChar (Uint16);
void   SPR_2ClrAllChar (void);
Uint16 SPR_2GourTblNoToVram (Uint16);
Uint16 SPR_2LookupTblNoToVram (Uint16);
Uint16 SPR_2CharNoToVram (Uint16);
void   SPR_2GetAreaSizeInfo(SprAreaSize  *areaSizeInfo);
void   SPR_2OpenCommand (Uint16);
void   SPR_2CloseCommand (void);
void   SPR_2FlushDrawPrty (void);
void   SPR_2LocalCoord (Sint32, XyInt *);
void   SPR_2SysClip (Sint32, XyInt *);
void   SPR_2UserClip (Sint32, XyInt *);
void   SPR_2Line (Sint32, Uint16, Uint16, XyInt *, Uint16);
void   SPR_2PolyLine (Sint32, Uint16, Uint16, XyInt *, Uint16);
void   SPR_2Polygon (Sint32, Uint16, Uint16, XyInt *, Uint16);
void   SPR_2NormSpr (Sint32, Uint16, Uint16, Uint16, Uint16, XyInt *, Uint16);
void   SPR_2ScaleSpr (Sint32, Uint16, Uint16, Uint16, 
                                      Uint16, XyInt *xy, Uint16);
void   SPR_2DistSpr (Sint32, Uint16, Uint16, Uint16,
                                     Uint16, XyInt *xy, Uint16);
void   SPR_2Cmd (Sint32, SprSpCmd *);

#endif /* _SPR2_ */

#ifdef _SPR3_
/**************** SEGA_SP3.H *************************************************/
/*
 * TYPEDEFS
 */
typedef	struct SprCluster   SprCluster;
typedef	struct SprObject3D  SprObject3D;
typedef	struct SprSurface   SprSurface;
typedef	struct SprInbetInf  SprInbetInf;
typedef	struct SprTexture   SprTexture;
typedef struct Spr3dStatus  Spr3dStatus;
/* internal data type */
typedef	struct Spr3WorkArea Spr3WorkArea;


/*
 * STRUCTURE DECLARATIONS
 */
struct SprCluster {
    Uint16	no;             /* クラスタ番号                              */
    Uint16	angleSeq;       /* 回転順序                                  */
    MthXyz	angle;	        /* 親クラスタの座標系における回転            */
    MthXyz	point;	        /* 親クラスタの座標系における原点の座標      */
    SprObject3D	*object;        /* ３Ｄオブジェクト                          */
    SprCluster	*next;       	/* 次クラスタ                                */
    SprCluster	*child;      	/* 子クラスタ                                */
    SprInbetInf *inbetInf;     	/* ３Ｄオブジェクト間接続ポリゴン情報        */
    void        (*transStart)(SprCluster*);
                                /* 座標変換開始前ユーザコールバックルーチン  */
    void        (*transEnd)(SprCluster*, SprObject3D*, MthMatrix*, MthXyz*);
                                /* 座標変換終了後ユーザコールバックルーチン  */
    void        *context;       /* ユーザコンテキストエリア                  */
};

struct SprObject3D {
    Uint16	no;             /* オブジェクト番号                          */
    Uint16      dispFlag;       /* 表示フラグ                                */
    Uint16      vertCount;      /* 頂点数                                    */
    Uint16      surfaceCount;   /* 面数                                      */
    MthXyz      *vertPoint;     /* 頂点座標テーブル                          */
    MthXyz      *vertNormal;    /* 頂点の法線ベクトルテーブル                */
    SprSurface  *surface;       /* 面定義テーブル                            */
    MthXyz      *surfaceNormal; /* 面の法線ベクトルテーブル                  */
    MthXyz      *surfaceVert;   /* 面の代表頂点座標テーブル                  */
    Uint16      **shdIdxTbl;    /* シェーディングインデックステーブル        */
    Fixed32     surfNormK;      /* 面法線ベクトルの計算補正値                */
    SprObject3D *next;          /* 次３Ｄオブジェクト                        */
};

struct SprSurface {
    Uint16     vertNo[4];       /* 面を構成する頂点番号                      */
    Uint16     drawMode;        /* 描画モード                                */
    Uint16     color;           /* カラーデータ                              */
};

struct SprInbetInf {
    Sint32       vertCount;     /* 取り出し頂点数                            */
    SprObject3D  *fromObj;      /* 接続頂点データ取り出しオブジェクト        */
    Uint16       *fromVertNo;   /* 取り出し頂点番号配列                      */
    SprObject3D  *toObj;        /* 接続頂点データ設定オブジェクト            */
    Uint16       *toVertNo;     /* 設定頂点番号配列                          */
    SprInbetInf  *next;         /* 次３Ｄオブジェクト間接続ポリゴン情報      */
};

struct SprTexture {
    Uint16       charNo;        /* キャラクタ番号                            */
    Uint16       colorMode;     /* カラーモード                              */
    Uint16       color;         /* カラーデータ（ｶﾗｰﾊﾞﾝｸorﾙｯｸｱｯﾌﾟﾃｰﾌﾞﾙNo）   */
    Uint16       width;         /* キャラクタの幅                            */
    Uint16       height;        /* キャラクタの高さ                          */
    Uint8        *charData;     /* キャラクタデータのポインタ                */
    SprLookupTbl *lookupTbl;    /* ルックアップテーブルのポインタ            */
};

struct Spr3WorkArea {
    Uint16      objSurfMax;        /* オブジェクト内最大面数                 */
    Uint16      objVertMax;        /* オブジェクト内最大頂点数               */
    Sint32      *surfBright[2];    /* 面輝度テーブル                         */
    Sint32      *vertBright[2];    /* 頂点輝度テーブル                       */
    MthXyz      *coordView3D[2];   /* ３Ｄ視点座標変換後テーブル             */
    MthXyz      *coordWorld3D[2];  /* ３Ｄワールド座標変換後テーブル         */
    XyInt       *coord2D;          /* ２Ｄ透視変換後テーブル                 */
    Uint8       *coord2DFlag;      /* ２Ｄ透視変換処理フラグテーブル         */
};

struct Spr3dStatus {
    MthXyz    lightAngle;       /* 光源アングル                           */
    MthXyz    viewCoordPoint;   /* 視点座標系内の視点位置                 */
    MthXyz    viewPoint;        /* 視点座標                               */
    MthXyz    viewAngle;        /* 視点アングル                           */
    Sint32    viewAngleSeq;     /* 視点アングル回転操作                   */
    Sint32    zSortMode;        /* ＺソートＺ座標値の採用モード           */
    Fixed32   zSortZMin;        /* フラッシュ単位現Ｚ座標最小値           */
    Fixed32   zSortZMax;        /* フラッシュ単位現Ｚ座標最大値           */
    Fixed32   clipZMin;         /* 視点座標系クリップＺ最小値座標         */
    Fixed32   clipZMax;         /* 視点座標系クリップＺ最大値座標         */
    Sint32    clipLevel;        /* クリッピングレベル                     */
    MthXy     unitPixel;        /* X,Y 1.0 に対するスクリーンのピクセル数 */

// WTF ?
    Sint32    unitPixelX;       /* X 1.0 に対するスクリーンのピクセル数   */
    Sint32    unitPixelY;       /* Y 1.0 に対するスクリーンのピクセル数   */
};


typedef struct TComTbl  TComTbl;
struct TComTbl {
    Fixed32     zSortBZMin;     /* i   フラッシュ単位前Ｚ座標最小値          */
    int         zSftCnt;        /* i   ＺソートＺシフト数                    */
    int         zSortBlkCnt;    /* i   Ｚソートブロック数                    */
    Sint32      spriteDrawFlag; /* i   変形スプライト描画フラグ              */
    Sint32      otherDrawFlag;  /* i   その他ポリゴン描画フラグ              */
    Sint32      *unitPixel;     /* i   X,Y 1.0に対するスクリーンのピクセル数 */
    Fixed32     *clipRatio;     /* i   ３Ｄクリッピング率 [0]=X, [1]=Y       */
    Fixed32     *clipZ;         /* i   視点座標系クリップＺ [0]=min, [1]=max */
    SprCharTbl  *charTbl;       /* i   スプライトキャラクタテーブル          */
    Sint32      *vertBright;    /* i   頂点輝度テーブル                      */
    MthXyz      *coordView3D;   /* i   ３Ｄ視点座標変換後テーブル            */
    XyInt       *coord2D;       /* i   ２Ｄ透視変換後テーブル                */
    int         dispFlag;       /* i   表示フラグ                            */
    Uint16      **shdIdxTbl;    /* i   シェーディングインデックステーブル    */
    Uint32      gourGrayTopR;   /* i   グーログレイ先頭ＶＲＡＭアドレス      */
    Uint32      lookupTblR;     /* i   ルックアップテーブルＶＲＡＭ内位置    */
    MthXyz      *lightVector;   /* i   光源ベクタ                            */
    MthMatrix   *viewMatrix;    /* i   視点座標系への座標変換マトリックス    */
    MthMatrix   *worldMatrix;   /* i   ワールド座標系への座標変換マトリックス*/
};

typedef struct T3Dto2D  T3Dto2D;
struct T3Dto2D {
    int         vertCount;      /* i   頂点数                                */
    int         surfaceCount;   /* i   面数                                  */
    SprSurface  *surfInfo;      /* i   面定義テーブル                        */
    MthXyz      *vertPoint;     /* i   頂点座標テーブル                      */
    MthXyz      *surfaceNormal; /* i   面の法線ベクトルテーブル              */
    MthXyz      *vertNormal;    /* i   頂点の法線ベクトルテーブル            */
    MthXyz      *coordView3D;   /* o   ３Ｄ視点座標変換後テーブル            */
    XyInt       *coord2D;       /* o   ２Ｄ透視変換後テーブル                */
    Sint32      *surfBright;    /* o   面輝度テーブル                        */
    Sint32      *vertBright;    /* o   頂点輝度テーブル                      */
    MthXyz      *coordWorld3D;  /* o   頂点のワールド座標テーブル            */
};

typedef struct T2DtoCmd  T2DtoCmd;
struct T2DtoCmd {
    int         surfaceCount;   /* i   面数                                  */
    SprSurface  *surface;       /* i   面定義テーブル                        */
    Sint32      *surfBright;    /* i   面輝度テーブル                        */
    Fixed32     zSortZMin;      /* i/o フラッシュ単位現Ｚ座標最小値          */
    Fixed32     zSortZMax;      /* i/o フラッシュ単位現Ｚ座標最大値          */
    SprDrawPrtyBlk *prtyTbl;    /* i/o 描画プライオリティブロックテーブル    */
    SprSpCmd    *cmdBuf;        /* o   スプライトコマンドバッファ            */
    int         outCmdPos;      /* o   書き込みコマンド数                    */
    SprGourTbl  *gourTblCur;    /* i/o グーロテーブルカレントアドレス        */
    Uint32      gourTblCurR;    /* i/o グーロカレントＶＲＡＭアドレス        */
};

typedef struct GetSurfNormPara  GetSurfNormPara;
struct GetSurfNormPara {
    int         surfaceCount;   /* i   面数                                  */
    SprSurface  *surface;       /* i   面定義テーブル                        */
    MthXyz      *vertPoint;     /* i   頂点座標テーブル                      */
    Fixed32     surfNormK;      /* i   面法線ベクトルの計算補正値            */
    MthXyz      *surfaceNormal; /* o   面の法線ベクトルテーブル              */
};

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define   ROT_SEQ_ZYX       0x0000
#define   ROT_SEQ_ZXY       0x0001
#define   ROT_SEQ_YZX       0x0002
#define   ROT_SEQ_YXZ       0x0003
#define   ROT_SEQ_XYZ       0x0004
#define   ROT_SEQ_XZY       0x0005

#define   DISP_POLYGON      0x0000
#define   DISP_POLYLINE     0x8000
#define   DISP_MASK         0xc000
#define   BOTH_FACE         0x1000

#define   NO_SHADING        0x0000
#define   FLAT_SHADING      0x0100
#define   GOURAUD_SHADING   0x0200
#define   SHADING_MASK      0x0300
#define   INBETWEEN_OBJECT  0x0010

#define   MATE_COLOR        0x0000
#define   MATE_TEXTURE      0x4000
#define   MATE_SHADING_TBL  0x8000
#define   MATE_AUTO_SHADING 0xc000
#define   MATE_MASK         0xc000

#define   ZSORT_MIN         0x0000
#define   ZSORT_MAX         0x1000
#define   ZSORT_MID         0x2000
#define   ZSORT_MASK        0x3000
#define   ZSORT_FLOAT_MODE  0
#define   ZSORT_FIXED_MODE  1

#define   DRAW_MODE_MASK    0x07ff

#ifdef SPR_3USE_DOUBLE_BUF
#define   SPR_3mac1(sp3ObjSurfMax) \
static Sint32    sp3SurfBright1 [sp3ObjSurfMax]; \
static Sint32    sp3SurfBright2 [sp3ObjSurfMax];
#else
#define   SPR_3mac1(sp3ObjSurfMax) \
static Sint32    sp3SurfBright1 [sp3ObjSurfMax];
#endif

#ifdef SPR_3USE_DOUBLE_BUF
  #ifdef SPR_3NEED_WORLD_VERT
#define   SPR_3mac2(sp3ObjVertMax) \
static Sint32    sp3VertBright1   [sp3ObjVertMax];   \
static MthXyz    sp3CoordView3D1  [sp3ObjVertMax];   \
static MthXyz    sp3CoordWorld3D1 [sp3ObjVertMax];   \
static Sint32    sp3VertBright2   [sp3ObjVertMax];   \
static MthXyz    sp3CoordView3D2  [sp3ObjVertMax];   \
static MthXyz    sp3CoordWorld3D2 [sp3ObjVertMax];   \
static XyInt     sp3Coord2D       [sp3ObjVertMax];   \
static Uint8     sp3Coord2DFlag   [sp3ObjVertMax];
  #else
#define   SPR_3mac2(sp3ObjVertMax) \
static Sint32    sp3VertBright1   [sp3ObjVertMax];   \
static MthXyz    sp3CoordView3D1  [sp3ObjVertMax];   \
static Sint32    sp3VertBright2   [sp3ObjVertMax];   \
static MthXyz    sp3CoordView3D2  [sp3ObjVertMax];   \
static XyInt     sp3Coord2D       [sp3ObjVertMax];   \
static Uint8     sp3Coord2DFlag   [sp3ObjVertMax];
  #endif
#else
  #ifdef SPR_3NEED_WORLD_VERT
#define   SPR_3mac2(sp3ObjVertMax) \
static Sint32    sp3VertBright1   [sp3ObjVertMax];   \
static MthXyz    sp3CoordView3D1  [sp3ObjVertMax];   \
static MthXyz    sp3CoordWorld3D1 [sp3ObjVertMax];   \
static XyInt     sp3Coord2D       [sp3ObjVertMax];   \
static Uint8     sp3Coord2DFlag   [sp3ObjVertMax];
  #else
#define   SPR_3mac2(sp3ObjVertMax) \
static Sint32    sp3VertBright1   [sp3ObjVertMax];   \
static MthXyz    sp3CoordView3D1  [sp3ObjVertMax];   \
static XyInt     sp3Coord2D       [sp3ObjVertMax];   \
static Uint8     sp3Coord2DFlag   [sp3ObjVertMax];
  #endif
#endif

#ifdef SPR_3USE_DOUBLE_BUF
  #ifdef SPR_3NEED_WORLD_VERT
#define   SPR_3mac3(sp3WorkArea, sp3ObjSurfMax, sp3ObjVertMax)  \
static Spr3WorkArea  sp3WorkArea = {                          \
   sp3ObjSurfMax, sp3ObjVertMax, \
   sp3SurfBright1, sp3SurfBright2, sp3VertBright1, sp3VertBright2, \
   sp3CoordView3D1, sp3CoordView3D2, sp3CoordWorld3D1, sp3CoordWorld3D2, \
   sp3Coord2D, sp3Coord2DFlag };
  #else
#define   SPR_3mac3(sp3WorkArea, sp3ObjSurfMax, sp3ObjVertMax)  \
static Spr3WorkArea  sp3WorkArea = {                          \
   sp3ObjSurfMax, sp3ObjVertMax, \
   sp3SurfBright1, sp3SurfBright2, sp3VertBright1, sp3VertBright2, \
   sp3CoordView3D1, sp3CoordView3D2,              NULL,            NULL, \
   sp3Coord2D, sp3Coord2DFlag };
  #endif
#else
  #ifdef SPR_3NEED_WORLD_VERT
#define   SPR_3mac3(sp3WorkArea, sp3ObjSurfMax, sp3ObjVertMax) \
static Spr3WorkArea  sp3WorkArea = {                          \
   sp3ObjSurfMax, sp3ObjVertMax, \
   sp3SurfBright1,           NULL, sp3VertBright1,           NULL, \
   sp3CoordView3D1,          NULL, sp3CoordWorld3D1,         NULL, \
   sp3Coord2D, sp3Coord2DFlag };
  #else
#define   SPR_3mac3(sp3WorkArea, sp3ObjSurfMax, sp3ObjVertMax) \
static Spr3WorkArea  sp3WorkArea = {                          \
   sp3ObjSurfMax, sp3ObjVertMax, \
   sp3SurfBright1,            NULL, sp3VertBright1,           NULL, \
   sp3CoordView3D1,           NULL,             NULL,             NULL, \
   sp3Coord2D, sp3Coord2DFlag };
  #endif
#endif

#define   SPR_3DefineWork(sp3WorkArea, sp3ObjSurfMax, sp3ObjVertMax) \
 SPR_3mac1(sp3ObjSurfMax) \
 SPR_3mac2(sp3ObjVertMax) \
 SPR_3mac3(sp3WorkArea, sp3ObjSurfMax, sp3ObjVertMax)

/*
 * PROTOTYPE DECLARATIONS
 */
void   SPR_3Initial       (Spr3WorkArea  *workArea);
void   SPR_3SetLight      (Uint16 moveKind, MthXyz *lightAngle);
void   SPR_3SetView       (Uint16 moveKind, MthXyz *viewPoint,
                           MthXyz *viewAngle, Uint16 angleSeq,
                           MthXyz *viewCoordPoint);
void   SPR_3MoveCluster   (SprCluster *cluster, Uint16 moveKind,
					   MthXyz *angle, MthXyz *point);
void   SPR_3DrawModel     (SprCluster *rootCluster);
void   SPR_3Flush         (void);
void   SPR_3SetTexture    (SprTexture *texture);
void   SPR_3ChangeTexColor(Uint16 charNo, Uint16 color,
                                          SprLookupTbl *lookupTbl);
void   SPR_3ClrTexture    (SprTexture *texture);
void   SPR_3SetZSortMinMax(Uint16 pZSortMode,
                                       Fixed32 pZSortZMin, Fixed32 pZSortZMax);
void   SPR_3SetClipLevel  (Uint16 pClipLevel,
				       Fixed32 pClipZMin, Fixed32 pClipZMax);
void   SPR_3SetPixelCount (Uint16 pixelCountX, Uint16 pixelCountY);
void   SPR_3GetStatus (Spr3dStatus *spr3dStatus);
void   SPR_3CallAllCluster(SprCluster *cluster, void (*userFunc)(SprCluster*));
void   SPR_3SetSurfNormVect(SprObject3D  *obj);
void   SPR_3SetDrawSpeed(Sint32 hssFlag, Sint32 eosFlag, Sint32 pclpFlag);

/*
 * ＤＳＰによるマトリックス合成
 */
#define MTX_OP_CLEAR	1            /*　出力マトリックスエリアのクリア      */
#define MTX_OP_REV_Z	2            /*　Ｚの符号反転合成                    */
#define MTX_OP_MOV_XYZ	3            /*　平行移動合成                        */
#define MTX_OP_ROT_X	4            /*　Ｘ回転移動合成                      */
#define MTX_OP_ROT_Y	5            /*　Ｙ回転移動合成                      */
#define MTX_OP_ROT_Z	6            /*　Ｚ回転移動合成                      */
#define MTX_OP_END	7            /*　処理終了                            */

typedef struct  SprDspMatrixOp {     /* マトリックス合成オペレーション       */
         Fixed32     x;              /* Ｘ値                                 */
         Fixed32     y;              /* Ｙ値                                 */
         Fixed32     z;              /* Ｚ値                                 */
         Sint32      opCode;         /* オペレーションコード                 */
} SprDspMatrixOp;

typedef struct  SprDspMatrixParm {   /* ＤＳＰ用マトリックス合成パラメータ   */
         MthMatrix   *inputMatrix;   /* 入力マトリックスのアドレス           */
         MthMatrix   *outputMatrix;  /* 出力マトリックスのアドレス           */
         SprDspMatrixOp  op[15];     /* マトリックス合成オペレーション       */
} SprDspMatrixParm;


#endif /* _SPR3_ */

#endif /* ifndef SEGA_SPR_H */

/*  end of file */
