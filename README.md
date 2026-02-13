## Table of Contents
1. [Goal](#goal)
2. [Usage](#usage)
  - [Build it](#build-it)
  - [Run it](#run-it)
  - [Map a device from host to container](#map-a-device-from-host-to-container)
  - [Use it in an IDE](#use-it-in-an-ide)
  - [Change GCC Version for SH2](#change-gcc-version-for-sh2)
    - [Available GCC Versions](#available-gcc-versions)
    - [Example](#example)
    - [Advanced Build Examples](#advanced-build-examples)
  - [Docker Hub](#docker-hub)
  - [Mount with SSHFS](#mount-with-sshfs)
  - [Copy Your SSH Key into the Container](#copy-your-ssh-key-into-the-container)
3. [Content](#content)
  - [List of variables](#list-of-variables)
    - [SSHD variables](#sshd-variables)
    - [Boost variables](#boost-variables)
    - [GCC(SH2) build variables](#gccsh2-build-variables)
    - [SGL variables](#sgl-variables)
    - [SBL variables](#sbl-variables)
    - [SATURN SDK variables](#saturn-sdk-variables)
    - [JO ENGINE variables](#jo-engine-variables)
    - [Yaul variables](#yaul-variables)
    - [SRL variables](#srl-variables)
    - [IAPETUS variables](#iapetus-variables)
    - [CyberwarriorX CDC](#cyberwarriorx-cdc)
  - [Build status](#build-status)
    - [Tools](#tools)
    - [SGL 3.02](#sgl-302)
    - [SGL 3.20](#sgl-320)
    - [SBL 6.01](#sbl-601)
    - [SDK Samples](#sdk-samples)
    - [Jo Engine 2025.1](#jo-engine-20251)
    - [Yaul Version 0.3.1](#yaul-version-031)
    - [SRL Version 0.9](#srl-version-09)
4. [Misc notes](#misc-notes)

---

## Goal

Provide a cross-platform starting point with everything set up and ready to use out of the box.

---

## Usage

### Build it
To build the Docker image:
```bash
docker build -t saturn-docker . --file ./Dockerfile
```

### Run it
To run the container interactively:
```bash
docker run -it --rm -v $(pwd):/saturn saturn-docker /bin/bash
```
To run the container with SSH support and current user:
```bash
docker run -it -p 2222:22 --rm --user $(id -u):$(id -g) -v $(pwd):/saturn saturn-docker /bin/bash
```

### Map a device from host to container
To map a device from the host to the container, you can use the `--device` flag with `docker run`. For example, to map `/dev/ttyUSB0` on the host to `/dev/ttyUSB0` in the container:
```bash
docker run -it --rm --device=/dev/ttyUSB0:/dev/ttyUSB0 -v $(pwd):/saturn saturn-docker /bin/bash
```
Alternatively, you can modify the `compose.yaml` to include the device mapping:
```yaml
services:
  build-saturn:
    build: .
    ports:
      - "${SSH_PORT:-2222}:22"
    command: /bin/bash
    tty: true
    devices:
      - "/dev/ttyUSB0:/dev/ttyUSB0"
```

For dynamic device access (when device paths change), you can allow a class of devices with `--device-cgroup-rule`. For example, allow all USB serial (`ttyUSB*`) devices (major 188), and give access rights to /dev/bus/usb (major 189):
```bash
docker run -it --rm \
  --device-cgroup-rule='c 188:0 rmw' \
  --device-cgroup-rule='c 189:* rmw' \
  -v /dev/bus/usb:/dev/bus/usb \
  -v /dev:/dev \
  -v $(pwd):/saturn saturn-docker /bin/bash
```
You still need the device node to exist in the container. If it does not appear automatically, add a bind mount for a specific device or run the container in privileged mode.

### Use it in an IDE
For IDE integration, refer to:
- [VSCode Setup](Documentation/VisualIdiot.md)

### Change GCC Version for SH2
The default GCC version is **14.3.0**. To change it, use the `--build-arg GCC_VERSION_ARG` parameter during the build process.

#### Available GCC Versions:
- **14.3.0** (default)
- **14.2.0** 
- ~~**14.1.0**~~ (deprecated)
- **13.3.0**
- ~~**13.2.0**~~ (deprecated)
- **12.4.0**
- ~~**12.3.0**~~ (deprecated)
- ~~**12.2.0**~~ (deprecated)
- **11.5.0**
- ~~**11.3.0**~~ (deprecated)
- **10.4.0**
- **9.5.0**
- **8.4.0**

#### Example:
```bash
docker build --build-arg GCC_VERSION_ARG=14.3.0 -t saturn-docker . --file ./Dockerfile
```

#### Advanced Build Examples:
```bash
docker build \
  --build-arg GCC_VERSION_ARG=14.3.0 \
  --build-arg INSTALL_SATURNSDK_SAMPLES=0 \
  --build-arg INSTALL_JO_ENGINE_LIB=0 \
  --build-arg INSTALL_JO_ENGINE_SAMPLES=0 \
  --build-arg INSTALL_YAUL_LIB_ARG=0 \
  --build-arg INSTALL_YAUL_SAMPLES=0 \
  --build-arg INSTALL_SBL_LIB=1 \
  --build-arg INSTALL_SBL_SAMPLES=0 \
  --build-arg INSTALL_SBL_EXAMPLES=0 \
  --build-arg INSTALL_SGL_LIB=0 \
  --build-arg INSTALL_SGL_SAMPLES=0 \
  -t saturn-docker . --file ./Dockerfile
```

```bash
docker build  --build-arg GCC_VERSION_ARG=12.4.0 \
  --build-arg INSTALL_SATURNSDK_SAMPLES=1 \
  --build-arg INSTALL_JO_ENGINE_LIB=1 \
  --build-arg INSTALL_JO_ENGINE_SAMPLES=1 \
  --build-arg INSTALL_YAUL_LIB_ARG=1 \
  --build-arg INSTALL_YAUL_SAMPLES=1 \
  --build-arg INSTALL_SBL_LIB=1 \
  --build-arg INSTALL_SBL_SAMPLES=1 \
  --build-arg INSTALL_SBL_EXAMPLES=1 \
  --build-arg INSTALL_SGL_LIB=1 \
  --build-arg INSTALL_SGL_SAMPLES=1 \
  --build-arg MAKEFLAGS_ARG="-j 8" \
  -t saturn-docker . --file ./Dockerfile
```

### Docker Hub
Explore the images on Docker Hub: [Saturn Docker Tags](https://hub.docker.com/r/willll/saturn-docker/tags).

### Mount with SSHFS
To mount the container with SSHFS:
```bash
sudo sshfs -o allow_other root@192.168.1.161:/saturn /home/will/tmp/sshfs/ -p 2222
```

### Copy Your SSH Key into the Container
```bash
docker build --build-arg ssh_pub_key="$(cat ~/.ssh/host_ca.pub)" --squash -t saturn-docker . --file ./Dockerfile
```

---

## Content

### List of variables

#### SSHD variables

| Variable        | Default Value | Description          |
|------------------|---------------|----------------------|
| `ssh_pub_key`    | *(empty)*     | SSH public key content |

#### Boost variables

| Variable        | Default Value | Description          |
|------------------|---------------|----------------------|
| `INSTALL_BOOST_LIB`    | 1     | 0 to disable Boost installation |

#### GCC(SH2) build variables

| Variable           | Default Value | Description                         |
|---------------------|---------------|-------------------------------------|
| `MAKEFLAGS_ARG`     | "-j 1"        | Extra parameters for `make`         |
| `GCC_VERSION_ARG`   | 14.3.0        | GCC version                        |
| `NCPU`              | 1             | Number of CPUs for GCC build        |
| `BUILD_TYPE_ARG`    | Release       | Build type (Release/Debug)          |

#### SGL variables

| Variable             | Default Value | Description             |
|-----------------------|---------------|-------------------------|
| `INSTALL_SGL_LIB`     | 1             | 0 to disable SGL library |
| `INSTALL_SGL_SAMPLES` | 1             | 0 to disable SGL samples |

#### SBL variables

| Variable               | Default Value | Description       |
|-------------------------|---------------|-------------------|
| `INSTALL_SBL_LIB`      | 1             | 0 to disable      |
| `INSTALL_SBL_SAMPLES`  | 1             | 0 to disable      |
| `INSTALL_SBL_EXAMPLES` | 1             | 0 to disable      |

#### SATURN SDK variables

| Variable                     | Default Value | Description       |
|-------------------------------|---------------|-------------------|
| `INSTALL_SATURNSDK_SAMPLES`  | 1             | 0 to disable      |

#### JO ENGINE variables

| Variable                 | Default Value | Description       |
|---------------------------|---------------|-------------------|
| `INSTALL_JO_ENGINE_LIB`   | 1             | 0 to disable      |
| `INSTALL_JO_ENGINE_SAMPLES` | 1           | 0 to disable      |
| `JO_ENGINE_TAG`           | v2024.2       | Specify the version/tag |

#### Yaul variables

| Variable                     | Default Value                     | Description                                |
|-------------------------------|-----------------------------------|--------------------------------------------|
| `INSTALL_YAUL_LIB_ARG`        | 1                                 | 0 to disable                               |
| `INSTALL_YAUL_SAMPLES`        | 0                                 | 0 to disable                               |
| `YAUL_TAG`                    | 0.3.1                             | Tag to retrieve from Yaul's GitHub         |
| `YAUL_EXAMPLES_COMMIT_SHA`    | 21fd76d83dffd49afc4926d6a8408eecfec474f5 | Commit matching `YAUL_TAG` in Yaul examples GitHub |

#### SRL variables

| Variable                 | Default Value | Description       |
|---------------------------|---------------|-------------------|
| `INSTALL_SRL_LIB`   | 1             | 0 to disable      |
| `SRL_LIB_TAG`           | 0.9.1       | Specify the version/tag |

#### IAPETUS variables

| Variable                   | Default Value                     | Description       |
|-----------------------------|-----------------------------------|-------------------|
| `INSTALL_IAPETUS_LIB`       | 0                                 | 1 to enable       |
| `INSTALL_IAPETUS_SAMPLES`   | 0                                 | 1 to enable       |
| `IAPETUS_COMMIT_SHA`        | 955d7c50f634cdd18722657c920987200d9ba3a5 | Specify commit SHA |

#### CyberwarriorX CDC

| Variable                        | Default Value | Description       |
|----------------------------------|---------------|-------------------|
| `INSTALL_CYBERWARRIORX_CDC_LIB` | 0             | 1 to enable       |

### Build status

### Tools
- [x] GCC 8.4.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_8.4.0
- [x] GCC 9.5.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_9.5.0
- [x] GCC 10.4.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_10.4.0
- [x] GCC 11.5.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_11.5.0
- [x] GCC 12.4.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_12.4.0
- [x] GCC 13.3.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_13.3.0
- [x] GCC 14.2.0 for SH2 : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_14.2.0
- [x] GCC 14.3.0 for SH2 **(Default)** : https://github.com/willll/Saturn-SDK-GCC-SH2/tree/gcc_14.3.0
- [ ] GDB for SH2 **(Not tested yet)**
- [X] GCC 14.2.0 for M68K : https://github.com/willll/Saturn-SDK-GCC-M68K
- [x] CMake profile
- [x] schily-tools : ISO generation tools
- [x] Saturn-SDK-Tool-IPMaker : IP.BIN generation tool : https://github.com/willll/Saturn-SDK-Tool-IPMaker
- [x] satcon : Sega Saturn image convertor https://git.sr.ht/~ndiddy/satconv/


#### [SGL 3.02](SGL_3.02/README.md)
<details>

- [X] `SGL` **(patched)**
- [ ] `SGL Samples`
  - [x] `akira`
  - [x] `akira3`
  - [x] `demo_a`
  - [x] `demo_b`
  - [x] `demo_c`
  - [x] `demo_d`
  - [x] `demogun`
  - [x] `design1`
  - [x] `design2`
  - [x] `design3`
  - [x] `driving`
  - [x] `driving2`
  - [x] `fighting`
  - [x] `s_2_2`
  - [x] `s_3_2`
  - [x] `s_4_2`
  - [x] `s_4_3_1`
  - [x] `s_4_3_2`
  - [x] `s_4_3_3`
  - [x] `s_4_3_4`
  - [x] `s_4_5`
  - [x] `s_5_2`
  - [x] `s_6_3`
  - [x] `s_7_2`
  - [x] `s_7_4`
  - [x] `s_7_6`
  - [x] `s_8_8_1`
  - [x] `s_8_8_2`
  - [x] `s_8_8_3`
  - [x] `s_8_9_1`
  - [x] `s_8_9_2`
  - [x] `s_8_10_1`
  - [x] `s_8_10_2`
  - [x] `s_8_11`
  - [x] `s_8_12`
  - [x] `s_9_1`
  - [x] `s_10`
  - [x] `s_cd1`
  - [x] `s_cd2`
  - [x] `s_cd3`
  - [x] `s_cd4`
  - [x] `sampsnd1`
  - [x] `sampsnd2`
  - [ ] `sbklib`
  - [x] `sega2d_1`
  - [x] `sega3d_1`
  - [x] `sega3d_2`
  - [x] `sega3d_3`
  - [x] `shooting`
  - [x] `torus2`
  - [x] `torus3`
- [ ] `SGL Samples from SGL 3.20`
  - [X] `bitmap`
  - [X] `cdda` **(Missing tracks ?)**
  - [X] `coloff`
  - [X] `expccalc`
  - [X] `grad`
  - [X] `linecol`
  - [X] `linscl`
  - [ ] `pcm`
    - [ ] `seamless`
      - [ ] `all` **(Error : Stream System open)**
      - [ ] `basic` **(Error : Stream System open)**      
      - [ ] `ff` **(Error : Stream System open)**     
      - [ ] `pause` **(Error : Stream System open)**                 
    - [ ] `simple`
      - [ ] `all` **(Error : Stream System open)**
      - [ ] `gfs` **(Error : Stream System open)**      
      - [ ] `mem` **(Error : Stream System open)**     
      - [ ] `stm` **(Error : Stream System open)**   
  - [X] `rotatebg`
    - [X] `coef`
    - [X] `linecol`
  - [X] `shadow`
    - [X] `msb`
    - [X] `sprite`
  - [ ] `sound`
      - [ ] `mapchg` **(Black screen)**
      - [ ] `sndtest` **(no sounds ?)**
  - [X] `spccalc`
    - [X] `cel`
    - [X] `dot`
  - [X] `spprio`
    - [X] `cel`
    - [X] `dot`
  - [X] `sprcc2bg`
    - [X] `apply`
    - [X] `basic`  
  - [X] `window`
    - [X] `sprwin`
- [ ] `SGL Demos`
  - [ ] `biplane` **(Missing files in scroll folder)**
  - [ ] `cdda_sgl` **(Missing files in src folder)**
  - [x] `chrome`
  - [x] `flying`
</details>

#### SGL 3.20
<details>

- [ ] `SGL`
- [ ] `SGL Samples`
</details>

#### SBL 6.01
<details>

- [ ] `SBL Libs`
  - [x] `blp`
  - [x] `cdc` **(copied from https://github.com/shicky256/sbl/tree/master/segalib/cdc)**
  - [x] `cmp`
  - [x] `csh`
  - [x] `dbg`
  - [x] `dma`
  - [x] `dsp` **TO BE VALIDATED**
  - [x] `gfs`
  - [x] `fld` **Converted from COFF**
  - [x] `int`
  - [x] `mem`
  - [x] `mpg`
  - [x] `mth` **(copied from https://github.com/shicky256/sbl/tree/master/segalib/mth)**
  - [ ] `pcm` **TO BE VALIDATED**
  - [x] `per` **(copied from https://github.com/shicky256/sbl/tree/master/segalib/per)**
  - [x] `scl` **(copied from https://github.com/shicky256/sbl/tree/master/segalib/scl)**
  - [ ] `snd` **TO BE VALIDATED**
  - [x] `spr` **(copied from https://github.com/shicky256/sbl/tree/master/segalib/spr)**
  - [x] `stm`
- [ ] `SBL Samples`
  - [x] `bup`
  - [x] `csh`
  - [x] `dbg`
    - [x] `smpdbg1`
  - [x] `dma`
    - [x] `dma0`
    - [x] `dma1`
    - [x] `dma2`
    - [x] `dma10`
    - [x] `dma12`
  - [ ] `dual` **(required or just sample code ?)**
  - [ ] `fld`
      - [ ] `smpfld01` **(Crash at runtime)**
  - [ ] `game` **(black screen at runtime)**
  - [ ] `game_cd` **(black screen at runtime)**
  - [ ] `gfs`
    - [ ] `smpgfs1a` **(Crash at runtime)**
    - [ ] `smpgfs1b` **(Crash at runtime)**
    - [ ] `smpgfs2` **(Crash at runtime)**
  - [ ] `mem`
    - [ ] `smpmem0` **(Crash at runtime)**
    - [?] `smpmem1` **(black screen at runtime)**
  - [ ] `mpg`
    - [ ] `smpmpg1`
  - [ ] `pcm`
    - [ ] `pcm2` **(Crash at runtime)**
    - [ ] `pcm3` **(black screen at runtime)**
    - [ ] `pcm4` **(black screen at runtime)**
    - [ ] `pcm5` **(Crash at runtime)**
    - [ ] `pcm13` **(black screen at runtime)**
  - [X] `per`
    - [X] `smpper0` **(no output)**
    - [X] `smpper1` **(no output)**
    - [X] `smpper7` **(controls ?)**
    - [X] `smpper8`
    - [X] `smpper12`
  - [ ] `sblsgl`
    - [ ] `sblsgl00` **(Link errors)**
    - [ ] `sblsgl01` **(Link errors)**
    - [ ] `sblsgl02` **(Link errors)**
    - [ ] `sblsgl03` **(Link errors)**
    - [ ] `sblsgl04` **(Link errors)**
  - [x] `scl`
      - [x] `smpscl1`
      - [x] `smpscl3`
      - [x] `smpscl5`
  - [ ] `sgl`
    - [x] `demo_a`
    - [x] `demo_b`
    - [x] `demo_c`
    - [X] `s_2_2`
    - [X] `s_3_2`
    - [X] `s_4_2`
    - [X] `s_4_3_1`
    - [X] `s_4_3_2`
    - [x] `s_4_3_3`
    - [x] `s_4_3_4`
    - [ ] `s_4_5` **(black screen at runtime)**
    - [ ] `s_5_2` **(black screen at runtime)**
    - [ ] `s_6_3` **(black screen at runtime)**
    - [x] `s_7_2`
    - [x] `s_7_4`
    - [ ] `s_7_6` **(black screen at runtime)**
  - [ ] `snd`
    - [ ] `smpsnd1` **(Runtime ???)**
    - [ ] `smpsnd4` **(Runtime ???)**
    - [ ] `smpsnd5` **(Runtime ???)**
    - [ ] `smpsnd6` **(Runtime ???)**
  - [x] `spr`
    - [x] `spr2`
    - [x] `spr7`
    - [x] `spr8`
  - [ ] `stm`
    - [ ] `smpstm1` **(Crash at runtime)**
    - [ ] `smpstm2` **(Crash at runtime)**
  - [ ] `sys` **(Not tested)**
  - [ ] `tim`
    - [ ] `smp0` **(Crash at runtime)**
    - [ ] `smp1` **(Crash at runtime)**
</details>

#### SDK Samples
<details>

- [x] `SDK Samples`
  - [x] `Basic`
  - [x] `Sprite`
  - [x] `Textured Sprite`
  - [ ] `CDBrowser`
</details>

#### [Jo Engine 2025.1](https://www.jo-engine.org/)
<details>

- [X] `Jo Engine`
- [X] `Jo Engine Samples`
  - [X] `demo1`
  - [X] `demo - homing laser`
  - [X] `demo - 2D storyboard`
  - [X] `demo - line scroll`
  - [X] `demo - 3D`
  - [X] `demo - map`
  - [X] `demo - 3D map`
  - [X] `demo - mode7`
  - [X] `demo - 3D - programmatically`
  - [X] `demo - mouse`
  - [X] `demo - multitap`
  - [X] `demo - 8bits tga`
  - [X] `demo - nbg2 font`
  - [X] `demo - advanced 3D`
  - [X] `demo - paint`
  - [X] `demo - audio`
  - [X] `demo - analog`
  - [X] `demo - printf`
  - [X] `demo - render to image`
  - [X] `demo - background`
  - [X] `demo - shooter`
  - [X] `demo - backup`
  - [X] `demo - simple water effect`
  - [X] `demo - bullet`
  - [X] `demo - software rendering`
  - [X] `demo - debug console`
  - [X] `demo - sonic`
  - [X] `demo - everydaycute`
  - [X] `demo - sprite animation`
  - [X] `demo - filesystem`
  - [X] `demo - tileset`
  - [X] `demo - gamepad`
  - [X] `demo - vdp2 plane`
  - [X] `demo - gouraud shading`
  - [x] `demo - video`
  - [X] `demo - hardcoded image`
  - [X] `demo - visual novel`
  - [X] `demo - voxel`
</details>

#### [Yaul Version 0.3.1](https://github.com/yaul-org/libyaul)
<details>

- [ ] `Yaul`
  - [X] `Yaul Library`
  - [ ] `Yaul Examples` (https://github.com/yaul-org/libyaul-examples)
    - [x] `arp-comm` << This example is disabled
    - [x] `bcl`
    - [x] `c++`
    - [x] `cd-block`
    - [x] `cpu-divu`
    - [x] `cpu-dmac`
    - [x] `cpu-dual`
    - [x] `cpu-frt`
    - [x] `cpu-wdt`
    - [ ] `dbgio-font`
    - [x] `dbgio-menu`
    - [x] `dbgio-usb-cart`
    - [x] `dma-queue`
    - [x] `dram-cart`
    - [x] `fibers`
    - [ ] `fileserver`
    - [x] `gdb`
    - [x] `mm-stats`
    - [ ] `netlink-template`
    - [x] `scu-dsp`
    - [ ] `scu-timers` << This example is disabled
    - [x] `smpc-rtc`
    - [x] `usb-cart`
    - [x] `vdp1-balls`
    - [x] `vdp1-drawing`
    - [x] `vdp1-interlace`
    - [x] `vdp1-mesh`
    - [x] `vdp1-mic3d`
    - [ ] `vdp1-normal-sprite`
    - [x] `vdp1-software-blending`
    - [x] `vdp1-st-niccc`
    - [x] `vdp1-uv-coords`
    - [x] `vdp1-zoom-sprite`
    - [x] `vdp2-24bpp-bitmap`
    - [x] `vdp2-all-nbgs`
    - [x] `vdp2-back-screen`
    - [x] `vdp2-effect-tunnel`
    - [x] `dp2-line-scroll`
    - [x] `vdp2-nbg0`
    - [x] `vdp2-normal-bitmap`
    - [x] `vdp2-rbg0-bitmap`
    - [x] `vdp2-rbg0`
    - [x] `vdp2-reduction-bitmap`
    - [x] `vdp2-special-function`
    - [x] `vdp2-zooming`
</details>

#### [SRL Version 0.9](https://github.com/ReyeMe/SaturnRingLib)
<details>

- [x] `SRL`
  - [x] `SRL Library` (Headers only, no static library)
  - [x] `SRL Samples` (https://github.com/ReyeMe/SaturnRingLib/tree/main/Samples)
    - [x] `CD - File reading`
    - [x] `Input - Analog`
    - [x] `Input - Gamepad`
    - [x] `Input - Gun`
    - [x] `Logs`
    - [x] `Makefile - pre and post build`
    - [x] `Math - Random`
    - [x] `Math - Random image`
    - [x] `SH2 - Slave`
    - [x] `SMPC - Clock`
    - [x] `Sound - CDDA`
    - [x] `Sound - PCM`
    - [x] `SRL - Event`
    - [x] `STL`
    - [x] `VDP1 - 3D - Animation`
    - [x] `VDP1 - 3D - Flat teapot`
    - [x] `VDP1 - 3D - Smooth teapot`
    - [x] `VDP1 - Clipping`
    - [x] `VDP1 - Distorted sprite`
    - [x] `VDP1 - Gouraud`
    - [x] `VDP1 - Interactive polygon`
    - [x] `VDP1 - Lines`
    - [x] `VDP1 - Sprite effects`
    - [x] `VDP1 - Sprites`
    - [x] `VDP2 - Bmp2Tile`
    - [x] `VDP2 - Layers`
    - [x] `VDP2 - RBG0 Rotation`
</details>

## Misc notes:

- [RetroReversing Guide](https://github.com/RetroReversing/retroReversing/tree/master/pages/sega/saturn)
- [Boost Preprocessor Introduction](https://www.boost.org/doc/libs/1_76_0/libs/preprocessor/doc/AppendixA-AnIntroductiontoPreprocessorMetaprogramming.html)
