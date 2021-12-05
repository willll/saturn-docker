!----------------------------------------------------------------------------
!  mth_mtrx.c -- MTH ライブラリ マトリックス演算処理 モジュール
!  Copyright(c) 1994 SEGA
!  Written by H.E on 1994-03-28 Ver.0.80
!  Updated by H.E on 1994-03-28 Ver.0.80
!
!  このライブラリはマトリックス演算処理モジュールで、
!  以下のルーチンを含む。
!
!  MTH_InitialMatrix       -  マトリックススタックの初期化
!  MTH_ClearMatrix         -  カレントマトリックスのクリア
!  MTH_PushMatrix          -  マトリックスのプッシュ
!  MTH_PopMatrix           -  マトリックスのポップ
!  MTH_MoveMatrix          -  マトリックス合成・平行移動
!  MTH_RotateMatrixX       -  マトリックス合成・Ｘ軸回転
!  MTH_RotateMatrixY       -  マトリックス合成・Ｙ軸回転
!  MTH_RotateMatrixZ       -  マトリックス合成・Ｚ軸回転
!  MTH_ReverseZ            -  マトリックス合成・Ｚ軸符号反転
!  MTH_MulMatrix           -  マトリックス乗算
!  MTH_CoordTrans          -  マトリックス演算・頂点の座標変換
!  MTH_NormalTrans         -  マトリックス演算・法線ベクトルの座標変換
!
!----------------------------------------------------------------------------
!
          .extern     _MTH_Sin
          .extern     _MTH_Cos
          .extern     _MTH_Mul
          .global     _MTH_InitialMatrix
          .global     _MTH_ClearMatrix
          .global     _MTH_PushMatrix
          .global     _MTH_PopMatrix
          .global     _MTH_MoveMatrix
          .global     _MTH_RotateMatrixX
          .global     _MTH_RotateMatrixY
          .global     _MTH_RotateMatrixZ
          .global     _MTH_ReverseZ
          .global     _MTH_MulMatrix
          .global     _MTH_CoordTrans
          .global     _MTH_NormalTrans

          .section .text
!
!
!*****************************************************************************
!*
!* NAME:  MTH_InitialMatrix()  - Initial Matrix Stack
!*
!*     void MTH_InitialMatrix(MthMatrixTbl *matrixTbl, Uint16 stackSize,
!*						     MthMtrix *matrix)
!*
!* PARAMETERS :
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <o> マトリックステーブル
!* 　　(2) Uint16       stackSize  - <i> マトリックススタックの最大エントリ数
!* 　　(3) MthMatrix    *matrix    - <i> マトリックススタックエリア
!*
!* DESCRIPTION:
!*
!*     マトリックススタックテーブルの初期化
!*
!* POSTCONDITIONS:
!*
!*     No exist.
!*
!* CAVEATS:
!*
!*****************************************************************************
!

	.align 2

_MTH_InitialMatrix:             ! function: MTH_InitialMatrix
          STS.L       pr,@-r15
          MOV.W       r5,@r4
          MOV.L       r6,@(8,r4)
          MOV.L       r6,@(4,r4)
          BRA         _MTH_ClearMatrix
          LDS.L       @r15+,pr
!
!
!*****************************************************************************
!*
!* NAME:  MTH_ClearMatrix()  - clear current matrix to unit matrix
!*
!*     void MTH_ClearMatrix(MthMatrixTbl *matrixTbl)
!*
!* PARAMETERS :
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!*
!* DESCRIPTION:
!*
!*     マトリックススタック内のカレントマトリックスを単位行列にクリアする。
!*
!* POSTCONDITIONS:
!*
!*     No exist.
!*
!* CAVEATS:
!*
!*****************************************************************************
!
_MTH_ClearMatrix:               ! function: MTH_ClearMatrix
          MOV.L       cm_unitMatrix,r6
          MOV.L       @(4,r4),r5
          MOV         #12,r0
cm_0:
	  MOV.L       @r6+,r1
          MOV.L       r1,@r5
          DT          r0
          BF.S        cm_0
          ADD         #4,r5
          RTS
          NOP
