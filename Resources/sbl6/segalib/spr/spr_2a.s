!-----------------------------------------------------------------------------
!  spr_2a.src -- SPR ライブラリ 2A モジュール
!  Copyright(c) 1994 SEGA
!  Written by H.E on 1994-07-25 Ver.1.00
!  Updated by H.E on 1994-07-25 Ver.1.00
!
!  このライブラリはスプライト表示拡張（２Ｄ）処理モジュールで、以下のルーチン
!  を含む。
!
!  SPR_2SetDrawPrty         -  コマンド描画プライオリティのセット
!  SPR_2FlushDrawPrtyBlock  -  コマンド描画プライオリティブロックのフラッシュ
!
!----------------------------------------------------------------------------
!
          .extern     _SpDrawPrtyBlkMax
          .extern     _SpCmdNo
          .extern     _SpDrawPrtyTopCmdNo
          .extern     _SpDrawPrtyProcFlag
          .extern     _SpCmdChainBlkNo
          .extern     _SpCmdChain
          .global     _SpDrawPrtyBlk
          .extern     _SpCrCmdChain
          .extern     _SpCmdTbl
          .extern     _SpCmdTblR
!
          .global     _SPR_2SetDrawPrty
          .global     _SPR_2FlushDrawPrtyBlock
!


.if 1
.equ nextNo,0
.equ cmdNo,2

.equ topNo,0
.equ botNo,2

.equ control,0
.equ link,2
.equ drawMode,4
.equ color,6
.equ charAddr,8
.equ charSize,10
.equ ax,12
.equ ay,14
.equ bx,16
.equ by,18
.equ cx,20
.equ cy,22
.equ dx,24
.equ dy,26
.equ grshAddr,28
.endif

.if 0
					.SECTION    SprCmdChain,DUMMY
nextNo    .RES.W      1
cmdNo     .RES.W      1
!
          .SECTION    SprDrawPrtyBlk,DUMMY
topNo     .RES.W      1
botNo     .RES.W      1
!
          .SECTION    SprSpCmd,DUMMY
control   .RES.W      1
link      .RES.W      1
drawMode  .RES.W      1
color     .RES.W      1
charAddr  .RES.W      1
charSize  .RES.W      1
ax        .RES.W      1
ay        .RES.W      1
bx        .RES.W      1
by        .RES.W      1
cx        .RES.W      1
cy        .RES.W      1
dx        .RES.W      1
dy        .RES.W      1
grshAddr  .RES.W      1
dummy     .RES.W      1
.endif

!
          .section .text

!*****************************************************************************
!*
!* NAME:  SPR_2SetDrawPrty - Set Draw Priority Command Chain
!*
!*      void SPR_2SetDrawPrty(int drawPrtyNo)!
!*
!* PARAMETERS :
!*
!*     (1) int   drawPrtyNo   - <i>  コマンド描画プライオリティ番号
!*
!* DESCRIPTION:
!*
!*     コマンド描画プライオリティ管理ブロックにコマンド番号を設定する
!*
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

_SPR_2SetDrawPrty:
                                        ! r4 : drawPrtyNo
!
          MOV.L       SDP_SpDrawPrtyBlkMax,r1
          MOV.L       @r1,r1            ! r6 : SpDrawPrtyBlkMax
          CMP/ge      r1,r4
          BF          SDP_02
          MOV         r1,r4
          ADD         #-1,r4
SDP_02:
          MOV.L       SDP_SpCrCmdChain,r7
          MOV         r7,r3             ! r7,r3 : &SpCrCmdChain
          MOV.L       @r7,r7
          ADD         #4,r7
          MOV.L       r7,@r3            ! SpCrCmdChain++
          ADD         #-4,r7            ! r7 : wCmdChain
          MOV.L       SDP_SpCmdNo,r2
          MOV.L       @r2,r0            ! r0 = SpCmdNo
          MOV.W       r0,@(cmdNo,r7)    ! wCmdChain->cmdNo = SpCmdNo
          MOV         #-1,r1            !
          MOV         r1,r0
          MOV.W       r0,@(nextNo,r7)   ! wCmdChain->nextNo = 0xffff
