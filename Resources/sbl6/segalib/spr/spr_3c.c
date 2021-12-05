/*----------------------------------------------------------------------------
 *  spr_3c.c -- SPR ライブラリ 3C モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  このライブラリはスプライト３Ｄ表示処理モジュールで、以下のルーチンを含む。
 *
 *  SPR_3Initial            -  ３Ｄスプライト表示の初期化
 *  SPR_3SetLight           -  光源ベクタのセット
 *  SPR_3SetView            -  視点のセット
 *  SPR_3CallAllCluster     -  全クラスタに対するファンクションコール
 *  SPR_3MoveCluster        -  クラスタの移動
 *  SPR_3DrawModel          -  モデルの登録
 *  SPR_3Flush              -  モデルの描画
 *  SPR_3SetTexture         -  テクスチャのセット
 *  SPR_3ChangeTexColor     -  テクスチャカラーデータの変更
 *  SPR_3ClrTexture         -  テクスチャエリアの解放
 *  SPR_3SetClipLevel       -  クリッピングレベルのセット
 *  SPR_3SetZSortMinMax     -  視点座標系Ｚソート範囲の最小・最大値をセット
 *  SPR_3SetPixelCount      -  単位ピクセル数のセット
 *  SPR_3SetTransPostRtn    -  座標変換結果通知ルーチンのセット
 *  SPR_3GetStatus          -  現３Ｄ環境データの取得
 *  SPR_3SetSurfNormVect    -  ３Ｄオブジェクトの面法線を計算しセット
 *  SPR_3SetDrawSpeed       -  ＶＤＰ１の高速描画パラメータをセット
 *
 *  このライブラリを使用するには次のインクルードファイルとワークエリアを定義す
 *  る必要がある。
 *
 *   #include "sega_scl.h"
 *   #define  _SPR3_
 *   #include "sega_spr.h"
 *
 *   #define CommandMax    100          // 最大コマンド数
 *   #define GourTblMax    100          // 最大グーローテーブル数
 *   #define LookupTblMax  100          // 最大ルックアップテーブル数
 *   #define CharMax       100          // 最大キャラクタ数
 *   #define DrawPrtyMax   256          // 最大プライオリティブロック数
 *   SPR_2DefineWork(work2D,
 *                CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)
 *                                      // ２Ｄワークエリア定義
 *
 *   #define ObjSurfMax     16          // ３Ｄオブジェクトの最大面数
 *   #define ObjVertMax     16          // ３Ｄオブジェクトの最大頂点数
 *   SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax)
 *                                      // ３Ｄワークエリア定義
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */

#include <string.h>

#define  _SPR3_
#include "sega_spr.h"

#ifdef _SH
#include <machine.h>
#endif

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define   SCREEN_W_SIZE     256
#define   SCREEN_H_SIZE     256

#define   FRMBUF_MIN_X     -1000
#define   FRMBUF_MAX_X      1000
#define   FRMBUF_MIN_Y     -1000
#define   FRMBUF_MAX_Y      1000

#define   FIXED_MIN        0x80000000
#define   FIXED_MAX        0x7fffffff
#define   CLIP_MIN_Z       FIXED_MIN
#define   CLIP_MAX_Z       MTH_FIXED(-0.005)
#define   ZSORT_MIN_Z      MTH_FIXED(-10.0)
#define   ZSORT_MAX_Z      MTH_FIXED(0)

#define	WORLD_MATRIX_STACK_SIZE	32

/*
 * DATA TYPE DECLARATIONS
 */
typedef struct PolTbl  PolTbl;
struct PolTbl {
    Uint16     dispFlag;        /* 表示フラグ                                */
    Uint16     surfBright;      /* 面輝度                                    */
    Uint16     drawMode;        /* 描画モード                                */
    Uint16     color;           /* カラーデータ                              */
    Uint16     polyCnt;         /* ２Ｄポリゴン数                            */
    XyInt      xy[16];          /* ２Ｄポリゴン座標                          */
    SprGourTbl gourTbl;         /* グーローテーブル                          */
    Fixed32    z;               /* 描画ポリゴンＺ                            */
};
typedef struct DoubleBufArea  DoubleBufArea;
struct DoubleBufArea {
    SprCluster  *cluster;       /* クラスタ                                  */
    SprObject3D *object;        /* ３Ｄオブジェクト                          */
    Sint32      *surfBright;    /* 面輝度テーブル                            */
    Sint32      *vertBright;    /* 頂点輝度テーブル                          */
    MthXyz      *coordView3D;   /* ３Ｄ視点座標変換後テーブル                */
    MthXyz      *coordWorld3D;  /* ３Ｄワールド座標変換後テーブル            */
};

/*
 * GLOBAL DECLARATIONS
 */
extern Sint32      Sp2OpenFlag;             /* Open Command Exec Flag        */
extern SprCharTbl *SpCharTbl;               /* Character Table Pointer       */
extern Sint32      SpCharMax;               /* Character Table Entry Count   */
extern Sint32      SpDrawPrtyBlkMax;        /* Draw Priority Block Max Count */

/*
 * STATIC DECLARATIONS
 */
static MthMatrix    viewMatrix;
static MthMatrix    lightMatrix;
static MthMatrix    worldMatrix [WORLD_MATRIX_STACK_SIZE];
static MthMatrixTbl viewMatrixTbl;
static MthMatrixTbl lightMatrixTbl;
static MthMatrixTbl worldMatrixTbl;

static MthViewLight      viewLight;
static MthPolyTransParm  polyTransParm;

static DoubleBufArea dBuf[2];      /* 座標変換ダブルバッファエリア管理       */
static DoubleBufArea *cBuf;        /* カレント座標変換バッファ               */
static Sint32    doubleBufFlag;    /* ダブルバッファ有りフラグ               */
static Sint32    bufSW;            /* カレントダブルバッファＳＷ  0 or 1     */
static Sint32    dbFirstFlag;      /* ダブルバッファ操作ファーストフラグ     */
static Sint32    firstDrawFlag;    /* フラッシュ後最初のモデル描画フラグ     */
static MthXy     unitPixel;        /* X,Y 1.0 に対するスクリーンのピクセル数 */
static MthXyz    lightAngle;       /* 光源アングル                           */
static MthXyz	 worldLightVector; /* ワールド座標系での光源ベクトル         */
static MthXyz    viewCoordPoint;   /* 視点座標系内の視点位置                 */
static MthXyz    viewPoint;        /* 視点座標                               */
static MthXyz    viewAngle;        /* 視点アングル                           */
static Sint32	 viewAngleSeq;     /* 視点アングル回転操作                   */
static XyInt     *coord2D;         /* ２Ｄ透視変換後テーブル                 */
static Uint8     *coord2DFlag;     /* ２Ｄ透視変換処理フラグテーブル         */
static Sint32    gourTblNo;        /* フラッシュ単位グーローテーブルカウンタ */
static Fixed32   zSortZMinOrg;     /* フラッシュ単位元Ｚ座標最小値           */
static Fixed32   zSortZMaxOrg;     /* フラッシュ単位元Ｚ座標最大値           */
static Fixed32   zSortZMin;        /* フラッシュ単位現Ｚ座標最小値           */
static Fixed32   zSortZMax;        /* フラッシュ単位現Ｚ座標最大値           */
static Fixed32   zSortBZMin;       /* フラッシュ単位前Ｚ座標最小値           */
static Fixed32   zSortBZMax;       /* フラッシュ単位前Ｚ座標最大値           */
static Uint16    zSortBlkCnt;      /* Ｚソートブロック数                     */
static Sint32    zSortMode;        /* ＺソートＺ座標値の採用モード           */
static int       blkSftCnt;        /* Ｚソートブロックハッシュ用シフト数     */
static int       zSftCnt;          /* ＺソートＺシフト数                     */
static Sint16    clipScrXMin;      /* スクリーン境界Ｘ最小値座標             */
static Sint16    clipScrXMax;      /* スクリーン境界Ｘ最大値座標             */
static Sint16    clipScrYMin;      /* スクリーン境界Ｙ最小値座標             */
static Sint16    clipScrYMax;      /* スクリーン境界Ｙ最大値座標             */
static Fixed32   clipZMin;         /* 視点座標系クリップＺ最小値座標         */
static Fixed32   clipZMax;         /* 視点座標系クリップＺ最大値座標         */
static Sint32    clipLevel;        /* クリッピングレベル                     */
				   /*   ０＝クリッピングなし                 */
				   /*   １＝視点座標系のＺ範囲でクリップ     */
                                   /*       （座標変換後）                   */
                                   /*   ２＝スクリーンに掛からないポリゴンの */
				   /*       削除（透視変換後）               */
                                   /*   ３＝フレームバッファの境界でクリップ */
                                   /*       （透視変換後）                   */
static Uint16   postWorldDataFlag; /* ワールド座標通知要フラグ               */
static Sint32   spriteDrawFlag;    /* 変形スプライト描画フラグ               */
static Sint32   otherDrawFlag;     /* その他ポリゴン描画フラグ               */

static	MthXyz	orgLightVect =
	    { MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(-1.0)};