!
	  .ALIGN 2

cm_unitMatrix:
          .long     unitMatrix
!
!
!*************************************************************************
!*
!* NAME : MTH_PushMatrix    - push current matrix to matrix stack
!*
!*     void MTH_PushMatrix(MthMatrixTbl *matrixTbl)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!*
!* DESCRIPTION
!*
!*	カレントマトリックスのプッシュ処理
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_PushMatrix:                ! function: MTH_PushMatrix
          MOV.L       @(4,r4),r6
          MOV.L       @(4,r4),r5
          ADD         #48,r5
          MOV         r5,r2
          MOV         #12,r0
pm_0:
	  MOV.L       @r6+,r1
          MOV.L       r1,@r5
          DT          r0
          BF.S        pm_0
          ADD         #4,r5
          RTS
          MOV.L       r2,@(4,r4)
!
!
!*************************************************************************
!*
!* NAME : MTH_PopMatrix    - pop current matrix from matrix stack
!*
!*     void MTH_PopMatrix(MthMatrixTbl *matrixTbl)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!*
!* DESCRIPTION
!*
!*	カレントマトリックスのポップ処理
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_PopMatrix:                 ! function: MTH_PopMatrix
          MOV.L       @(4,r4),r3
          ADD         #-48,r3
          RTS
          MOV.L       r3,@(4,r4)
!
!
!*************************************************************************
!*
!* NAME : MTH_MoveMatrix   - Move Matrix
!*
!*     void MTH_MoveMatrix(MthMatrixTbl *matrixTbl,
!*                                       Fixed32 x, Fixed32 y, Fixed32 z)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!* 　　(2) Fixed32      x          - <i> Ｘ方向の移動量
!* 　　(3) Fixed32      y          - <i> Ｙ方向の移動量
!* 　　(4) Fixed32      z          - <i> Ｚ方向の移動量
!*
!* DESCRIPTION
!*
!* カレントマトリックスに対しＸＹＺ平行移動のマトリックス合成を行う
!*
!* | M00 M01 M02 M03 | | 1 0 0 x |   | M00 M01 M02 M00*x+M01*y+M02*z+M03 |
!* | M10 M11 M12 M13 |.| 0 1 0 y | = | M10 M11 M12 M10*x+M11*y+M12*z+M13 |
!* | M20 M21 M22 M23 | | 0 0 1 z |   | M20 M21 M22 M20*x+M21*y+M22*z+M23 |
!* |   0   0   0   1 | | 0 0 0 1 |   |   0   0   0                     1 |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_MoveMatrix:                ! function: MTH_MoveMatrix
          MOV.L       @(4,r4),r3 ! r3 = currentMatrix
          MOV.L       r7,@-r15   ! z
          MOV.L       r6,@-r15   ! y
          MOV.L       r5,@-r15   ! x
          STS         mach,r6
          STS         macl,r7
!
          MOV         r15,r4     ! r4 = (x, y, z)
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M00*x+M01*y+M02*z
          MOV         r3,r2
          MOV.L       @r3+,r1
          ADD         r0,r1      ! r1 = r0 + M03
          MOV.L       r1,@r2     ! currentMatrix[0][3] = r1
!
          MOV         r15,r4     ! @r4 = (x, y, z)
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M10*x+M11*y+M12*z
          MOV         r3,r2
          MOV.L       @r3+,r1
          ADD         r0,r1      ! r1 = r0 + M03
          MOV.L       r1,@r2     ! currentMatrix[1][3] = r1
!
          MOV         r15,r4     ! r4 = (x, y, z)
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M20*x+M21*y+M22*z
          MOV.L       @r3,r1
          ADD         r0,r1      ! r1 = r0 + M03
	  ADD         #12,r15
          LDS         r6,mach
          LDS         r7,macl
          RTS
          MOV.L       r1,@r3     ! currentMatrix[2][3] = r1
