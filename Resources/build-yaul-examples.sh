#!/bin/bash
set -e

build () {
	echo "BUILDING $1"
	cd $SATURN_YAUL/examples/$1
	SILENT=1 make clean
	SILENT=1 make
}

if [ $INSTALL_YAUL_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_YAUL ]; then
		echo "$SATURN_YAUL does not exist !"
		exit 1
	fi

	git clone --depth 1 https://github.com/ijacquez/libyaul-examples.git "$SATURN_YAUL/examples"

	cd $SATURN_YAUL/examples

	EXAMPLES=(
						#arp-comm << This example is disabled
						bcl
						#c++
						cd-block
						cpu-divu
						cpu-dmac
						cpu-dual
						cpu-frt
						cpu-wdt
						dbgio-menu
						dbgio-usb-cart
						#dma-queue
						dram-cart
						#fileserver
						#gdb
						scu-dsp
						#scu-timers << This example is disabled
						#usb-cart
						vdp1-balls
						vdp1-drawing
						vdp1-interlace
						vdp1-mic3d
						vdp1-sega3d
						vdp1-software-blending
						vdp1-st-niccc
						vdp1-uv-coords
						vdp1-zoom-sprite
						vdp2-24bpp-bitmap
						vdp2-2x2-plane
						vdp2-back-screen
						vdp2-line-scroll
						vdp2-nbg0
						vdp2-normal-bitmap
						vdp2-rbg0-bitmap
						vdp2-rbg0
						vdp2-reduction-bitmap
						vdp2-special-function
						vdp2-zooming
						)

	#
	#
	#
	for example in "${EXAMPLES[@]}"
	do
		build $example
	done

fi

exit 0