static void   initDraw(void);
static void   drawPol(PolTbl *polTbl);
static void   transCluster(SprCluster *cluster);
static void   setInbetPolygons(DoubleBufArea *buf);
static int    clip2DLevel2(PolTbl *polTbl);
static Sint32 clipScrCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt);
static Sint32 outcodeScr(XyInt *xy1, XyInt *xy2);
static int    clip2DLevel3(PolTbl *polTbl);
static Sint32 clipFrmCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt);
static Sint32 outcodeFrm(XyInt *xy1, XyInt *xy2);
static int    clipFrame(PolTbl *polTbl);
static int    frmCrossChk(int frmNo, XyInt *p1, XyInt *p2, XyInt *cp);
static void   transPolygons(DoubleBufArea *buf, XyInt *vert2d);
static void   viewCoordMatrix(void);
static void   setLightAngle(void);
extern Uint16 SPR_3GetShdColor(Uint16 baseRgbColor, Uint16 bright);



/*****************************************************************************
 *
 * NAME:  SPR_3Initial()  - Initial 3D Sprite Display Environment
 *
 * PARAMETERS :
 *
 * 　　(1) Spr3WorkArea *workArea - <i> ３Ｄスプライトワークエリア定義
 *
 * DESCRIPTION:
 *
 *     ３Ｄスプライト表示の初期化処理
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
SPR_3Initial(Spr3WorkArea  *workArea)
{
    MthXyz  w0,w1;

    /** BEGIN ***************************************************************/
    SPR_3SetDrawSpeed(0, 0, 0);

    dBuf[0].surfBright   = workArea->surfBright[0];
    dBuf[1].surfBright   = workArea->surfBright[1];
    dBuf[0].vertBright   = workArea->vertBright[0];
    dBuf[1].vertBright   = workArea->vertBright[1];
    dBuf[0].coordView3D  = workArea->coordView3D[0];
    dBuf[1].coordView3D  = workArea->coordView3D[1];
    dBuf[0].coordWorld3D = workArea->coordWorld3D[0];
    dBuf[1].coordWorld3D = workArea->coordWorld3D[1];
    if(workArea->surfBright[1])
        doubleBufFlag = 1;
    else
        doubleBufFlag = 0;
    if(workArea->coordWorld3D[0])
        postWorldDataFlag = 1;
    else
        postWorldDataFlag = 0;

    if(!dBuf[1].surfBright)   dBuf[1].surfBright   = dBuf[0].surfBright;
    if(!dBuf[1].vertBright)   dBuf[1].vertBright   = dBuf[0].vertBright;
    if(!dBuf[1].coordView3D)  dBuf[1].coordView3D  = dBuf[0].coordView3D;
    if(!dBuf[1].coordWorld3D) dBuf[1].coordWorld3D = dBuf[0].coordWorld3D;

    coord2D      = workArea->coord2D;
    coord2DFlag  = workArea->coord2DFlag;

    zSortBlkCnt = SpDrawPrtyBlkMax;
    for(blkSftCnt=0; blkSftCnt<16; blkSftCnt++)
       if((zSortBlkCnt<<=1) & 0x8000) break;
    zSortBlkCnt = SpDrawPrtyBlkMax;
    zSortZMin = ZSORT_MIN_Z;
    zSortZMax = ZSORT_MAX_Z;
    zSortMode = ZSORT_FLOAT_MODE;

    clipLevel = 2;
    clipZMin  = CLIP_MIN_Z;
    clipZMax  = CLIP_MAX_Z;

    w0.x = MTH_FIXED(0.0);   w0.y = MTH_FIXED(0.0);     w0.z = MTH_FIXED(0.0);
    SPR_3SetLight(1, &w0);
    w0.x = MTH_FIXED(0.0);   w0.y = MTH_FIXED(0.0);     w0.z = MTH_FIXED(0.0);
    w1.x = MTH_FIXED(0.0);   w1.y = MTH_FIXED(0.0);     w1.z = MTH_FIXED(0.0);
    SPR_3SetView(1, &w0, &w1, ROT_SEQ_XYZ, &w0);

    firstDrawFlag = 1;
    MTH_InitialMatrix(&worldMatrixTbl, WORLD_MATRIX_STACK_SIZE,	worldMatrix);
    MTH_PolyDataTransInit();
    SPR_3SetPixelCount(SCREEN_W_SIZE, SCREEN_H_SIZE);
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetLight()  - Set Light Angle
 *
 * PARAMETERS :
 *
 *     (1) Uint16      moveKind  - <i> 移動量の種別ーブルポインタ
 *                       　　　　　    0 = 相対移動、1 = 絶対移動タ
 *     (2) MthXyz *pLightAngler  - <i>  平行光源のアングル
 *
 * DESCRIPTION:
 *
 *     光源アングルセット
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
SPR_3SetLight(Uint16 moveKind, MthXyz *pLightAngle)
{
    Fixed32 angleX, angleY, angleZ;

    /** BEGIN ***************************************************************/
    if(moveKind & 0x0002) {
        angleX = MTH_Atan(pLightAngle->y, pLightAngle->z);
        angleY = MTH_Atan(pLightAngle->z, pLightAngle->x);
        angleZ = MTH_Atan(pLightAngle->y, pLightAngle->x);
    } else  {
        angleX = pLightAngle->x;
        angleY = pLightAngle->y;
        angleZ = pLightAngle->z;
    }
    if(moveKind & 0x0001) {
	lightAngle.x  = angleX;
	lightAngle.y  = angleY;
	lightAngle.z  = angleZ;
    } else       {
	lightAngle.x  += angleX;
	lightAngle.y  += angleY;
	lightAngle.z  += angleZ;
    }
    if(lightAngle.x > MTH_FIXED( 180)) lightAngle.x -= MTH_FIXED(360);  else
    if(lightAngle.x < MTH_FIXED(-180)) lightAngle.x += MTH_FIXED(360);
    if(lightAngle.y > MTH_FIXED( 180)) lightAngle.y -= MTH_FIXED(360);  else
    if(lightAngle.y < MTH_FIXED(-180)) lightAngle.y += MTH_FIXED(360);
    if(lightAngle.z > MTH_FIXED( 180)) lightAngle.z -= MTH_FIXED(360);  else
    if(lightAngle.z < MTH_FIXED(-180)) lightAngle.z += MTH_FIXED(360);
    setLightAngle();
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetView()  - Set View Point and Angle
 *
 * PARAMETERS :
 *
 *     (1) Uint16  moveKind         - <i> 移動量の種別ーブルポインタ
 *                       　　　　　       0 = 相対移動、1 = 絶対移動タ
 *     (2) MthXyz *pViewPoint       - <i> 視点座標
 *     (3) MthXyz *pViewAngle       - <i> 視点のアングル（度）
 *     (4) Uint16  pViewAngleSeq    - <i> アングルの回転操作
 *　　　　　　　　　　　    　            ROT_SEQ_ZYX
 *　　　　　　　　　　　    　            ROT_SEQ_ZXY
 *　　　　　　　　　　　    　            ROT_SEQ_YZX
 *　　　　　　　　　　　    　            ROT_SEQ_YXZ
 *　　　　　　　　　　　    　            ROT_SEQ_XYZ
 *　　　　　　　　　　　    　            ROT_SEQ_XZY
 *     (5) MthXyz *pViewCoordPoint  - <i> 視点座標系内の視点位置
 *
 * DESCRIPTION:
 *
 *     視点のセット
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
SPR_3SetView(Uint16 moveKind, MthXyz *pViewPoint, MthXyz *pViewAngle,
                              Uint16 pViewAngleSeq, MthXyz *pViewCoordPoint)
{
    Fixed32 angleX, angleY, angleZ;

    /** BEGIN ***************************************************************/
    viewAngleSeq = pViewAngleSeq;

    if(pViewCoordPoint) {
        if(moveKind & 0x0001) {
	    viewCoordPoint.x  = pViewCoordPoint->x;
	    viewCoordPoint.y  = pViewCoordPoint->y;
	    viewCoordPoint.z  = pViewCoordPoint->z;
        } else       {
	    viewCoordPoint.x += pViewCoordPoint->x;
	    viewCoordPoint.y += pViewCoordPoint->y;
	    viewCoordPoint.z += pViewCoordPoint->z;
        }
    }

    if(pViewPoint) {
        if(moveKind & 0x0001) {
	    viewPoint.x  = pViewPoint->x;
	    viewPoint.y  = pViewPoint->y;
	    viewPoint.z  = pViewPoint->z;
        } else       {
	    viewPoint.x += pViewPoint->x;
	    viewPoint.y += pViewPoint->y;
	    viewPoint.z += pViewPoint->z;
        }
    }

    if(pViewAngle) {
        if(moveKind & 0x0002) {
            angleX = MTH_Atan(pViewAngle->y, pViewAngle->z);
            angleY = MTH_Atan(pViewAngle->z, pViewAngle->x);
            angleZ = MTH_Atan(pViewAngle->y, pViewAngle->x);
        } else  {
            angleX = pViewAngle->x;
            angleY = pViewAngle->y;
            angleZ = pViewAngle->z;
        }
        if(moveKind & 0x0001) {
	    viewAngle.x  = angleX;
	    viewAngle.y  = angleY;
	    viewAngle.z  = angleZ;
        } else       {
	    viewAngle.x += angleX;
	    viewAngle.y += angleY;
	    viewAngle.z += angleZ;
        }
        if(viewAngle.x > MTH_FIXED( 180)) viewAngle.x -= MTH_FIXED(360);  else
        if(viewAngle.x < MTH_FIXED(-180)) viewAngle.x += MTH_FIXED(360);
        if(viewAngle.y > MTH_FIXED( 180)) viewAngle.y -= MTH_FIXED(360);  else
        if(viewAngle.y < MTH_FIXED(-180)) viewAngle.y += MTH_FIXED(360);
        if(viewAngle.z > MTH_FIXED( 180)) viewAngle.z -= MTH_FIXED(360);  else
        if(viewAngle.z < MTH_FIXED(-180)) viewAngle.z += MTH_FIXED(360);
    }

}


/*****************************************************************************
 *
 * NAME:  SPR_3CallAllCluster()  - Call User Function with all Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *cluster           - <i> ルートクラスタテーブルのポインタ
 *     (2) void (*userFunc)(SprCluster*) - <i> ユーザファンクション
 *
 * DESCRIPTION:
 *
 *     ルートクラスタにつながる全クラスタをパラメータとして指定ファンクション
 *     を呼び出す
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void   SPR_3CallAllCluster(SprCluster *cluster, void (*userFunc)(SprCluster*))
{
    /** BEGIN ***************************************************************/
    while(cluster) {
        userFunc(cluster);
	/* 子クラスタの処理 */
	if(cluster->child) SPR_3CallAllCluster(cluster->child, userFunc);
        cluster = cluster->next;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3MoveCluster()  - Move Specify Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *cluster   - <i> 移動すべきクラスタテーブルのポインタ
 *     (2) Uint16      moveKind  - <i> 移動量の種別ーブルポインタ
 *                       　　　　　    0 = 相対移動、1 = 絶対移動タ
 *     (3) MthXyz     *angle     - <i> 回転移動量（度）
 *     (4) MthXyz     *point     - <i> 平行移動量テーブルポインタ
 *
 * DESCRIPTION:
 *
 *     クラスタの移動
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
SPR_3MoveCluster(SprCluster *cluster, Uint16 moveKind,
				      MthXyz *angle, MthXyz *point)
{
    Fixed32 angleX, angleY, angleZ;

    /** BEGIN ***************************************************************/
    if(angle) {
        if(moveKind & 0x0002) {
            angleX = MTH_Atan(angle->y, angle->z);
            angleY = MTH_Atan(angle->z, angle->x);
            angleZ = MTH_Atan(angle->y, angle->x);
        } else  {
	    angleX = angle->x;
	    angleY = angle->y;
	    angleZ = angle->z;
        }
        if(moveKind & 0x0001) {
   	    cluster->angle.x  = angleX;
	    cluster->angle.y  = angleY;
	    cluster->angle.z  = angleZ;
        } else       {
	    cluster->angle.x += angleX;
	    cluster->angle.y += angleY;
	    cluster->angle.z += angleZ;
        }
        if(cluster->angle.x > MTH_FIXED( 180))
            cluster->angle.x -= MTH_FIXED(360);
        else
        if(cluster->angle.x < MTH_FIXED(-180))
            cluster->angle.x += MTH_FIXED(360);
        if(cluster->angle.y > MTH_FIXED( 180))
            cluster->angle.y -= MTH_FIXED(360);
        else
        if(cluster->angle.y < MTH_FIXED(-180))
            cluster->angle.y += MTH_FIXED(360);
        if(cluster->angle.z > MTH_FIXED( 180))
            cluster->angle.z -= MTH_FIXED(360);
        else
        if(cluster->angle.z < MTH_FIXED(-180))
            cluster->angle.z += MTH_FIXED(360);
    }
    if(point)
        if(moveKind & 0x0001) {
	    cluster->point.x  = point->x;
	    cluster->point.y  = point->y;
	    cluster->point.z  = point->z;
        } else       {
	    cluster->point.x += point->x;
	    cluster->point.y += point->y;
	    cluster->point.z += point->z;
        }
}


/*****************************************************************************
 *
 * NAME:  SPR_3DrawModel()  - Set Specify Model
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *rootCluster  - <i>  表示すべきモデルのルートクラスタ
 *                                         テーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     モデルの登録
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
SPR_3DrawModel(SprCluster *rootCluster)
{
    /** BEGIN ***************************************************************/
    if(firstDrawFlag) {
	firstDrawFlag = 0;
	viewCoordMatrix();
	initDraw();
    }

    transCluster(rootCluster);
}


/*****************************************************************************
 *
 * NAME:  SPR_3Flush()  - Draw End All Seted Cluster
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     登録された全モデルの描画終了処理
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
SPR_3Flush(void)
{
    /** BEGIN ***************************************************************/
    if(doubleBufFlag) {
	cBuf = &dBuf[bufSW^1];
	transPolygons(cBuf, coord2D);
    }
    SPR_2FlushDrawPrty();
    firstDrawFlag = 1;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetTexture()  - Set Texture Data
 *
 * PARAMETERS :
 *
 *     (1) SprTexture *texture  - <i>  テクスチャテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     テクスチャのセット
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
SPR_3SetTexture(SprTexture *texture)
{
    SprTexture   *wTex;

    /** BEGIN ***************************************************************/
    for(wTex = texture; wTex->charNo != 0xffff; wTex++) {
	SPR_2SetChar(wTex->charNo, wTex->colorMode, wTex->color,
                     wTex->width, wTex->height, wTex->charData);
	if(wTex->lookupTbl)
	    SPR_2SetLookupTbl(wTex->color, wTex->lookupTbl);
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3ChangeTexColor()  - Change Texture Color
 *
 * PARAMETERS :
 *
 *     (1) Uint16      charNo    - <i>  キャラクタ番号
 *     (2) Uint16      color     - <i>  カラーデータ（ｶﾗｰﾊﾞﾝｸorﾙｯｸｱｯﾌﾟﾃｰﾌﾞﾙNo）
 *     (2) SprLookupTbl *lookupTbl   - <i>  ルックアップテーブルポインタ
 *
 * DESCRIPTION:
 *
 *     テクスチャカラーデータの変更
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
SPR_3ChangeTexColor(Uint16 charNo, Uint16 color, SprLookupTbl *lookupTbl)
{
    /** BEGIN ***************************************************************/
    SpCharTbl[charNo].color = color;
    if(lookupTbl)
        SPR_2SetLookupTbl(color, lookupTbl);
}


/*****************************************************************************
 *
 * NAME:  SPR_3ClrTexture()  - Clear Texture Data
 *
 * PARAMETERS :
 *
 *     (1) SprTexture *texture  - <i>  テクスチャテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     テクスチャエリアの解放
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
SPR_3ClrTexture(SprTexture *texture)
{
    SprTexture  *wTex;

    /** BEGIN ***************************************************************/
    for(wTex = texture; wTex->charNo != 0xffff; wTex++)
        SPR_2ClrChar(wTex->charNo);
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetZSortMinMax()  - Set Z Sort Min/Max Z Value
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pZSortMode  - <i>  ＺソートＺ座標値の採用モード
 *                                     ０＝浮動モード
 *                                     １＝固定モード
 *     (2) Fixed32  pZSortMin   - <i>  視点座標系ＺソートＺ最小値座標
 *     (3) Fixed32  pZSortMax   - <i>  視点座標系ＺソートＺ最大値座標
 *
 *
 * DESCRIPTION:
 *
 *     視点座標系Ｚソート範囲の最小・最大値をセット
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
SPR_3SetZSortMinMax(Uint16 pZSortMode, Fixed32 pZSortMin, Fixed32 pZSortMax)
{

    /** BEGIN ***************************************************************/
    zSortMode    = pZSortMode;
    zSortZMin    = pZSortMin;
    zSortZMax    = pZSortMax;
    zSortZMinOrg = pZSortMin;
    zSortZMaxOrg = pZSortMax;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetClipLevel()  - Set Cliping Level
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pClipLevel  - <i>  クリッピングのレベル番号
 *                                     ０＝クリッピングなし
 *                                     １＝視点座標系のＺ範囲でクリップ
 *                                         （座標変換後）
 *                                     ２＝スクリーンに掛からないポリゴンの
 *                                         削除（透視変換後）
 *                                     ３＝フレームバッファの境界でクリップ
 *                                         （透視変換後）
 *     (2) Fixed32  pClipZMin   - <i>  視点座標系クリップＺ最小値座標
 *     (3) Fixed32  pClipZMax   - <i>  視点座標系クリップＺ最大値座標
 *
 * DESCRIPTION:
 *
 *     クリッピングレベルのセット
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
SPR_3SetClipLevel(Uint16 pClipLevel, Fixed32 pClipZMin, Fixed32 pClipZMax)
{

    /** BEGIN ***************************************************************/
    clipLevel = pClipLevel;
    clipZMin  = pClipZMin;
    clipZMax  = pClipZMax;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetPixelCount()  - Set Screen Pixel Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pixelCountX  - <i>  スクリーンＸの単位ピクセル数
 *     (2) Uint16   pixelCountY  - <i>  スクリーンＹの単位ピクセル数
 *
 * DESCRIPTION:
 *
 *     スクリーンへの透視変換時のＸＹ各々 1.0 に対するスクリーンのピクセル数
 *     を設定する。
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void SPR_3SetPixelCount(Uint16 pixelCountX, Uint16 pixelCountY)
{

    /** BEGIN ***************************************************************/
    unitPixel.x = MTH_IntToFixed(pixelCountX);
    unitPixel.y = MTH_IntToFixed(pixelCountY);
}


/*****************************************************************************
 *
 * NAME:  SPR_3GetStatus()  - Get 3D Environment Value
 *
 * PARAMETERS :
 *
 *     (1) Spr3dStatus *spr3dStatus  - <o>  ３Ｄ環境データテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     現３Ｄ環境データの取得。
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
void   SPR_3GetStatus(Spr3dStatus *spr3dStatus)
{
    /** BEGIN ***************************************************************/
    spr3dStatus->lightAngle      = lightAngle;
    spr3dStatus->viewCoordPoint  = viewCoordPoint;
    spr3dStatus->viewPoint       = viewPoint;
    spr3dStatus->viewAngle       = viewAngle;
    spr3dStatus->viewAngleSeq    = viewAngleSeq;
    spr3dStatus->zSortMode       = zSortMode;
    spr3dStatus->zSortZMin       = zSortZMin;
    spr3dStatus->zSortZMax       = zSortZMax;
    spr3dStatus->clipZMin        = clipZMin;
    spr3dStatus->clipZMax        = clipZMax;
    spr3dStatus->clipLevel       = clipLevel;
    spr3dStatus->unitPixel       = unitPixel;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetSurfNormVect() - Compute Surface Normal Vector in the Object
 *
 * PARAMETERS :
 *
 *     (1)  SprObject3D  *obj  - <i/o>  ３Ｄオブジェクトのポインタ
 *
 * DESCRIPTION:
 *
 *     指定３Ｄオブジェクトの面の法線を計算し、セットする。
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
void   SPR_3SetSurfNormVect(SprObject3D  *obj)
{
    Sint32      i, p1, p2, p3;
    MthXyz      *wVertPoint;
    SprSurface  *wSurface;
    MthXyz      *wSurfNormal;

    /** BEGIN ***************************************************************/
    wSurface    = obj->surface;
    wVertPoint  = obj->vertPoint;
    wSurfNormal = obj->surfaceNormal;
    for(i = 0; i < obj->surfaceCount; i++) {
	p1 = wSurface->vertNo[0];
	p2 = wSurface->vertNo[1];
	p3 = wSurface->vertNo[2];
	MTH_ComputeNormVect(obj->surfNormK, &wVertPoint[p1], &wVertPoint[p2],
			                    &wVertPoint[p3], wSurfNormal);
	wSurface++;
	wSurfNormal++;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetDrawSpeed() - Set VDP1 Drawing Speed up Parameter
 *
 * PARAMETERS :
 *
 *     (1)  Sint32    hssFlag  - <i>  ハイスピードシュリンクフラグ
 *                                    テクスチャポリゴンの描画モード
 *                                    ０＝精度優先描画（デフォルト）
 *                                    １＝速度優先描画
 *     (2)  Sint32    eosFlag  - <i>  hss=1 で速度優先描画を指定した時の
 *                                    元絵テクスチャのサンプリング座標
 *                                    ０＝偶数座標をサンプリング（デフォルト）
 *                                    １＝奇数座標をサンプリング
 *     (3)  Sint32    pclpFlag - <i>  プリクリッピング有効・無効フラグ
 *                                    ０＝有効（デフォルト）
 *                                    １＝無効
 *
 *
 * DESCRIPTION:
 *
 *     ＶＤＰ１の高速描画パラメータをセット
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
void   SPR_3SetDrawSpeed(Sint32 hssFlag, Sint32 eosFlag, Sint32 pclpFlag)
{
    Sint32      i, p1, p2, p3;
    MthXyz      *wVertPoint;
    SprSurface  *wSurface;
    MthXyz      *wSurfNormal;

    /** BEGIN ***************************************************************/
    if(hssFlag)
       spriteDrawFlag  = HSS_ENABLE;
    else
       spriteDrawFlag  = 0;

    if(pclpFlag) {
       spriteDrawFlag |= PCLP_ENABLE;
       otherDrawFlag   = PCLP_ENABLE;
    } else {
       spriteDrawFlag &= ~PCLP_ENABLE;
       otherDrawFlag   = 0;
    }

    SPR_SetEosMode(eosFlag);
}


/*****************************************************************************
 *
 * NAME:  initDraw()  - Initial Draw Environment
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ３Ｄ表示のテーブル初期化
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
static void
initDraw(void)
{
    Fixed32   ztMax;
    Sint32    i;

    /** BEGIN ***************************************************************/
    if(zSortMode) {
        zSortBZMin    = zSortZMinOrg;
        zSortBZMax    = zSortZMaxOrg;
    } else        {
        zSortBZMin    = zSortZMin;
        zSortBZMax    = zSortZMax;
    }
    zSortZMin     = FIXED_MAX;
    zSortZMax     = FIXED_MIN;
    ztMax = zSortBZMax - zSortBZMin;
    for(i=0; i<32; i++)
	if((ztMax<<=1) & 0x80000000) break;
    zSftCnt = 17 - i + blkSftCnt;
    if(zSftCnt < 0) zSftCnt = 0;
    dbFirstFlag = 1;
    bufSW       = 0;
    if(Sp2OpenFlag) {
        gourTblNo   = 0;
        clipScrXMin = -SpLCoordX;
        clipScrXMax = SpScreenX - SpLCoordX - 1;
        clipScrYMin = -SpLCoordY;
        clipScrYMax = SpScreenY - SpLCoordY - 1;
        Sp2OpenFlag = 0;
    }
}


/*****************************************************************************
 *
 * NAME:  drawPol()  - Draw 1 Polygon
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl          - <i>  ポリゴンテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     ポリゴンの描画
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
drawPol(PolTbl *polTbl)
{
    Sint32     drawPrtyBlkNo;
    Sint32     shadingKind, dispKind, drawMode, colOrTex, textCtrl;
    Sint32     surfBright;
    Sint32     mateKind, i;
    XyInt      *xy;
    SprGourTbl *gourTbl;
    Sint32     z;

    /** BEGIN ***************************************************************/
    dispKind    = polTbl->dispFlag & DISP_MASK;
    shadingKind = polTbl->dispFlag & SHADING_MASK;
    mateKind    = polTbl->drawMode & MATE_MASK;
    drawMode    = polTbl->drawMode & DRAW_MODE_MASK;
    surfBright  = polTbl->surfBright;
    gourTbl     = &polTbl->gourTbl;
    colOrTex    = polTbl->color;
    xy          = polTbl->xy;

    if(mateKind == MATE_TEXTURE) {
        textCtrl   = colOrTex & 0xc000;
        textCtrl >>= 10;
        colOrTex  &= 0x3fff;
    }

    /* Ｚ値ブロックソートの最大最小値を得る */
    if(polTbl->z < zSortZMin) zSortZMin = polTbl->z;
    if(polTbl->z > zSortZMax) zSortZMax = polTbl->z;
    if(polTbl->z > zSortBZMin) {
	drawPrtyBlkNo = (int)((polTbl->z - zSortBZMin) >> zSftCnt);
	if(drawPrtyBlkNo >= zSortBlkCnt)
	    drawPrtyBlkNo = zSortBlkCnt - 1;
    } else
	drawPrtyBlkNo = 0;

    for(i=0; i<polTbl->polyCnt; i++) {
	if(shadingKind == NO_SHADING) {
            if(mateKind == MATE_TEXTURE) {
		SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                                  drawMode | spriteDrawFlag,
		   	          0xffff, colOrTex, xy, NO_GOUR);
            } else {
    	        if(dispKind == DISP_POLYGON) {
	            SPR_2Polygon(drawPrtyBlkNo,
	                      drawMode | otherDrawFlag, colOrTex, xy, NO_GOUR);
                } else {
	            SPR_2PolyLine(drawPrtyBlkNo,
	                      drawMode | otherDrawFlag, colOrTex, xy, NO_GOUR);
                }
            }
        } else
	if(shadingKind == FLAT_SHADING) {  /* Color Mode is Only RGB */
            if(mateKind == MATE_TEXTURE) {
	        SPR_2SetGourTbl(gourTblNo, gourTbl);
                SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                             drawMode | DRAW_GOURAU  | spriteDrawFlag,
                              0xffff, colOrTex, xy, gourTblNo);
		gourTblNo++;
            } else {
                if(dispKind == DISP_POLYGON) {
		    SPR_2Polygon(drawPrtyBlkNo, drawMode | otherDrawFlag,
                                 surfBright, xy, NO_GOUR);
                } else {
                    SPR_2PolyLine(drawPrtyBlkNo, drawMode | otherDrawFlag,
                                  surfBright, xy, NO_GOUR);
	        }
	    }
        } else
        if(shadingKind == GOURAUD_SHADING) { /* Color Mode is Only RGB */
            SPR_2SetGourTbl(gourTblNo, gourTbl);
            if(mateKind == MATE_TEXTURE) {
                SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                                 drawMode | DRAW_GOURAU | spriteDrawFlag,
		   	                0xffff, colOrTex, xy, gourTblNo);
            } else     {
	        if(dispKind == DISP_POLYGON) {
                    SPR_2Polygon(drawPrtyBlkNo,
                                 drawMode | DRAW_GOURAU | otherDrawFlag,
						colOrTex, xy, gourTblNo);
                } else {
		    SPR_2PolyLine(drawPrtyBlkNo,
                                  drawMode | DRAW_GOURAU | otherDrawFlag,
				          	colOrTex, xy, gourTblNo);
                }
            }
	    gourTblNo++;
        }
        xy += 4;
    }
}


/*****************************************************************************
 *
 * NAME:  transCluster()  - Transfer Cluster Coord
 *
 * PARAMETERS :
 *
 *     (1) SprCluster  *cluster  - <i>  ルートクラスタのポインタ
 *
 * DESCRIPTION:
 *
 *     クラスタ単位の隠面、座標変換処理
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
static void
transCluster(SprCluster *cluster)
{
    SprObject3D  *obj;
    SprSurface   *wSurface;
    MthXyz       *wVertPoint, *wSurfaceVert;
    Fixed32      *wSurfPoint, *wVPoint;
    Uint8        *wSurfBright;
    Uint16       *wVertNo;
    Sint32 	 i, j;
    const static MthMatrix	unitMatrix
	= { MTH_FIXED(1.0), MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(0.0),
	    MTH_FIXED(0.0), MTH_FIXED(1.0), MTH_FIXED(0.0), MTH_FIXED(0.0),
	    MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(1.0), MTH_FIXED(0.0)};

    /** BEGIN ***************************************************************/
    while(cluster) {
        if(cluster->transStart)  cluster->transStart(cluster);

        MTH_PushMatrix(&worldMatrixTbl);

	/* カレントマトリックスにクラスタの移動量マトリックスセット */
	MTH_MoveMatrix(&worldMatrixTbl,
                       cluster->point.x, cluster->point.y, cluster->point.z);
	switch(cluster->angleSeq) {
	    case ROT_SEQ_ZYX :
		 MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
	         MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
	         MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
                 break;
            case ROT_SEQ_ZXY :
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
	         MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
	         MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 break;
            case ROT_SEQ_YZX :
	         MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
                 break;
            case ROT_SEQ_YXZ :
	         MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
	         MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
		 break;
	    case ROT_SEQ_XYZ :
		 MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
		 MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 break;
	    case ROT_SEQ_XZY :
		 MTH_RotateMatrixY(&worldMatrixTbl, cluster->angle.y);
		 MTH_RotateMatrixZ(&worldMatrixTbl, cluster->angle.z);
		 MTH_RotateMatrixX(&worldMatrixTbl, cluster->angle.x);
		 break;
	}

	obj = cluster->object;
	if(obj) {
	    if(postWorldDataFlag) {
	        MTH_MulMatrix(&viewMatrix, worldMatrixTbl.current,
						     &viewLight.viewMatrix);
	        memcpy(&viewLight.worldMatrix, worldMatrixTbl.current,
						     sizeof(MthMatrix));
	    } else {
	        memcpy(&viewLight.viewMatrix, worldMatrixTbl.current,
						     sizeof(MthMatrix));
	    }
	}

	while(obj) {
            cBuf = &dBuf[bufSW];
            cBuf->cluster = cluster;
            cBuf->object  = obj;
            if(obj->dispFlag & INBETWEEN_OBJECT) {
		memcpy(&viewLight.viewMatrix, &unitMatrix,sizeof(MthMatrix));
		SPR_3SetSurfNormVect(obj);
            }

	    if(obj->surfaceVert == 0) {
		/* 面の代表頂点テーブルを得る */
		wSurfPoint  = (Fixed32*)cBuf->coordView3D;
		wSurface    = obj->surface;
		wVertPoint  = obj->vertPoint;

		for(i = 0; i < obj->surfaceCount; i++) {
		    j = wSurface->vertNo[0];
		    wVPoint = (Fixed32*)&wVertPoint[j];
		    *wSurfPoint++ = *wVPoint++;
		    *wSurfPoint++ = *wVPoint++;
		    *wSurfPoint++ = *wVPoint;
		    wSurface++;
		}
		wSurfaceVert = cBuf->coordView3D;
	    } else
		wSurfaceVert = obj->surfaceVert;

	    /* 面法線テーブルから面輝度テーブルを得るためのパラメータセット */
	    polyTransParm.viewLight = &viewLight;
	    polyTransParm.surfCount = obj->surfaceCount;
	    polyTransParm.surfPoint = wSurfaceVert;
	    polyTransParm.surfNormal = obj->surfaceNormal;
	    polyTransParm.surfBright = cBuf->surfBright;

	    /* 頂点データの視点座標変換後テーブルを得るためのパラメータセット*/
	    polyTransParm.transViewVertCount = obj->vertCount;
	    polyTransParm.transViewVertSrc = obj->vertPoint;
	    polyTransParm.transViewVertAns = cBuf->coordView3D;

	    /* グーロー用頂点輝度テーブルを得るためのパラメータセット  */
	    if((obj->dispFlag & SHADING_MASK) == GOURAUD_SHADING) {
                if(obj->dispFlag & INBETWEEN_OBJECT) {
  	            polyTransParm.gourVertCount = 0;
  	            memcpy(cBuf->vertBright,obj->vertNormal,
  	                                  obj->vertCount*sizeof(Sint32));
                } else {
	            polyTransParm.gourVertCount = obj->vertCount;
	            polyTransParm.vertNormal    = obj->vertNormal;
		    polyTransParm.vertBright    = cBuf->vertBright;
		}
            } else
	        polyTransParm.gourVertCount = 0;

	    /* 頂点データのワールド座標変換後テーブルを得るための
                                                        パラメータセット*/
	    if(postWorldDataFlag) {
	        polyTransParm.transWorldVertCount = obj->vertCount;
	        polyTransParm.transWorldVertSrc   = obj->vertPoint;
	        polyTransParm.transWorldVertAns   = cBuf->coordWorld3D;
            } else
	        polyTransParm.transWorldVertCount = 0;

	    MTH_PolyDataTransExec(&polyTransParm);

            if(dbFirstFlag) {
		MTH_PolyDataTransCheck();
                if(cluster->transEnd)
                    cluster->transEnd(cluster, obj, worldMatrixTbl.current,
                                                         cBuf->coordWorld3D);
            } else {
		/* ポリゴン描画コマンドの登録処理 */
                cBuf = &dBuf[bufSW^1];
		transPolygons(cBuf, coord2D);
		setInbetPolygons(cBuf);
		MTH_PolyDataTransCheck();
                if(cluster->transEnd) {
		    cBuf = &dBuf[bufSW];
                    cluster->transEnd(cluster, obj, worldMatrixTbl.current,
                                                         cBuf->coordWorld3D);
                }
            }

	    if(doubleBufFlag) {
		dbFirstFlag = 0;
                bufSW ^= 1;
	    } else {
                cBuf = &dBuf[bufSW];
		transPolygons(cBuf, coord2D);
                setInbetPolygons(cBuf);
            }

            obj = obj->next;

	}

	/* 子クラスタの処理 */
	if(cluster->child) transCluster(cluster->child);

	MTH_PopMatrix(&worldMatrixTbl);
        cluster = cluster->next;
    }
}


/*****************************************************************************
 *
 * NAME:  setInbetPolygons  - Set Inbetween Polygon Data to Inbet Object
 *
 * PARAMETERS :
 *
 *     (1) DoubleBufArea  *buf    - <i>  カレント座標変換バッファ
 *
 * DESCRIPTION:
 *
 *     ３Ｄオブジェクト間接続ポリゴン視点座標系頂点データの登録処理
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
static void
setInbetPolygons(DoubleBufArea *buf)
{
    Sint32       i;
    SprObject3D  *inbetObj;
    SprInbetInf  *wInbetInf;
    MthXyz       *fromVertPoint;
    Sint32       *fromVertBright;
    Uint16       *fromVertNo;
    MthXyz       *toVertPoint;
    Sint32       *toVertBright;
    Uint16       *toVertNo;
    SprCluster   *cluster;
    SprObject3D  *obj;

    /** BEGIN ***************************************************************/
    cluster = buf->cluster;
    obj     = buf->object;

    /*  ３Ｄオブジェクト間接続ポリゴン情報がある場合     */
    if(cluster->inbetInf) {
	 wInbetInf = cluster->inbetInf;
	 while(wInbetInf) {
	     if(obj == wInbetInf->fromObj) {
		 fromVertPoint = cBuf->coordView3D;
		 fromVertNo    = wInbetInf->fromVertNo;
		 toVertPoint   = wInbetInf->toObj->vertPoint;
		 toVertNo      = wInbetInf->toVertNo;
		 for(i=0 ; i<wInbetInf->vertCount; i++)
		     toVertPoint[toVertNo[i]] = fromVertPoint[fromVertNo[i]];
		 toVertBright  = (Sint32*)wInbetInf->toObj->vertNormal;
		 if(toVertBright) {
		     fromVertBright = cBuf->vertBright;
		     for(i=0 ; i<wInbetInf->vertCount; i++)
		         toVertBright[toVertNo[i]] =
		                              fromVertBright[fromVertNo[i]];
		 }
                 break;
             }
             wInbetInf = wInbetInf->next;
         }
    }
}


