#!/bin/bash
set -euo pipefail

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set -x
fi

: "${SATURN_TMP:?SATURN_TMP is not set}"
: "${UNAME:?UNAME is not set}"

SCRIPT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"

run_as_user() {
	su -p -s /bin/bash "$UNAME" -c "$*"
}

# Setup
run_as_user "bash \"$SCRIPT_DIR/setup-git.sh\""

# Install mkisofs
run_as_user "bash \"$SCRIPT_DIR/build-Mkisofs.sh\""

# Install Saturn-SDK-Tool-IPMaker 0.2
run_as_user "bash \"$SCRIPT_DIR/build-IPMaker.sh\" \"IPMaker_0.2\""

# Install Boost preprocessor, System, filesystem, program_options 1.78.0
bash "$SCRIPT_DIR/build-Boost.sh" "boost-1.78.0"

# Install gdown https://pypi.org/project/gdown/
if ! command -v gdown >/dev/null 2>&1; then
	pipx install gdown
	echo -e '#!/bin/bash\npipx run gdown "$@"' > /usr/bin/gdown
	chmod +x /usr/bin/gdown
fi

# Install CueMaker 1.0
run_as_user "bash \"$SCRIPT_DIR/build-CueMaker.sh\" \"CueMaker_1.0\""

# Install satconv
run_as_user "bash \"$SCRIPT_DIR/build-satconv.sh\""

# Install ftx
if [ -n "${FTX_TAG:-}" ]; then
	run_as_user "bash \"$SCRIPT_DIR/build-ftx.sh\" \"$FTX_TAG\""
else
	run_as_user "bash \"$SCRIPT_DIR/build-ftx.sh\""
fi

# Install SCU DSP Compiler
run_as_user "bash \"$SCRIPT_DIR/build-scu-dsp-asm.sh\""

# Install Mednafen (for testing/emulation)
run_as_user "bash \"$SCRIPT_DIR/build-mednafen.sh\""

if [ "${DOCKER_BUILDKIT:-}" == "1" ]; then
	set +x
fi

exit 0
