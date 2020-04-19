#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

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

BUILD_CXX_FLAGS += 
BUILD_CXX_FLAGS += 

# --------------------------------------------------------------
# Enable all possible plugin types

ifneq ($(MACOS_OR_WINDOWS),true)
TARGETS += ladspa
TARGETS += dssi
endif

TARGETS += lv2_dsp
TARGETS += vst

# Defines for bundling a VST plugin for MACOS
BIN_DIR = ../../bin
DPF_DIR =../../dpf
VST_DIR = $(BIN_DIR)/$(NAME).vst

all: $(TARGETS)
ifeq ($(MACOS),true)
	cp -r $(DPF_DIR)/utils/plugin.vst/ $(VST_DIR)
	mv $(BIN_DIR)/$(NAME)-vst.dylib $(VST_DIR)/Contents/MacOS/$(NAME)
	rm -f $(VST_DIR)/Contents/MacOS/deleteme
	sed -i -e "s/X-PROJECTNAME-X/$(NAME)/" $(VST_DIR)/Contents/Info.plist
	rm -f $(VST_DIR)/Contents/Info.plist-e
endif

# --------------------------------------------------------------