/*****************************************************************************
 *
 * NAME:  clip2DLevel2()  - Clip 2D Level 2
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl  - <i>  ２Ｄポリゴンテーブル
 *
 * DESCRIPTION:
 *
 *     ポリゴンの２Ｄクリッピング処理
 *     スクリーンに掛からないポリゴンの削除
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  リターンコード
 *                                 １＝描画しない
 *                                 ０＝描画する
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
clip2DLevel2(PolTbl *polTbl)
{
    Uint32   f1,f2;
    XyInt    *xy,*xyb;
    Sint32   i;

    /** BEGIN ***************************************************************/
    f1 = 0;
    xy = polTbl->xy;
    if(xy->x < clipScrXMin) f1 |= 8;
    if(xy->x > clipScrXMax) f1 |= 4;
    if(xy->y < clipScrYMin) f1 |= 2;
    if(xy->y > clipScrYMax) f1 |= 1;
    for(i=0; i<4; i++) {
	xyb = xy;
	if(i == 3)
	    xy -= 3;
	else
	    xy++;
	f2 = 0;
	if(xy->x < clipScrXMin) f2 |= 8;
	if(xy->x > clipScrXMax) f2 |= 4;
	if(xy->y < clipScrYMin) f2 |= 2;
	if(xy->y > clipScrYMax) f2 |= 1;
	if(!(f1 & f2)) {
	    if(!(f1 | f2)) return 0;
	    if(!clipScrCheck(xyb,xy,8)) return 0;
	}
	f1 = f2;
    }
    return 1;
}