!
          MOV.L       SDP_SpCmdChainBlkNo,r5 ! r5 : &SpCmdChainBlkNo
          MOV.L       SDP_SpDrawPrtyBlk,r6
          MOV.L       @r6,r6            ! r6 : SpDrawPrtyBlk
          MOV         r4,r2
          SHLL2       r2
          ADD         r2,r6             ! r6 : &SpDrawPrtyBlk[drawPrtyNo]
          MOV.W       @(topNo,r6),r0
          EXTS.W      r0,r0             ! r0 = SpDrawPrtyBlk[drawPrtyNo].topNo
          CMP/eq      r0,r1             ! if(r0 != 0xffff) goto SDP_00
          BF          SDP_00
          MOV.L       @r5,r0
          BRA         SDP_01
          MOV.W       r0,@(topNo,r6)    ! SpDrawPrtyBlk[drawPrtyNo].topNo =
!                                              SpCmdChainBlkNo
SDP_00:
          MOV.W       @(botNo,r6),r0    ! i = SpDrawPrtyBlk[drawPrtyNo].botNo
          EXTU.W      r0,r0
          MOV.L       SDP_SpCmdChain,r7 ! r7 : &SpCmdChain
          MOV.L       @r7,r7
          SHLL2       r0
          ADD         r0,r7             ! r7 : wCmdChain = &SpCmdChain[i]
          MOV.L       @r5,r0
          MOV.W       r0,@(nextNo,r7)   ! wCmdChain->nextNo = SpCmdChainBlkNo
!
SDP_01:
          MOV.W       r0,@(botNo,r6)    ! SpDrawPrtyBlk[drawPrtyNo].botNo =
!                                                            SpCmdChainBlkNo
          ADD         #1,r0
          RTS
          MOV.L       r0,@r5            ! SpCmdChainBlkNo++
!
	  .align 2

SDP_SpDrawPrtyBlk:
          .long     _SpDrawPrtyBlk
SDP_SpCmdChainBlkNo:
          .long     _SpCmdChainBlkNo
SDP_SpCrCmdChain:
          .long     _SpCrCmdChain
SDP_SpCmdNo:
          .long     _SpCmdNo
SDP_SpCmdChain:
          .long     _SpCmdChain
SDP_SpDrawPrtyBlkMax:
          .long     _SpDrawPrtyBlkMax
!
!

!FLAG:  .ASSIGNA 1
!.AIF \&FLAG EQ 1

!*****************************************************************************
!*
!* NAME:  SPR_2FlushDrawPrtyBlock()  - Flush Draw Priority Block
!*
!*     void SPR_2FlushDrawPrtyBlock(void)!
!*
!* PARAMETERS :
!*
!*     No exist.
!*
!* DESCRIPTION:
!*
!*     コマンド描画プライオリティブロックのフラッシュ
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

_SPR_2FlushDrawPrtyBlock:
!
          MOV.L       FDP_SpCmdChainBlkNo,r0
          CMP/eq      #0,r0             ! if(SpCmdChainBlkNo != 0)
          BF          FDP_04            !    goto FDP_04
          RTS
          NOP
FDP_04:
          MOV.L       r12,@-r15
          MOV.L       r11,@-r15
          MOV.L       r10,@-r15
          MOV.L       r9,@-r15
          MOV.L       r8,@-r15
!
          MOV.L       FDP_SpCmdChain,r8
          MOV.L       @r8,r8            ! r8 : SpCmdChain
          MOV.L       FDP_SpCmdTbl,r9
          MOV.L       @r9,r9            ! r9 : SpCmdTbl
          MOV.L       FDP_SpCmdNo,r12
          MOV.L       @r12,r12          ! r12: SpCmdNo
!
          MOV.L       FDP_SpDrawPrtyProcFlag,r0
          MOV.L       @r0,r0
          CMP/eq      #1,r0             ! if(SpDrawPrtyProcFlag != 1)
          BF          FDP_03            !    goto FDP_03
!
          MOV.L       FDP_SpDrawPrtyBlk,r4
          MOV.L       @r4,r4            ! r4 : wZBlk  = SpDrawPrtyBlk
          MOV.L       FDP_SpDrawPrtyTopCmdNo,r5
          MOV.L       @r5,r5            ! r5 : wCmdNo = SpDrawPrtyTopCmdNo
          MOV.L       FDP_SpDrawPrtyBlkMax,r7
          MOV.L       @r7,r7            ! r7 : SpDrawPrtyBlkMax
          MOV.L       FDP_SpCmdTblR,r10
          MOV.W       @r10,r0
          EXTU.W      r0,r10            ! r10: SpCmdTblR
          MOV         #-1,r6            ! r6 : 0xffff
!
FDP_00:
          MOV         #0,r0
          CMP/eq      r0,r7             ! if(r7 == 0)
          BT.s        FDP_02            !    goto FDP_02
          ADD         #-1,r7