!
!
!*************************************************************************
!*
!* NAME : MTH_RotateMatrixX  - Rotate Matrix X
!*
!*     void    MTH_RotateMatrixX(MthMatrixTbl *matrixTbl, Fixed32 xDegree)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!* 　　(2) Fixed32      xDegree    - <i> Ｘ軸の回転角度（-180から180の範囲）
!*
!* DESCRIPTION
!*
!* カレントマトリックスに対しＸ軸回転のマトリックス合成を行う
!*
!* | M00 M01 M02 M03 | | 1    0    0 0 |
!* | M10 M11 M12 M13 |.| 0  cos -sin 0 |
!* | M20 M21 M22 M23 | | 0  sin  cos 0 |
!* |   0   0   0   1 | | 0    0    0 1 |
!*
!*		         | M00 M01*cos+M02*sin -M01*sin+M02*cos M03 |
!*		       = | M10 M11*cos+M12*sin -M11*sin+M12*cos M13 |
!*		         | M20 M21*cos+M22*sin -M21*sin+M22*cos M23 |
!*		         |   0               0                0   1 |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_RotateMatrixX:             ! function: MTH_RotateMatrixX
          MOV.L       r8,@-r15
          MOV.L       r9,@-r15
          MOV.L       r10,@-r15
          STS.L       pr,@-r15
          STS.L       macl,@-r15
          STS.L       mach,@-r15
          MOV         r4,r8
          MOV         r5,r9
!
          MOV.L       rmx_MTH_Sin,r3
          JSR         @r3
          MOV         r9,r4
          MOV         r0,r10     ! r10 = sin
!
          MOV.L       rmx_MTH_Cos,r3
          JSR         @r3
          MOV         r9,r4      ! r0  = cos
!
          MOV.L       @(4,r8),r6 ! r6 = currentMatrix
          MOV.L       r0,@-r15   ! Push   cos
          NEG         r10,r1
          MOV.L       r1,@-r15   ! Push  -sin
          MOV         r15,r8     ! @r8 = (-sin,cos)
          MOV.L       r10,@-r15  ! Push   sin
          MOV.L       r0,@-r15   ! Push   cos
          MOV         r15,r7     ! @r7 = (cos,sin)
!
          MOV.L       @(8,r6),r0
          MOV.L       r0,@-r15   ! Push  M02
          MOV.L       @(4,r6),r0
          MOV.L       r0,@-r15   ! Push  M01
          MOV         r15,r9     ! @r9 = (M01,M02)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M01*cos+M02*sin
          MOV.L       r0,@(4,r6) ! M01 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = -M01*sin+M02*cos
          MOV.L       r0,@(8,r6) ! M02 = r0
	  ADD         #8,r15
!
          MOV.L       @(24,r6),r0
          MOV.L       r0,@-r15   ! Push  M12
          MOV.L       @(20,r6),r0
          MOV.L       r0,@-r15   ! Push  M11
          MOV         r15,r9     ! @r9 = (M11,M12)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M11*cos+M12*sin
          MOV.L       r0,@(20,r6) ! M11 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = -M11*sin+M12*cos
          MOV.L       r0,@(24,r6) ! M12 = r0
	  ADD         #8,r15
!
          MOV.L       @(40,r6),r0
          MOV.L       r0,@-r15   ! Push  M22
          MOV.L       @(36,r6),r0
          MOV.L       r0,@-r15   ! Push  M21
          MOV         r15,r9     ! @r9 = (M21,M22)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M21*cos+M22*sin
          MOV.L       r0,@(36,r6) ! M21 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = -M21*sin+M22*cos
          MOV.L       r0,@(40,r6) ! M22 = r0
	  ADD         #24,r15
!
          LDS.L       @r15+,mach
          LDS.L       @r15+,macl
          LDS.L       @r15+,pr
          MOV.L       @r15+,r10
          MOV.L       @r15+,r9
          RTS
          MOV.L       @r15+,r8
!
	  .align 2

rmx_MTH_Sin:
          .long     _MTH_Sin
rmx_MTH_Cos:
          .long     _MTH_Cos