/*****************************************************************************
 *
 * NAME:  clipScrCheck()  - Clip Screen Detail Check
 *
 * PARAMETERS :
 *
 *     (1) XyInt     *xy1   - <i>  ライン始点
 *     (2) XyInt     *xy2   - <i>  ライン終点
 *     (2) Sint32    chkCnt - <i>  チェックカウンタ
 *
 * DESCRIPTION:
 *
 *     ラインの中点がスクリーン内に入っているか２分割法によりリカーシブに
 *     チェックする。
 *
 *
 * POSTCONDITIONS:
 *
 *     Sint32   ret         - <o>  リターンコード
 *                                 ０＝スクリーン内に入っている
 *                                 １＝入っていない
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static Sint32
clipScrCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt)
{
   Sint32       ret;
   XyInt     xym;

   if(chkCnt <= 1) return 1;
   xym.x = (xy1->x + xy2->x) >> 1;
   if(xym.x & 0x4000) xym.x |= 0x8000;
   xym.y = (xy1->y + xy2->y) >> 1;
   if(xym.y & 0x4000) xym.y |= 0x8000;
   if((clipScrXMin <= xym.x) && (xym.x <= clipScrXMax) &&
      (clipScrYMin <= xym.y) && (xym.y <= clipScrYMax)) return 0;
   switch(outcodeScr(xy1, &xym)) {
      case -1 : /* just in */
          return 0;
      case  0 : /* nead detail check */
          ret = clipScrCheck(xy1, &xym, chkCnt-1);
          break;
      case  1 : /* just out */
	  ret = clipScrCheck(&xym, xy2, chkCnt-1);
	  break;
   }
   return ret;
}

