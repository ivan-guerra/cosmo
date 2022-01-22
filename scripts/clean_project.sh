#!/bin/bash

# This script cleans up the source tree leaving it as if a fresh clone of
# the repository was made.

LGREEN='\033[1;32m'
NC='\033[0m'

# Source the project configuration.
source config_cosmo.sh

# Remove the binary directory.
if [ -d $COSMO_BIN_DIR ]
then
    echo -e "${LGREEN}Removing '$COSMO_BIN_DIR'${NC}"
    rm -r $COSMO_BIN_DIR
fi

# Remove the OS kernel ELF.
if [ -f $COSMO_ELF_FILE ]
then
    echo -e "${LGREEN}Removing '$COSMO_ELF_FILE'${NC}"
    rm $COSMO_ELF_FILE
fi

# Remove project docs.
if [ -d $COSMO_DOCS_DIR ]
then
    echo -e "${LGREEN}Removing '$COSMO_DOCS_DIR'${NC}"
    rm -r $COSMO_DOCS_DIR
fi

# Remove the CMake build directory.
if [ -d $COSMO_BUILD_DIR ]
then
    echo -e "${LGREEN}Removing '$COSMO_BUILD_DIR'${NC}"
    rm -r $COSMO_BUILD_DIR
fi

# Remove the Bochs logging directory.
if [ -d $COSMO_BOCHS_LOG_DIR ]
then
    echo -e "${LGREEN}Removing '$COSMO_BOCHS_LOG_DIR'${NC}"
    rm -r $COSMO_BOCHS_LOG_DIR
fi
