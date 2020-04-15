#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

NOISEMAKER_PATH = ../../distrho-ports/ports/tal-noisemaker/source

# --------------------------------------------------------------
# Project name, used for binaries

NAME = reverse-camel

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	ReverseCamel.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Set include paths

BUILD_CXX_FLAGS += -I $(NOISEMAKER_PATH)
BUILD_CXX_FLAGS += -I $(NOISEMAKER_PATH)/Engine/

# --------------------------------------------------------------
# Enable all possible plugin types

ifneq ($(MACOS_OR_WINDOWS),true)
TARGETS += ladspa
TARGETS += dssi
endif

TARGETS += lv2_dsp
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
