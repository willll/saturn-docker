#!/bin/bash
set -e

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

build () {
	echo "BUILDING $1"
	# -j1: some Makefiles have rules with two targets (dbgio-font: "work/mrc_squa.cpd work/mrc_squa.pal:"), run twice at
	# the same time in parallel, one deleting the file the other reads. The examples are small: serial costs little
	SILENT=1 make -C $1 clean -j1
	make -C $1 all -j1
}

if [[ $INSTALL_YAUL_SAMPLES -eq 1 ]]; then

	if [ ! -d $SATURN_YAUL ]; then
		echo "$SATURN_YAUL does not exist !"
		exit 1
	fi

	git clone -n https://github.com/yaul-org/libyaul-examples.git "$SATURN_YAUL/examples"
	pushd "$SATURN_YAUL/examples"
  	git checkout $YAUL_EXAMPLES_COMMIT_SHA
  	popd

	cd $SATURN_YAUL/examples

	# Not built: fileserver and scu-timers are disabled upstream ($(error This example is disabled)),
	# netlink-template (no yaul.h with its netlink specs) and overlay (fix16.h errors with its specs) don't compile

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

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