!
!
!*************************************************************************
!*
!* NAME : MTH_RotateMatrixY  - Rotate Matrix Y
!*
!*     void    MTH_RotateMatrixY(MthMatrixTbl *matrixTbl, Fixed32 yDegree)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!* 　　(2) Fixed32      yDegree    - <i> Ｙ軸の回転角度（-180から180の範囲）
!*
!* DESCRIPTION
!*
!* カレントマトリックスに対しＹ軸回転のマトリックス合成を行う
!*
!* | M00 M01 M02 M03 | |  cos 0  sin 0 |
!* | M10 M11 M12 M13 |.|    0 1    0 0 |
!* | M20 M21 M22 M23 | | -sin 0  cos 0 |
!* |   0   0   0   1 | |    0 0    0 1 |
!*
!*			|  M00*cos-M02*sin  M01 M00*sin+M02*cos M03 |
!*		    =	|  M10*cos-M12*sin  M11 M10*sin+M12*cos M13 |
!*			|  M20*cos-M22*sin  M21 M20*sin+M22*cos M23 |
!*			|                0    0               0   1 |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_RotateMatrixY:             ! function: MTH_RotateMatrixY
          MOV.L       r8,@-r15
          MOV.L       r9,@-r15
          MOV.L       r10,@-r15
          STS.L       pr,@-r15
          STS.L       macl,@-r15
          STS.L       mach,@-r15
          MOV         r4,r8
          MOV         r5,r9
!
          MOV.L       rmy_MTH_Sin,r3
          JSR         @r3
          MOV         r9,r4
          MOV         r0,r10     ! r10 = sin
!
          MOV.L       rmy_MTH_Cos,r3
          JSR         @r3
          MOV         r9,r4      ! r0  = cos
!
          MOV.L       @(4,r8),r6 ! r6 = currentMatrix
          MOV.L       r0,@-r15   ! Push   cos
          MOV.L       r10,@-r15  ! Push   sin
          MOV         r15,r8     ! @r8 = (sin,cos)
          NEG         r10,r1
          MOV.L       r1,@-r15   ! Push  -sin
          MOV.L       r0,@-r15   ! Push   cos
          MOV         r15,r7     ! @r7 = (cos,-sin)
!
          MOV.L       @(8,r6),r0
          MOV.L       r0,@-r15   ! Push  M02
          MOV.L       @(0,r6),r0
          MOV.L       r0,@-r15   ! Push  M00
          MOV         r15,r9     ! @r9 = (M00,M02)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M00*cos-M02*sin
          MOV.L       r0,@(0,r6) ! M00 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M00*sin+M02*cos
          MOV.L       r0,@(8,r6) ! M02 = r0
	  ADD         #8,r15
!
          MOV.L       @(24,r6),r0
          MOV.L       r0,@-r15   ! Push  M12
          MOV.L       @(16,r6),r0
          MOV.L       r0,@-r15   ! Push  M10
          MOV         r15,r9     ! @r9 = (M10,M12)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M10*cos-M12*sin
          MOV.L       r0,@(16,r6) ! M10 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M10*sin+M12*cos
          MOV.L       r0,@(24,r6) ! M12 = r0
	  ADD         #8,r15
!
          MOV.L       @(40,r6),r0
          MOV.L       r0,@-r15   ! Push  M22
          MOV.L       @(32,r6),r0
          MOV.L       r0,@-r15   ! Push  M20
          MOV         r15,r9     ! @r9 = (M20,M22)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M20*cos-M22*sin
          MOV.L       r0,@(32,r6) ! M20 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M20*sin+M22*cos
          MOV.L       r0,@(40,r6) ! M22 = r0
	  ADD         #24,r15
!
          LDS.L       @r15+,mach
          LDS.L       @r15+,macl
          LDS.L       @r15+,pr
          MOV.L       @r15+,r10
          MOV.L       @r15+,r9
          RTS
          MOV.L       @r15+,r8
!
	  .align 2

rmy_MTH_Sin:
          .long     _MTH_Sin
rmy_MTH_Cos:
          .long     _MTH_Cos