static
Sint32 outcodeScr(XyInt *xy1, XyInt *xy2)
{
   Uint32 f1, f2;

   f1 = f2 = 0;
   if(xy1->x < clipScrXMin) f1 |= 8;
   if(xy1->x > clipScrXMax) f1 |= 4;
   if(xy1->y < clipScrYMin) f1 |= 2;
   if(xy1->y > clipScrYMax) f1 |= 1;
   if(xy2->x < clipScrXMin) f2 |= 8;
   if(xy2->x > clipScrXMax) f2 |= 4;
   if(xy2->y < clipScrYMin) f2 |= 2;
   if(xy2->y > clipScrYMax) f2 |= 1;
   if(f1 & f2)
       return  1; /* just out          */
   else
   if(f1 | f2)
       return  0; /* need detail check */
   else
       return -1; /* just in           */
}


/*****************************************************************************
 *
 * NAME:  clipFrmCheck()  - Clip Frame Buffer Detail Check
 *
 * PARAMETERS :
 *
 *     (1) XyInt  *xy1   - <i>  ライン始点
 *     (2) XyInt  *xy2   - <i>  ライン終点
 *     (2) Sint32    chkCnt - <i>  チェックカウンタ
 *
 * DESCRIPTION:
 *
 *     ラインの中点がフレームバッファ内に入っているか２分割法によりリカーシブに
 *     チェックする。
 *
 *
 * POSTCONDITIONS:
 *
 *     Sint32   ret         - <o>  リターンコード
 *                                 ０＝スクリーン内に入っている
 *                                 １＝入っていない
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static Sint32
clipFrmCheck(XyInt *xy1, XyInt *xy2, Sint32 chkCnt)
{
   Sint32       ret;
   XyInt     xym;

   if(chkCnt <= 1) return 1;
   xym.x = (xy1->x + xy2->x) >> 1;
   if(xym.x & 0x4000) xym.x |= 0x8000;
   xym.y = (xy1->y + xy2->y) >> 1;
   if(xym.y & 0x4000) xym.y |= 0x8000;
   if((FRMBUF_MIN_X <= xym.x) && (xym.x <= FRMBUF_MAX_X) &&
      (FRMBUF_MIN_Y <= xym.y) && (xym.y <= FRMBUF_MAX_Y)) return 0;
   switch(outcodeFrm(xy1, &xym)) {
      case -1 : /* just in */
          return 0;
      case  0 : /* nead detail check */
          ret = clipFrmCheck(xy1, &xym, chkCnt-1);
	  break;
      case  1 : /* just out */
          ret = clipFrmCheck(&xym, xy2, chkCnt-1);
          break;
   }
   return ret;
}

