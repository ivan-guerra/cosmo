#!/bin/bash

# This script configures the default search paths for many of the binaries
# and configuration files used by the project. Other scripts may source this
# file to find the resources that they need.

CWD=$(pwd)

# Root directory.
COSMO_PROJECT_PATH=$(dirname ${CWD})

# Scripts directory.
COSMO_SCRIPTS_PATH="${COSMO_PROJECT_PATH}/scripts"

# Binary directory.
COSMO_BIN_DIR="${COSMO_PROJECT_PATH}/bin"

# Location of kernel ELF file.
COSMO_ELF_FILE="${COSMO_PROJECT_PATH}/iso/boot/kernel.elf"

# Doxygen output directory.
COSMO_DOCS_DIR="${COSMO_PROJECT_PATH}/docs/cosmo_docs"

# CMake build files (see build_cosmo.sh for more info).
COSMO_BUILD_DIR="${COSMO_PROJECT_PATH}/build"

# Bochs configuration file.
COSMO_BOCHS_RC_FILE="${COSMO_SCRIPTS_PATH}/bochsrc.txt"

# Bochs logging directory.
COSMO_BOCHS_LOG_DIR="${COSMO_SCRIPTS_PATH}/bochs_logs"