!
!
!*************************************************************************
!*
!* NAME : MTH_RotateMatrixZ  - Rotate Matrix Z
!*
!*     void MTH_RotateMatrixZ(MthMatrixTbl *matrixTbl, Fixed32 zDegree)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!* 　　(2) Fixed32      zDegree    - <i> Ｚ軸の回転角度（-180から180の範囲）
!*
!* DESCRIPTION
!*
!* | M00 M01 M02 M03 | | cos -sin  0 0 |
!* | M10 M11 M12 M13 |.| sin  cos  0 0 |
!* | M20 M21 M22 M23 | |   0    0  1 0 |
!* |   0   0   0   1 | |   0    0  0 1 |
!*
!*				| M00*cos+M01*sin -M00*sin+M01*cos M02 M03 |
!*			      =	| M10*cos+M11*sin -M10*sin+M11*cos M12 M13 |
!*				| M20*cos+M21*sin -M20*sin+M21*cos M22 M23 |
!*				|               0                0   0   1 |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_RotateMatrixZ:             ! function: MTH_RotateMatrixZ
                                ! frame size=76
          MOV.L       r8,@-r15
          MOV.L       r9,@-r15
          MOV.L       r10,@-r15
          STS.L       pr,@-r15
          STS.L       macl,@-r15
          STS.L       mach,@-r15
          MOV         r4,r8
          MOV         r5,r9
!
          MOV.L       rmz_MTH_Sin,r3
          JSR         @r3
          MOV         r9,r4
          MOV         r0,r10     ! r10 = sin
!
          MOV.L       rmz_MTH_Cos,r3
          JSR         @r3
          MOV         r9,r4      ! r0  = cos
!
          MOV.L       @(4,r8),r6 ! r6 = currentMatrix
          MOV.L       r0,@-r15   ! Push   cos
          NEG         r10,r1
          MOV.L       r1,@-r15   ! Push  -sin
          MOV         r15,r8     ! @r8 = (-sin,cos)
          MOV.L       r10,@-r15  ! Push   sin
          MOV.L       r0,@-r15   ! Push   cos
          MOV         r15,r7     ! @r7 = (cos,sin)
!
          MOV.L       @(4,r6),r0
          MOV.L       r0,@-r15   ! Push  M01
          MOV.L       @(0,r6),r0
          MOV.L       r0,@-r15   ! Push  M00
          MOV         r15,r9     ! @r9 = (M00,M01)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M00*cos+M01*sin
          MOV.L       r0,@(0,r6) ! M00 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = -M00*sin+M01*cos
          MOV.L       r0,@(4,r6) ! M01 = r0
	  ADD         #8,r15
!
          MOV.L       @(20,r6),r0
          MOV.L       r0,@-r15   ! Push  M11
          MOV.L       @(16,r6),r0
          MOV.L       r0,@-r15   ! Push  M10
          MOV         r15,r9     ! @r9 = (M10,M11)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M10*cos+M11*sin
          MOV.L       r0,@(16,r6) ! M10 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = -M10*sin+M11*cos
          MOV.L       r0,@(20,r6) ! M11 = r0
	  ADD         #8,r15
!
          MOV.L       @(36,r6),r0
          MOV.L       r0,@-r15   ! Push  M21
          MOV.L       @(32,r6),r0
          MOV.L       r0,@-r15   ! Push  M20
          MOV         r15,r9     ! @r9 = (M20,M21)
          MOV         r7,r4
          MOV         r9,r3
          CLRMAC
          MAC.L       @r3+,@r4+
          MAC.L       @r3+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = M20*cos+M21*sin
          MOV.L       r0,@(32,r6) ! M20 = r0
          MOV         r8,r4
          CLRMAC
          MAC.L       @r9+,@r4+
          MAC.L       @r9+,@r4+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0       ! r0 = -M20*sin+M21*cos
          MOV.L       r0,@(36,r6) ! M21 = r0
	  ADD         #24,r15
!
          LDS.L       @r15+,mach
          LDS.L       @r15+,macl
          LDS.L       @r15+,pr
          MOV.L       @r15+,r10
          MOV.L       @r15+,r9
          RTS
          MOV.L       @r15+,r8
!
	  .align 2

rmz_MTH_Sin:
          .long     _MTH_Sin
rmz_MTH_Cos:
          .long     _MTH_Cos