static
Sint32 outcodeFrm(XyInt *xy1, XyInt *xy2)
{
   Uint32 f1, f2;

   f1 = f2 = 0;
   if(xy1->x < FRMBUF_MIN_X) f1 |= 8;
   if(xy1->x > FRMBUF_MAX_X) f1 |= 4;
   if(xy1->y < FRMBUF_MIN_Y) f1 |= 2;
   if(xy1->y > FRMBUF_MAX_Y) f1 |= 1;
   if(xy2->x < FRMBUF_MIN_X) f2 |= 8;
   if(xy2->x > FRMBUF_MAX_X) f2 |= 4;
   if(xy2->y < FRMBUF_MIN_Y) f2 |= 2;
   if(xy2->y > FRMBUF_MAX_Y) f2 |= 1;
   if(f1 & f2)
       return  1; /* just out          */
   else
   if(f1 | f2)
       return  0; /* need detail check */
   else
       return -1; /* just in           */
}


/*****************************************************************************
 *
 * NAME:  clip2DLevel3()  - Clip 2D Level 3
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl  - <i>  ２Ｄポリゴンテーブル
 *
 * DESCRIPTION:
 *
 *     ポリゴンの２Ｄクリッピング処理
 *     フレームバッファの境界でクリップ
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  リターンコード
 *                                 １＝描画しない
 *                                 ０＝描画する
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
clip2DLevel3(PolTbl *polTbl)
{
    Uint32   f1,f2;
    Sint32   i,justInCnt;
    XyInt *xy, *xyb;

    /** BEGIN ***************************************************************/
    f1 = 0;
    xy = polTbl->xy;
    justInCnt = 0;
    if(xy->x < FRMBUF_MIN_X) f1 |= 8;
    if(xy->x > FRMBUF_MAX_X) f1 |= 4;
    if(xy->y < FRMBUF_MIN_Y) f1 |= 2;
    if(xy->y > FRMBUF_MAX_Y) f1 |= 1;
    for(i=0; i<4; i++) {
	xyb = xy;
	if(i == 3)
	    xy -= 3;
	else
	    xy++;
	f2 = 0;
	if(xy->x < FRMBUF_MIN_X) f2 |= 8;
	if(xy->x > FRMBUF_MAX_X) f2 |= 4;
	if(xy->y < FRMBUF_MIN_Y) f2 |= 2;
	if(xy->y > FRMBUF_MAX_Y) f2 |= 1;
	if(!(f1 & f2)) { /* just in or between */
	    if(!(f1 | f2))
		justInCnt++;
	    else
		if(!clipFrmCheck(xyb,xy,8))  return clipFrame(polTbl);
	}
	f1 = f2;
    }
    if(justInCnt >=  4)  return 0;
    return 1;
}

