#!/bin/bash
set -e

VERBOSE=""

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set -x
	$VERBOSE="VERBOSE=1"
fi

if [ $INSTALL_JO_ENGINE_SAMPLES -eq 1 ]; then

	if [ ! -d $SATURN_JOENGINE ]; then
		echo "$SATURN_JOENGINE does not exist !"
		exit 1
	fi

	cd $SATURN_JOENGINE/Samples

	for f in *\ *; do mv "$f" "${f// /_}"; done

	SAMPLES=(
					'demo1'
					'demo_-_2D_storyboard'
					'demo_-_3D_-_programmatically'
					'demo_-_3D_map'
					'demo_-_3D'
					'demo_-_8bits_tga'
					'demo_-_advanced_3D'
					'demo_-_analog_-_mission_stick'
					'demo_-_analog_-_wheel'
					'demo_-_analog'
					'demo_-_audio'
					'demo_-_background'
					'demo_-_backup'
					'demo_-_bullet'
					'demo_-_debug_console'
					'demo_-_everydaycute'
					'demo_-_filesystem'
					'demo_-_gamepad'
					'demo_-_gouraud_shading'
					'demo_-_hardcoded_image'
					'demo_-_homing_laser'
					'demo_-_line_scroll'
					'demo_-_map'
					'demo_-_mode7'
					'demo_-_mouse'
					'demo_-_multitap'
					'demo_-_nbg2_font'
					'demo_-_paint'
					'demo_-_printf'
					'demo_-_render_to_image'
					'demo_-_shooter'
					'demo_-_simple_water_effect'
					'demo_-_software_rendering'
					'demo_-_sonic'
					'demo_-_sprite_animation'
					'demo_-_tileset'
					'demo_-_vdp2_map'
					'demo_-_vdp2_plane'
					'demo_-_video'
					'demo_-_visual_novel'
					'demo_-_voxel'
						)

	#
	#
	#
	for sample in ${SAMPLES[@]}
	do
		echo "BUILDING $sample"
		cd "$SATURN_JOENGINE/Samples/$sample"
		make $VERBOSE clean || exit 1
		make $VERBOSE $MAKEFLAGS || exit 1
	done

else
	echo "$(tput setaf 1)No joengine samples will be built$(tput sgr 0)"

fi

if [ "$DOCKER_BUILDKIT" == "1" ]; then
	set +x
fi

exit 0