!
!
!*************************************************************************
!*
!* NAME : MTH_ReverseZ  - Revers Signe Z
!*
!*     void    MTH_ReverseZ(MthMatrixTbl *matrixTbl)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrixTbl *matrixTbl - <i> マトリックステーブル
!*
!* DESCRIPTION
!*
!* カレントマトリックスに対しＺ軸符号反転のマトリックス合成を行う
!*
!* | M00 M01 M02 M03 | | 1 0  0 0 |
!* | M10 M11 M12 M13 |.| 0 1  0 0 |
!* | M20 M21 M22 M23 | | 0 0 -1 0 |
!* |   0   0   0   1 | | 0 0  0 1 |
!*
!*				| M00 M01 -M02 M03 |
!*			      =	| M10 M11 -M12 M13 |
!*				| M20 M21 -M22 M23 |
!*				|   0   0    0   1 |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_ReverseZ:                  ! function: MTH_ReverseZ
          MOV.L       @(4,r4),r6  ! r6 = currentMatrix
          MOV.L       @(8,r6),r0
          NEG         r0,r0
          MOV.L       r0,@(8,r6)  ! M02 = -M02
!
          MOV.L       @(24,r6),r0
          NEG         r0,r0
          MOV.L       r0,@(24,r6) ! M12 = -M12
!
          MOV.L       @(40,r6),r0
          NEG         r0,r0
          RTS
          MOV.L       r0,@(40,r6) ! M22 = -M22
!
!
!*************************************************************************
!*
!* NAME : MTH_MulMatrix  -  Multiple Matrix
!*
!*     void   MTH_MulMatrix(MthMatrix *a, MthMatrix *b, MthMatrix *c)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrix  *a - <i> 被乗算マトリックス
!* 　　(2) MthMatrix  *b - <i> 乗算マトリックス
!* 　　(3) MthMatrix  *c - <o> 乗算結果マトリックス
!*
!* DESCRIPTION
!*
!* マトリックス乗算処理
!*
!* | a00 a01 a02 a03 | | b00 b01 b02 b03 |	| c00 c01 c02 c03 |
!* | a10 a11 a12 a13 |.| b10 b11 b12 b13 |  =	| c10 c11 c12 c13 |
!* | a20 a21 a22 a23 | | b20 b21 b22 b23 |	| c20 c21 c22 c23 |
!* |   0   0   0   1 | |   0   0   0   1 |	|   0   0   0   1 |
!*
!*    c00 = a00*b00+a01*b10+a02*b20
!*    c10 = a10*b00+a11*b10+a12*b20
!*    c20 = a20*b00+a21*b10+a22*b20
!*
!*    c01 = a00*b01+a01*b11+a02*b21
!*    c11 = a10*b01+a11*b11+a12*b21
!*    c21 = a20*b01+a21*b11+a22*b21
!*
!*    c02 = a00*b02+a01*b12+a02*b22
!*    c12 = a10*b02+a11*b12+a12*b22
!*    c22 = a20*b02+a21*b12+a22*b22
!*
!*    c03 = a00*b03+a01*b13+a02*b23+a03
!*    c13 = a10*b03+a11*b13+a12*b23+a13
!*    c23 = a20*b03+a21*b13+a22*b23+a23
!*
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_MulMatrix:                 ! function: MTH_MulMatrix
    	    STS.L       mach,@-r15
          STS.L       macl,@-r15
!---------
          MOV.L       @(32,r5),r0
          MOV.L       r0,@-r15      ! Push b20
          MOV.L       @(16,r5),r0
          MOV.L       r0,@-r15      ! Push b10
          MOV.L       @(0,r5),r0
          MOV.L       r0,@-r15      ! Push b00
!
          MOV         r4,r2         ! r2 = a00
          MOV         r15,r3        ! r3 = (b00, b10, b20)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a00*b00+a01*b10+a02*b20
          MOV.L       r0,@(0,r6)    ! c00 = r0
!
          ADD         #4,r2         ! r2  = a10
          MOV         r15,r3        ! r3 = (b00, b10, b20)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a10*b00+a11*b10+a12*b20
          MOV.L       r0,@(16,r6)   ! c10 = r0
