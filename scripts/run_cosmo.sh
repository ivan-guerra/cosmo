#!/bin/bash

# This script runs cosmo OS in the Bochs emulator. In order for this script
# to run successfully, the following preqrequisites must be met:
#   (1) Bochs must be installed on the host system.
#   (2) The cosmo OS ISO must have been generated using generate_iso.sh.
# The Bochs configuration file can be found under the scripts directory:
# cosmo/scripts/bochsrc.txt.

LGREEN='\033[1;32m'
LRED='\033[1;31m'
NC='\033[0m'

# Source the project configuration.
source config_cosmo.sh

# Verify Bochs has been installed on the system.
if ! command -v bochs &> /dev/null
then
    echo -e "${LRED}bochs could not be found.${NC}"
    echo -e "${LRED}Please install bochs before running the sim.${NC}"
    exit 1
fi

# Verify the caller has created the cosmo OS ISO.
if [ ! -f $COSMO_BIN_DIR/cosmo.iso ]
then
    echo -e "${LRED}cosmo.iso not found!${NC}"
    echo -e "${LRED}Did you run 'scripts/generate_iso.sh'?${NC}"
    exit 1
fi

# Verify the bochsrc.txt file is present.
if [ ! -f $COSMO_BOCHS_RC_FILE ]
then
    echo -e "${LRED}Cannot find bochsrc.txt!${NC}"
    echo -e "${LRED}Expected location is: $COSMO_BOCHS_RC_FILE${NC}"
    exit 1
fi

# Create the Bochs logging directory if it does not already exist.
if [ ! -d $COSMO_BOCHS_LOG_DIR ]
then
    mkdir -pv $COSMO_BOCHS_LOG_DIR
fi

# Run the Bochs emulator.
if bochs                                    \
    -f $COSMO_BOCHS_RC_FILE                 \
    -log $COSMO_BOCHS_LOG_DIR/bochslog.txt  \
    -q;
then
    echo -e "${LGREEN}Emulation was successful!${NC}"
    exit 0
else
    echo -e "${LRED}Unable to run bochs sim. See console messages above.${NC}"
    exit 1
fi