/*****************************************************************************
 *
 * NAME:  clipFrame()  - Clip Frame Buffer
 *
 * PARAMETERS :
 *
 *     (1) PolTbl  *polTbl  - <i/o>  ２Ｄポリゴンテーブル
 *
 * DESCRIPTION:
 *
 *     ポリゴンをフレームバッファ境界でクリッピングし、複数のポリゴン
 *     を生成する
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  リターンコード
 *                                 １＝描画しない
 *                                 ０＝描画する
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
clipFrame(PolTbl *polTbl)
{
    Sint32   i, j, in, out, visible;
    XyInt *pxy, *qxy, *wxy, w1xy[16], w2xy[16], fxy, sxy;

    /** BEGIN ***************************************************************/
    pxy = polTbl->xy;          /* 入力多角形 */
    qxy = w1xy;                /* 出力多角形 */
    in  = 4;                   /* 入力頂点数 */
    for(i=0; i<4; i++) {       /* フレームバッファ境界辺についてのループ */
	out = 0;               /* 出力頂点数 */
	for(j=0; j<in; j++) {  /* 表示多角形の辺についてのループ */
	    if(j == 0) {       /* 多角形の最初の点の場合 */
		fxy.x = pxy->x;
		fxy.y = pxy->y;
	    } else     {
		if(frmCrossChk(i,&sxy,&pxy[j],&qxy[out]))  out++;
	    }
	    sxy.x = pxy[j].x;
	    sxy.y = pxy[j].y;
	    visible = 0;
	    /* フレームバッファ境界の内側にポイントが入っているか可視チェック*/
	    switch(i) {
		case 0 : /* フレームバッファ左辺とのチェック */
		     if(FRMBUF_MIN_X <= sxy.x)  visible = 1;
		     break;
		case 1 : /* フレームバッファ上辺とのチェック */
		     if(FRMBUF_MIN_Y <= sxy.y)  visible = 1;
		     break;
		case 2 : /* フレームバッファ右辺との交差チェック */
		     if(sxy.x <= FRMBUF_MAX_X)  visible = 1;
		     break;
		case 3 : /* フレームバッファ下辺との交差チェック */
		     if(sxy.y <= FRMBUF_MAX_Y)  visible = 1;
		     break;
	    }
	    if(visible) {
		qxy[out].x = sxy.x;
		qxy[out].y = sxy.y;
		out++;
	    }
	}
	if(out) {    /* 最終辺についての処理 */
	    if(frmCrossChk(i,&sxy,&fxy,&qxy[out]))  out++;  /* 1 = cross */
	}
	in = out;
	if(i == 0) {
	    pxy = qxy;
	    qxy = w2xy;
	} else {
	    wxy = pxy;
	    pxy = qxy;
	    qxy = wxy;
	}
    }
    if(out >= 4) {   /* 多角形ポリゴンを４角形ポリゴンに分解 */
	qxy = pxy;
	wxy = polTbl->xy;
	for(i=0; i<4; i++,wxy++,qxy++) {
	    wxy->x = qxy->x;
	    wxy->y = qxy->y;
	}
	out -= 4;
	j = 1;
	qxy--;
	while(out) {
	    if(out >= 2) {
	       wxy  ->x = pxy  ->x;
	       wxy++->y = pxy  ->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy++->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy++->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy  ->y;
	       out -= 2;
	    } else       {
	       wxy  ->x = pxy  ->x;
	       wxy++->y = pxy  ->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy++->y;
	       wxy  ->x = qxy  ->x;
	       wxy++->y = qxy  ->y;
	       wxy  ->x = pxy  ->x;
	       wxy++->y = pxy  ->y;
	       out -= 1;
	    }
	    j++;
	}
	polTbl->polyCnt = j;
	return 0;
    } else
    if(out >= 3) {
	qxy = pxy;
	wxy = polTbl->xy;
	for(i=0; i<3; i++,wxy++,qxy++) {
	    wxy->x = qxy->x;
	    wxy->y = qxy->y;
	}
	wxy->x = pxy->x;
	wxy->y = pxy->y;
	return 0;
    } else
	return 1;
}


/*****************************************************************************
 *
 * NAME:  frmCrossChk()  - Frame Buffer Cross Check
 *
 * PARAMETERS :
 *
 *     (1) int       frmNo  - <i>  フレームバッファ境界の辺番号
 *                                 ０＝左辺
 *                                 １＝上辺
 *                                 ２＝右辺
 *                                 ３＝下辺
 *     (2) XyInt  *p1    - <i>  ラインのスタートポイント
 *     (2) XyInt  *p2    - <i>  ラインのエンドポイント
 *     (2) XyInt  *cp    - <o>  境界とラインのクロスポイント
 *
 *
 * DESCRIPTION:
 *
 *     指定フレームバッファの境界と指定ラインが交差しているかチェックし、
 *     交差している場合その交点を返す
 *
 *
 * POSTCONDITIONS:
 *
 *     int   rtncd          - <o>  リターンコード
 *                                 １＝交差している
 *                                 ０＝交差していない
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static int
frmCrossChk(int frmNo, XyInt *p1, XyInt *p2, XyInt *cp)
{
    Sint32   w1, w2, w3;

    /** BEGIN ***************************************************************/
    switch(frmNo) {
	case 0 : /* フレームバッファ左辺との交差チェック */
	     w1 = FRMBUF_MIN_X - p1->x;
	     w2 = FRMBUF_MIN_X - p2->x;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->x - p1->x;
	     if(w3 == 0) return 0;
	     cp->x = FRMBUF_MIN_X;
	     cp->y = p1->y + ((Sint32)(p2->y - p1->y)) * w1 / w3;
          /* if(cp->y < FRMBUF_MIN_Y) cp->y = FRMBUF_MIN_Y; else
             if(cp->y > FRMBUF_MAX_Y) cp->y = FRMBUF_MAX_Y; */
	     break;
	case 1 : /* フレームバッファ上辺との交差チェック */
	     w1 = FRMBUF_MIN_Y - p1->y;
	     w2 = FRMBUF_MIN_Y - p2->y;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->y - p1->y;
	     if(w3 == 0) return 0;
	     cp->x = p1->x + ((Sint32)(p2->x - p1->x)) * w1 / w3;
	     cp->y = FRMBUF_MIN_Y;
          /* if(cp->x < FRMBUF_MIN_X) cp->x = FRMBUF_MIN_X; else
             if(cp->x > FRMBUF_MAX_X) cp->x = FRMBUF_MAX_X; */
	     break;
	case 2 : /* フレームバッファ右辺との交差チェック */
	     w1 = FRMBUF_MAX_X - p1->x;
	     w2 = FRMBUF_MAX_X - p2->x;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->x - p1->x;
	     if(w3 == 0) return 0;
	     cp->x = FRMBUF_MAX_X;
	     cp->y = p1->y + ((Sint32)(p2->y - p1->y)) * w1 / w3;
          /* if(cp->y < FRMBUF_MIN_Y) cp->y = FRMBUF_MIN_Y; else
             if(cp->y > FRMBUF_MAX_Y) cp->y = FRMBUF_MAX_Y; */
	     break;
	case 3 : /* フレームバッファ下辺との交差チェック */
	     w1 = FRMBUF_MAX_Y - p1->y;
	     w2 = FRMBUF_MAX_Y - p2->y;
	     if((w1 * w2) >= 0)  return 0;
	     w3 = p2->y - p1->y;
	     if(w3 == 0) return 0;
	     cp->x = p1->x + ((Sint32)(p2->x - p1->x)) * w1 / w3;
	     cp->y = FRMBUF_MAX_Y;
          /* if(cp->x < FRMBUF_MIN_X) cp->x = FRMBUF_MIN_X; else
             if(cp->x > FRMBUF_MAX_X) cp->x = FRMBUF_MAX_X; */
	     break;
    }
    return 1;
}


