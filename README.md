# Docker for SEGA Saturn dev

## Goal

Get a cross platform starting point, with everything up and running out of the box

## Usage

build it :
docker build -t saturn-docker . --file ./Dockerfile

Run it :
docker run -it --rm -v $(pwd):/saturn saturn-docker /bin/bash

## Content

TODO

## Build status

### Tools
- [x] GCC for SH2
- [x] Cmake profile
- [x] ISO generation tools

### SGL 3.02
- [x] SGL
- [ ] SGL Samples

### SBL 6.01
- [x] SBL Libs
  - [x] blp
  - [x] cdc **(copied from https://github.com/shicky256/sbl/tree/master/segalib/cdc)**
  - [x] cmp
  - [x] csh
  - [x] dbg
  - [x] dma
  - [x] dsp
  - [x] gfs
  - [x] int
  - [x] mem
  - [x] mpg
  - [x] mth **(copied from https://github.com/shicky256/sbl/tree/master/segalib/mth)**
  - [ ] pcm
  - [x] per **(copied from https://github.com/shicky256/sbl/tree/master/segalib/per)**
  - [x] scl **(copied from https://github.com/shicky256/sbl/tree/master/segalib/scl)**
  - [ ] snd
  - [x] spr **(copied from https://github.com/shicky256/sbl/tree/master/segalib/spr)**
  - [x] stm
- [ ] SBL Samples
  - [x] bup
  - [x] csh
  - [ ] dbg
  - [ ] dma
    - [x] dma0
    - [x] dma1
    - [x] dma2
    - [x] dma10
    - [x] dma12
  - [ ] dual **(required or just sample code ?)**
  - [ ] fld
  - [ ] game
  - [ ] game_cd
  - [ ] gfs
  - [ ] mem
  - [ ] mpg
  - [ ] pcm
    - [ ] pcm2 **(Crash at runtime)**
  - [ ] per
  - [ ] sblsgl
    - [ ] sblsgl00 **(Crash at runtime)**
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
    - [ ] spr8 **(Crash at runtime)**
  - [ ] stm
  - [ ] sys
  - [ ] tim


### SDK Samples
- [x] SDK Samples
  - [x] Basic
  - [x] Sprite
  - [x] Textured Sprite
  - [ ] CDBrowser

### Jo Engine (https://www.jo-engine.org/)
- [ ] Jo Engine

### Yaul (https://github.com/ijacquez/libyaul)
- [ ] Yaul


## Misc notes :

https://github.com/RetroReversing/retroReversing/tree/master/pages/sega/saturn

Boost preprocessor introduction :
https://www.boost.org/doc/libs/1_76_0/libs/preprocessor/doc/AppendixA-AnIntroductiontoPreprocessorMetaprogramming.html
