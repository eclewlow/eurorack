
# System specifications
F_CRYSTAL      = 8000000L
F_CPU          = 168000000L
SYSCLOCK       = SYSCLK_FREQ_168MHz
FAMILY         = f4xx
# USB            = enabled

# Preferred upload command
UPLOAD_COMMAND  = upload_jtag_erase_first

# Packages to build
TARGET         = waves2_bootloader
PACKAGES       = waves2/bootloader \
		waves2/drivers \
		stmlib/dsp \
		stmlib/utils \
		stmlib/system

TOOLCHAIN_PATH ?= /usr/local/arm-4.8.3/

include stmlib/makefile.inc
