#!/bin/bash
set -e

build () {
	echo "BUILDING $1"
	SILENT=1 make -C $1 clean $MAKEFLAGS
	make -C $1 all $MAKEFLAGS
}

if [ $INSTALL_YAUL_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_YAUL ]; then
		echo "$SATURN_YAUL does not exist !"
		exit 1
	fi

	git clone -n https://github.com/yaul-org/libyaul-examples.git "$SATURN_YAUL/examples"
	pushd "$SATURN_YAUL/examples"
  	git checkout $YAUL_EXAMPLES_COMMIT_SHA
  	popd

	cd $SATURN_YAUL/examples

	EXAMPLES=(
						arp-comm
						bcl
						c++
						cd-block
						cpu-divu
						cpu-dmac
						cpu-dual
						cpu-frt
						cpu-wdt
						dbgio-font
						dbgio-menu
						dbgio-usb-cart
						dma-queue
						dram-cart
						fibers
						#fileserver << This example is disabled\
						gamemath
						gdb
						mm-stats
						#netlink-template fatal error: sys/init.h: No such file or directory
						#overlay : missing files ?
						scu-dsp
						#scu-timers << This example is disabled\
						smpc-rtc
						usb-cart
						vdp1-balls
						vdp1-drawing
						vdp1-interlace
						vdp1-mesh
						vdp1-mic3d
						vdp1-software-blending
						vdp1-st-niccc
						vdp1-uv-coords
						vdp1-zoom-sprite
						vdp2-24bpp-bitmap
						vdp2-all-nbgs
						vdp2-back-screen
						vdp2-effect-tunnel
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

	# Disabled compos as they need to be updated to the current YAUL version
	#git clone --depth 1 https://github.com/ijacquez/saturn-compos.git "$SATURN_YAUL/compos"

	#cd "$SATURN_YAUL/compos"
	#echo "BUILDING compos"
	#SILENT=1 make clean
	#SILENT=1 make

else
	echo "$(tput setaf 1)No YAUL examples will be built$(tput sgr 0)"

fi

exit 0
