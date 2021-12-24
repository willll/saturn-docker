docker build -t saturn-docker . --file ./Dockerfile

docker run -it --rm -v $(pwd):/saturn saturn-docker /bin/bash

Build status :
- [x] GCC for SH2
- [x] SGL
- [ ] SGL Samples
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
  - [ ] spr
    - [x] spr1
    - [x] spr7
    - [ ] spr8 
- [x] SDK Samples
  - [x] Basic
  - [x] Sprite
  - [x] Textured Sprite
  - [ ] CDBrowser
- [ ] Jo Engine


Misc notes :

https://github.com/RetroReversing/retroReversing/tree/master/pages/sega/saturn
