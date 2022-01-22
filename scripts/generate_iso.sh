#!/bin/bash

# This script generates the cosmo OS ISO image. If successful, the ISO is
# installed to cosmo/bin/.

LGREEN='\033[1;32m'
LRED='\033[1;31m'
NC='\033[0m'

# Source the project configuration.
source config_cosmo.sh

# Verify genisoimage has been installed on the system.
if ! command -v genisoimage &> /dev/null
then
    echo -e "${LRED}genisoimage could not be found.${NC}"
    echo -e "${LRED}Please install genisoimage to build the OS image.${NC}"
    exit 1
fi

# Verify the caller has previously built and installed the kernel.
if [ ! -f $COSMO_ELF_FILE ]
then
    echo -e "${LRED}kernel.elf not detected!${NC}"
    echo -e "${LRED}Did you build AND install using Make?${NC}"
    exit 1
else
    echo -e "${LGREEN}kernel.elf found at '$COSMO_ELF_FILE'${NC}"
fi

# If the bin directory has not already been created, make it.
if [ ! -d $COSMO_BIN_DIR ];
then
    mkdir -pv $COSMO_BIN_DIR
fi

# Run the genisoimage on the contents of cosmo/iso. The output is the ISO image
# cosmo/bin/cosmo.iso.
if genisoimage -R                   \
    -b boot/grub/stage2_eltorito    \
    -no-emul-boot                   \
    -boot-load-size 4               \
    -A cosmo                        \
    -input-charset utf8             \
    -quiet                          \
    -boot-info-table                \
    -o $COSMO_BIN_DIR/cosmo.iso     \
    ../iso;
then
    echo -e "${LGREEN}cosmo.iso successfully created. See ${COSMO_BIN_DIR}/cosmo.iso.${NC}"
    exit 0
else
    echo -e "${LRED}Unable to create cosmo.iso. See console messages above.${NC}"
    exit 1
fi
