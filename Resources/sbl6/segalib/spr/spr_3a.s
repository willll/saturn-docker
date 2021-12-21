!-----------------------------------------------------------------------------
!  spr_3a.src -- SPR ライブラリ 2A モジュール
!  Copyright(c) 1994 SEGA
!  Written by H.E on 1994-07-25 Ver.1.00
!  Updated by H.E on 1994-07-25 Ver.1.00
!
!  このライブラリはスプライト３Ｄ表示処理モジュールで、以下のルーチンを含む。
!
!  SPR_3GetShdColor     -  ＲＧＢカラーの輝度による変換
!
!----------------------------------------------------------------------------
!
          .global     _SPR_3GetShdColor
          .section .text

!*****************************************************************************
!*
!* NAME:  SPR_3GetShdColor  - Compute RGB Shading Color
!*
!*        rgbColor = getShdColor(Uint16 baseRgbColor, Uint16 bright)
!*
!* PARAMETERS :
!*
!*     (1) Uint16  baseRgbColor  - <i>  元ＲＧＢカラー
!*     (2) Uint16  bright        - <i>  輝度（０－３１）
!*
!* DESCRIPTION:
!*
!*     ＲＧＢカラーを輝度により計算し返す
!*
!*
!* POSTCONDITIONS:
!*
!*     (1) Uint16  rgbColor      - <o>  ＲＧＢカラー
!*
!* CAVEATS:
!*
!*****************************************************************************
!
	.align 2

_SPR_3GetShdColor:
 	  			STS.L	      macl,@-r15
!                                      ! r4  : baseRgbColor
!                                      ! r5  : bright
          EXTU.W      r4,r1            ! r1  : r
          MOV         r1,r2            ! r2  : g
          MOV         r1,r3            ! r3  : b
          MOV         #31,r0
          AND         r0,r1            ! r =  baseRgbColor & 0x001f
          MOV.W       GS_03E0,r4
          AND         r4,r2
          SHLR2       r2
          SHLR2       r2
          SHLR        r2               ! g = (baseRgbColor & 0x03e0) >> 5
          MOV.W       GS_7C00,r0
          AND         r0,r3
          SHLR8       r3
          SHLR2       r3               ! b = (baseRgbColor & 0x7c00) >> 10
!
          MOV.L       GS_8421,r0       ! rgbColor  = 0x8421
          MUL.L       r1,r5
          STS         macl,r6
          SHLR2       r6
          SHLR2       r6
          SHLR        r6
          ADD         r6,r0            ! rgbColor += (r * bright) >> 5
          MUL.L       r2,r5
          STS         macl,r6
          AND         r4,r6
          ADD         r6,r0            ! rgbColor += ((g * bright) & 0x03e0)
          MUL.L       r3,r5
          STS         macl,r6
          AND         r4,r6
          SHLL2       r6
          SHLL2       r6
          SHLL        r6
          ADD         r6,r0         ! rgbColor += ((b * bright) & 0x03e0) << 5
          RTS
          LDS.L       @r15+,macl
!
	  .align 2

GS_03E0:  .word     0x03E0
GS_7C00:  .word     0x7C00
GS_8421:  .long     0x00008421
          .END