!
          MOV.W       @(topNo,r4),r0
          EXTS.W      r0,r3             ! r3 : chainBlkNo   = wZBlk->topNo
          MOV         r6,r0
          MOV.W       r0,@(topNo,r4)    ! wZBlk->topNo = 0xffff
          ADD         #4,r4             ! wZBlk++
!
FDP_01:
          CMP/eq      r3,r6             ! if(chainBlkNo == 0xffff)
          BT.s        FDP_00            !    goto FDP_00
          MOV         r8,r2
          SHLL2       r3
          ADD         r3,r2             ! r2 : wCmdChain =
!                                                   &SpCmdChain[chainBlkNo]
          MOV		      r9,r11
          MOV         r5,r0
          SHLL2       r0
          SHLL2       r0
          SHLL        r0
          ADD         r0,r11            ! r11 : spCmd = SpCmdTbl + wCmdNo
!
          MOV.W       @(control,r11),r0
          MOV.L       FDP_JUMP_ASSIGN,r1
          OR          r1,r0
          MOV.W       r0,@(control,r11) ! spCmd->control |= JUMP_ASSIGN
!
          MOV.W       @(cmdNo,r2),r0
          EXTS.W      r0,r5             ! r5 : wCmdNo = wCmdChain->cmdNo
!
          MOV.W       @(nextNo,r2),r0
          EXTS.W      r0,r3             ! r3 : chainBlkNo = wCmdChain->nextNo
!
          MOV         r10,r0
          MOV         r5,r1
          SHLL2       r1
          ADD         r1,r0
          BRA         FDP_01
          MOV.W       r0,@(link,r11)    ! spCmd->link =
!                                                   SpCmdTblR + (wCmdNo << 2)
FDP_02:
          MOV         r9,r11
          MOV         r5,r0
          SHLL2       r0
          SHLL2       r0
          SHLL        r0
          ADD         r0,r11            ! r11 : spCmd = SpCmdTbl + wCmdNo
!
          MOV.W       @(control,r11),r0
          MOV.L       FDP_JUMP_ASSIGN,r1
          OR          r1,r0
          MOV.W       r0,@(control,r11) ! spCmd->control |= JUMP_ASSIGN
!
          MOV         r10,r0
          MOV         r12,r1
          SHLL2       r1
          ADD         r1,r0
          MOV.W       r0,@(link,r11)    ! spCmd->link =
!                                                   SpCmdTblR + (SpCmdNo << 2)
FDP_03:
          MOV.L       FDP_SpCmdChainBlkNo,r1
          MOV         #0,r0
          MOV.L       r0,@r1            ! SpCmdChainBlkNo = 0
!
          MOV.L       FDP_SpCrCmdChain,r1
          MOV.L       r8,@r1            ! SpCrCmdChain = SpCmdChain
!
          MOV.L       FDP_SpDrawPrtyTopCmdNo,r5
          MOV.L       r12,@r5           ! SpDrawPrtyTopCmdNo = SpCmdNo
!
          MOV         r12,r2
          ADD         #1,r12
          MOV.L       FDP_SpCmdNo,r1
          MOV.L       r12,@r1
          SHLL2       r2
          SHLL2       r2
          SHLL        r2
          ADD         r2,r9             ! r9 : spCmd = SpCmdTbl + SpCmdNo++
!
          MOV.L       FDP_CTRL_SKIP,r0
          MOV.W       r0,@(control,r9)  ! spCmd->control = CTRL_SKIP
!
          MOV.L       @r15+,r8
          MOV.L       @r15+,r9
          MOV.L       @r15+,r10
          MOV.L       @r15+,r11
          RTS
          MOV.L       @r15+,r12
!
	  .align 2

FDP_JUMP_ASSIGN:
          .long     0x00001000
FDP_CTRL_SKIP:
          .long     0x00004000
FDP_SpCmdTblR:
          .long     _SpCmdTblR
FDP_SpCmdChain:
          .long     _SpCmdChain
FDP_SpCmdTbl:
          .long     _SpCmdTbl
FDP_SpDrawPrtyProcFlag:
          .long     _SpDrawPrtyProcFlag
FDP_SpDrawPrtyBlk:
          .long     _SpDrawPrtyBlk
FDP_SpDrawPrtyTopCmdNo:
          .long     _SpDrawPrtyTopCmdNo
FDP_SpDrawPrtyBlkMax:
          .long     _SpDrawPrtyBlkMax
FDP_SpCmdNo:
          .long     _SpCmdNo
FDP_SpCmdChainBlkNo:
          .long     _SpCmdChainBlkNo
FDP_SpCrCmdChain:
          .long     _SpCrCmdChain
!
! .AENDI
!
      	  .end
