
# System specifications
F_CRYSTAL      = 8000000L
F_CPU          = 168000000L
SYSCLOCK       = SYSCLK_FREQ_168MHz
FAMILY         = f4xx
# USB            = enabled
# EXTRA_DEFINES  = -DSTM32F446xx

APPLICATION_LARGE = TRUE
BOOTLOADER        = waves2_bootloader

# Preferred upload command
UPLOAD_COMMAND  = upload_combo_jtag_erase_first

# Packages to build
TARGET         = waves2
PACKAGES       = waves2 \
		waves2/drivers \
		waves2/dsp \
		waves2/dsp/engine \
		stmlib/dsp \
		stmlib/utils \
		stmlib/system

RESOURCES      = waves2/resources

TOOLCHAIN_PATH ?= /usr/local/arm-4.8.3/

include stmlib/makefile.inc

# Rule for building the firmware update file
wav:  $(TARGET_BIN)
	python stm_audio_bootloader/qpsk/encoder.py \
		-t stm32f4 -s 48000 -b 12000 -c 6000 -p 256 \
		$(TARGET_BIN)
