# Docker for SEGA Saturn dev

## Goal

Get a cross platform starting point, with everything up and running out of the box

## Usage

build it : 
docker build -t saturn-docker . --file ./Dockerfile

Run it :
docker run -it --rm -v $(pwd):/saturn saturn-docker /bin/bash

## Build status

### Tools 
- [x] GCC for SH2
- [x] Cmake profile
- [x] ISO generation tools

### SGL 3.00
- [x] SGL
- [ ] SGL Samples

### SBL 6.01
- [x] SBL Libs
  - [x] blp
  - [x] cdc
  - [x] cmp
  - [x] csh
  - [x] dbg
  - [x] dma
  - [x] dsp
  - [x] gfs
  - [ ] int
  - [x] mem
  - [x] mpg
  - [x] per
  - [x] scl
  - [x] spr
  - [x] stm
- [ ] SBL Samples
  - [x] bup
  - [x] csh
  - [ ] dbg
  - [ ] dma
    - [ ] dma0
    - [ ] dma1
    - [ ] dma2
    - [ ] dma10
    - [ ] dma12 
  - [ ] dual
  - [ ] fld
  - [ ] game
  - [ ] game_cd
  - [ ] gfs
  - [ ] mem
  - [ ] mpg
  - [ ] pcm
  - [ ] per
  - [ ] sblsgl
    - [ ] sblsgl00
    - [ ] sblsgl01
    - [ ] sblsgl02
    - [ ] sblsgl03
    - [ ] sblsgl04
  - [ ] scl
  - [ ] sgl
  - [ ] snd
  - [ ] spr
    - [x] spr1
    - [x] spr7
    - [ ] spr8
  - [ ] stm
  - [ ] sys
  - [ ] tim


### SDK Samples
- [x] SDK Samples
  - [x] Basic
  - [x] Sprite
  - [x] Textured Sprite
  - [ ] CDBrowser

### Jo Engine
- [ ] Jo Engine


Misc notes :

https://github.com/RetroReversing/retroReversing/tree/master/pages/sega/saturn