/*****************************************************************************
 *
 * NAME:  transPolygons  - Set Polygon Data to polTbl
 *
 * PARAMETERS :
 *
 *     (1) DoubleBufArea  *buf    - <i>  カレント座標変換バッファ
 *     (12 XyInt          *vert2d - <O>  ２Ｄ座標保存ワークエリア
 *
 * DESCRIPTION:
 *
 *     ポリゴン描画コマンドの登録処理
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
static void
transPolygons(DoubleBufArea *buf, XyInt *vert2d)
{
    Sint32     i, j;
    Sint32     shadingKind, mateKind, bothFace, dispFlag, sBright;
    Uint32     zSortKind;
    Sint32     *wSurfBright, *vertBright, l;
    SprSurface *wSurface;
    SprObject3D *obj;
    PolTbl     polTbl;
    Uint16     *gour, w;
    Uint16     *wVertNo;
    Uint16     *shadingTbl;
    XyInt      *wxy;
    MthXyz     xyz[4], *wxyz, *coordView3D;
    Fixed32    zMax, *wFix1, *wFix2;

    static Uint16    grayCode[32] = {
        RGB16_COLOR( 0, 0, 0), RGB16_COLOR( 1, 1, 1),
        RGB16_COLOR( 2, 2, 2), RGB16_COLOR( 3, 3, 3),
        RGB16_COLOR( 4, 4, 4), RGB16_COLOR( 5, 5, 5),
        RGB16_COLOR( 6, 6, 6), RGB16_COLOR( 7, 7, 7),
        RGB16_COLOR( 8, 8, 8), RGB16_COLOR( 9, 9, 9),
        RGB16_COLOR(10,10,10), RGB16_COLOR(11,11,11),
        RGB16_COLOR(12,12,12), RGB16_COLOR(13,13,13),
        RGB16_COLOR(14,14,14), RGB16_COLOR(15,15,15),
        RGB16_COLOR(16,16,16), RGB16_COLOR(17,17,17),
        RGB16_COLOR(18,18,18), RGB16_COLOR(19,19,19),
        RGB16_COLOR(20,20,20), RGB16_COLOR(21,21,21),
        RGB16_COLOR(22,22,22), RGB16_COLOR(23,23,23),
        RGB16_COLOR(24,24,24), RGB16_COLOR(25,25,25),
        RGB16_COLOR(26,26,26), RGB16_COLOR(27,27,27),
        RGB16_COLOR(28,28,28), RGB16_COLOR(29,29,29),
        RGB16_COLOR(30,30,30), RGB16_COLOR(31,31,31)
    };

    static Uint16    texGrayCode[32] = {
        RGB16_COLOR( 8, 8, 8), RGB16_COLOR( 8, 8, 8),
        RGB16_COLOR( 9, 9, 9), RGB16_COLOR( 9, 9, 9),
        RGB16_COLOR(10,10,10), RGB16_COLOR(10,10,10),
        RGB16_COLOR(11,11,11), RGB16_COLOR(11,11,11),
        RGB16_COLOR(12,12,12), RGB16_COLOR(12,12,12),
        RGB16_COLOR(13,13,13), RGB16_COLOR(13,13,13),
        RGB16_COLOR(14,14,14), RGB16_COLOR(14,14,14),
        RGB16_COLOR(15,15,15), RGB16_COLOR(15,15,15),
        RGB16_COLOR(16,16,16), RGB16_COLOR(16,16,16),
        RGB16_COLOR(17,17,17), RGB16_COLOR(17,17,17),
        RGB16_COLOR(18,18,18), RGB16_COLOR(18,18,18),
        RGB16_COLOR(19,19,19), RGB16_COLOR(19,19,19),
        RGB16_COLOR(20,20,20), RGB16_COLOR(20,20,20),
        RGB16_COLOR(21,21,21), RGB16_COLOR(21,21,21),
        RGB16_COLOR(22,22,22), RGB16_COLOR(22,22,22),
        RGB16_COLOR(23,23,23), RGB16_COLOR(23,23,23),
    };

    /** BEGIN ***************************************************************/
    obj         = buf->object;
    coordView3D = buf->coordView3D;
    vertBright  = buf->vertBright;
    wSurfBright = buf->surfBright;
    wSurface    = obj->surface;
    shadingKind = obj->dispFlag & SHADING_MASK;
    bothFace    = obj->dispFlag & BOTH_FACE;
    wxy         = vert2d;
    for(i=0; i < obj->vertCount; i++) {
	wxy->x = 0x7fff;
	wxy++;
    }

    for(i = 0; i < obj->surfaceCount; i++, wSurfBright++, wSurface++) {
	sBright = *wSurfBright & 0xff;
	dispFlag = 1;
	if(*wSurfBright & 0x80000000)
	    if(bothFace) {
	       sBright = 31 - sBright;
	       dispFlag = 2;
	    } else
	       dispFlag = 0;

 /* *(Sint32*)0x6060008 += 1; */ /* ### */
	if(dispFlag) {
 /* *(Sint32*)0x606000c += 1; */ /* ### */
	    /* 頂点データの取り出し */
	    wVertNo  = wSurface->vertNo;
	    wFix1 = (Fixed32*)xyz;
	    for(j = 0; j < 4; j++, wVertNo++) {
		wFix2 = (Fixed32*)&coordView3D[*wVertNo];
		*wFix1++ = *wFix2++;   /* X */
		*wFix1++ = *wFix2++;   /* Y */
		*wFix1++ = *wFix2;     /* Z */
	    }

	    /* 代表Ｚ値による３Ｄクリッピング */
	    zMax = MAX(MAX(MAX(xyz[0].z,xyz[1].z),xyz[2].z),xyz[3].z);
	    if(clipLevel > 0)
		if((zMax < clipZMin) || (zMax > clipZMax)) continue;

	    /* 代表Ｚ値の取り出し */
	    zSortKind = wSurface->drawMode & ZSORT_MASK;
	    if(zSortKind == ZSORT_MIN)
		polTbl.z = MIN(MIN(MIN(xyz[0].z,xyz[1].z),xyz[2].z),xyz[3].z);
	    else
	    if(zSortKind == ZSORT_MAX)
		polTbl.z = zMax;
	    else
		polTbl.z = MTH_Mul(xyz[0].z+xyz[1].z+xyz[2].z+xyz[3].z,
		                   MTH_FIXED(0.25));

	    /* 透視変換 */
	    wVertNo = wSurface->vertNo;
	    wxyz    = xyz;
	    wxy     = polTbl.xy;
	    for(j = 0; j < 4; j++, wVertNo++) {
		if(vert2d[*wVertNo].x != 0x7fff) {
		    wxy->x = vert2d[*wVertNo].x;
		    wxy->y = vert2d[*wVertNo].y;
		} else {
		    MTH_Pers2D(wxyz, &unitPixel, wxy);
		    coord2D[*wVertNo].x = wxy->x;
		    coord2D[*wVertNo].y = wxy->y;
		}
		wxyz++;
		wxy++;
	    }

	    /* ２Ｄクリッピング */
	    polTbl.polyCnt = 1;
	    if(clipLevel == 2) {
		if(clip2DLevel2(&polTbl)) continue;
	    } else
	    if(clipLevel == 3) {
		if(clip2DLevel3(&polTbl)) continue;
	    }

	    if(shadingKind == GOURAUD_SHADING) {
		gour    = (Uint16*)&polTbl.gourTbl;
		wVertNo = wSurface->vertNo;
		if(obj->shdIdxTbl)
		    shadingTbl = obj->shdIdxTbl[0];
		else
		    shadingTbl = grayCode;
                if(dispFlag == 1) {
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour   = shadingTbl[vertBright[*wVertNo  ]];
                } else {
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour   = shadingTbl[31 - vertBright[*wVertNo  ]];
                }
	    }

	    polTbl.dispFlag = obj->dispFlag;
            mateKind = wSurface->drawMode & MATE_MASK;
	    if(shadingKind == FLAT_SHADING) {
		if(mateKind == MATE_SHADING_TBL) {
		    shadingTbl = obj->shdIdxTbl[wSurface->color];
		    polTbl.surfBright = shadingTbl[sBright];
		} else
		if(mateKind == MATE_TEXTURE) {
		    gour    = (Uint16*)&polTbl.gourTbl;
		    if(obj->shdIdxTbl) {
		        shadingTbl = obj->shdIdxTbl[0];
		        w = shadingTbl[sBright];
		    } else
		        w = texGrayCode[sBright];
		    *gour++ = w;
		    *gour++ = w;
		    *gour++ = w;
		    *gour   = w;
		} else {
		    polTbl.surfBright =
			    SPR_3GetShdColor(wSurface->color,sBright);
		}
	    }
	    polTbl.drawMode   = wSurface->drawMode;
	    polTbl.color      = wSurface->color;

	    drawPol(&polTbl);
	}
    }
}


/*****************************************************************************
 *
 * NAME:  viewCoordMatrix()  - set View Coord Convert Matrix To
 *                                                    current matrix
 *
 * PARAMETERS :
 *
 * 　　なし
 *
 *
 * DESCRIPTION:
 *
 *     カレントマトリックスに視点座標系への変換マトリックスをセット
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
static void viewCoordMatrix(void)
{
    MTH_InitialMatrix(&viewMatrixTbl, 1, &viewMatrix);
    MTH_ReverseZ(&viewMatrixTbl);
    MTH_MoveMatrix(&viewMatrixTbl, viewCoordPoint.x,
				   viewCoordPoint.y,
				   viewCoordPoint.z);
    switch(viewAngleSeq) {
        case ROT_SEQ_ZYX :
   	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     break;
        case ROT_SEQ_ZXY :
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     break;
        case ROT_SEQ_YZX :
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     break;
        case ROT_SEQ_YXZ :
             MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     break;
        case ROT_SEQ_XYZ :
  	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     break;
        case ROT_SEQ_XZY :
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     break;
    }
    MTH_MoveMatrix(&viewMatrixTbl, -viewPoint.x, -viewPoint.y, -viewPoint.z);
    MTH_NormalTrans(&viewMatrix, &worldLightVector, &viewLight.lightVector);
    if(postWorldDataFlag) {
        MTH_ClearMatrix(&worldMatrixTbl);
    } else {
	memcpy(worldMatrixTbl.current, &viewMatrix, sizeof(MthMatrix));
    }

}


/*****************************************************************************
 *
 * NAME:  lightAngle()  -
 *
 * PARAMETERS :
 *
 * 　　MthXyz *lightAngle
 *
 *
 * DESCRIPTION:
 *
 *     光源のアングルを設定する
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
static void setLightAngle(void)
{
    MTH_InitialMatrix(&lightMatrixTbl, 1, &lightMatrix);
    MTH_RotateMatrixZ(&lightMatrixTbl, lightAngle.z);
    MTH_RotateMatrixY(&lightMatrixTbl, lightAngle.y);
    MTH_RotateMatrixX(&lightMatrixTbl, lightAngle.x);
    MTH_NormalTrans(&lightMatrix, &orgLightVect, &worldLightVector);
}

/*  end of file */