!
          ADD         #4,r2         ! r2  = a20
          MOV         r15,r3        ! r3 = (b00, b10, b20)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a20*b00+a21*b10+a22*b20
          MOV.L       r0,@(32,r6)   ! c20 = r0
!---------
	  ADD         #12,r15
          MOV.L       @(36,r5),r0
          MOV.L       r0,@-r15      ! Push b21
          MOV.L       @(20,r5),r0
          MOV.L       r0,@-r15      ! Push b11
          MOV.L       @(4,r5),r0
          MOV.L       r0,@-r15      ! Push b01
!
          MOV         r4,r2         ! r2 = a00
          MOV         r15,r3        ! r3 = (b01, b11, b21)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a00*b01+a01*b11+a02*b21
          MOV.L       r0,@(4,r6)    ! c01 = r0
!
          ADD         #4,r2         ! r2  = a10
          MOV         r15,r3        ! r3 = (b01, b11, b21)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a10*b01+a11*b11+a12*b21
          MOV.L       r0,@(20,r6)   ! c11 = r0
!
          ADD         #4,r2         ! r2  = a20
          MOV         r15,r3        ! r3 = (b01, b11, b21)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a20*b01+a21*b11+a22*b21
          MOV.L       r0,@(36,r6)   ! c21 = r0
!---------
	  ADD         #12,r15
          MOV.L       @(40,r5),r0
          MOV.L       r0,@-r15      ! Push b22
          MOV.L       @(24,r5),r0
          MOV.L       r0,@-r15      ! Push b12
          MOV.L       @(8,r5),r0
          MOV.L       r0,@-r15      ! Push b02
!
          MOV         r4,r2         ! r2 = a00
          MOV         r15,r3        ! r3 = (b02, b12, b22)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a00*b02+a01*b12+a02*b22
          MOV.L       r0,@(8,r6)    ! c02 = r0
!
          ADD         #4,r2         ! r2  = a10
          MOV         r15,r3        ! r3 = (b02, b12, b22)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a10*b02+a11*b12+a12*b22
          MOV.L       r0,@(24,r6)   ! c12 = r0
!
          ADD         #4,r2         ! r2  = a20
          MOV         r15,r3        ! r3 = (b02, b12, b22)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a20*b02+a21*b12+a22*b22
          MOV.L       r0,@(40,r6)   ! c22 = r0
!---------
	  ADD         #12,r15
	  MOV         #1,r0
	  SHLL16      r0
          MOV.L       r0,@-r15      ! Push 1.0
          MOV.L       @(44,r5),r0
          MOV.L       r0,@-r15      ! Push b23
          MOV.L       @(28,r5),r0
          MOV.L       r0,@-r15      ! Push b13
          MOV.L       @(12,r5),r0
          MOV.L       r0,@-r15      ! Push b03
!
          MOV         r4,r2         ! r2 = a00
          MOV         r15,r3        ! r3 = (b03, b13, b23, 1.0)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a00*b03+a01*b13+a02*b23+a03
          MOV.L       r0,@(12,r6)   ! c03 = r0
!
          MOV         r15,r3        ! r3 = (b03, b13, b23, 1.0)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a10*b03+a11*b13+a12*b23+a13
          MOV.L       r0,@(28,r6)   ! c13 = r0
!
          MOV         r15,r3        ! r3 = (b03, b13, b23, 1.0)
          CLRMAC
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
          MAC.L       @r2+,@r3+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0         ! r0  = a20*b03+a21*b13+a22*b23+a23
!
	  ADD         #16,r15
          LDS.L       @r15+,macl
          LDS.L       @r15+,mach
          RTS
          MOV.L       r0,@(44,r6)   ! c23 = r0
!
!
!*************************************************************************
!*
!* NAME : MTH_CoordTrans  -  Coord Transfer of Vertices
!*
!*     void    MTH_CoordTrans(MthMatrix *matrix, MthXyz *src, MthXyz *ans)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrix *matrix - <i> 座標変換マトリックス
!* 　　(2) MthXyz    *src    - <i> 変換前頂点座標
!* 　　(3) MthXyz    *ans    - <o> 変換後頂点座標
!*
!* DESCRIPTION
!*
!* マトリックス演算・頂点の座標変換
!*
!*	| Xn |	 | M00, M01, M02, M03 | | Xa |   | M00Xa+M01Ya+M02Za+M03 |
!*	| Yn | = | M10, M11, M12, M13 |.| Ya | = | M10Xa+M11Ya+M12Za+M13 |
!*	| Zn |	 | M20, M21, M22, M23 | | Za |   | M20Xa+M21Ya+M22Za+m23 |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_CoordTrans:                ! function: MTH_CoordTrans
          STS         mach,r2
          STS         macl,r3
!
          MOV         r5,r7     ! r7 = (Xa, Ya, Za)
          CLRMAC
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M00Xa+M01Ya+M02Za
          MOV.L       @r4+,r1
          ADD         r0,r1      ! r1 = r0 + M03
          MOV.L       r1,@(0,r6) ! Xn = r1
!
          MOV         r5,r7     ! r7 = (Xa, Ya, Za)
          CLRMAC
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M10Xa+M11Ya+M12Za
          MOV.L       @r4+,r1
          ADD         r0,r1      ! r1 = r0 + M13
          MOV.L       r1,@(4,r6) ! Yn = r1
!
          MOV         r5,r7     ! r7 = (Xa, Ya, Za)
          CLRMAC
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M20Xa+M21Ya+M22Za
          MOV.L       @r4+,r1
          ADD         r0,r1      ! r1 = r0 + M23
!
          LDS         r2,mach
          LDS         r3,macl
          RTS
          MOV.L       r1,@(8,r6) ! Zn = r1
!
!
!*************************************************************************
!*
!* NAME : MTH_NormalTrans  -  Coord Transfer of Normal
!*
!*     void    MTH_NormalTrans(MthMatrix *matrix, MthXyz *src, MthXyz *ans)
!*
!* PARAMETERS
!*
!* 　　(1) MthMatrix  *matrix - <i> 座標変換マトリックス
!* 　　(2) MthXyz     *src    - <i> 変換前法線ベクトル
!* 　　(2) MthXyz     *ans    - <o> 変換後法線ベクトル
!*
!* DESCRIPTION
!*
!*	| Xn |	 | M00, M01, M02 | | Xa |   | M00Xa+M01Ya+M02Za |
!*	| Yn | = | M10, M11, M12 |.| Ya | = | M10Xa+M11Ya+M12Za |
!*	| Zn |	 | M20, M21, M22 | | Za |   | M20Xa+M21Ya+M22Za |
!*
!* POSTCONDITIONS
!*
!* CAVEATS
!*
!*************************************************************************
!
_MTH_NormalTrans:               ! function: MTH_NormalTrans
                                ! frame size=32
          STS         mach,r2
          STS         macl,r3
!
          MOV         r5,r7     ! r7 = (Xa, Ya, Za)
          CLRMAC
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M00Xa+M01Ya+M02Za
          ADD         #4,r4
          MOV.L       r0,@(0,r6) ! Xn = r0
!
          MOV         r5,r7     ! r7 = (Xa, Ya, Za)
          CLRMAC
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M10Xa+M11Ya+M12Za
          ADD         #4,r4
          MOV.L       r0,@(4,r6) ! Yn = r0
!
          MOV         r5,r7     ! r7 = (Xa, Ya, Za)
          CLRMAC
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
          MAC.L       @r4+,@r7+
  	  STS	      mach,r1
	  STS	      macl,r0
	  XTRCT	      r1,r0      ! r0 = M20Xa+M21Ya+M22Za
!
          LDS         r2,mach
          LDS         r3,macl
          RTS
          MOV.L       r0,@(8,r6) ! Zn = r0
!
!
!*************************************************************************
!*
!*  Data Define
!*
!*************************************************************************
!
          .section .data

	  .align 2

unitMatrix:
          .long     0x00010000,0x00000000,0x00000000,0x00000000
	  .long     0x00000000,0x00010000,0x00000000,0x00000000
	  .long     0x00000000,0x00000000,0x00010000,0x00000000

	  .END
