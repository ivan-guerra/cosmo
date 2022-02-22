#!/bin/bash

# This script generates the cosmo OS ISO image. If successful, the ISO is
# installed to cosmo/bin/.

LGREEN='\033[1;32m'
LRED='\033[1;31m'
NC='\033[0m'

# Source the project configuration.
source config_cosmo.sh

# Verify grub-mkrescue is installed.
COSMO_GRUB_MKRESCUE=""
if command -v grub-mkrescue &> /dev/null
then
    COSMO_GRUB_MKRESCUE="grub-mkrescue"
elif command -v grub2-mkrescue &> /dev/null
then
    COSMO_GRUB_MKRESCUE="grub2-mkrescue"
fi

# grub-mkrescue could not be found, let the User know.
if [ -z "${COSMO_GRUB_MKRESCUE}" ]
then
    echo -e "${LRED}grub-mkrescue could not be found.${NC}"
    echo -e "${LRED}Please install grub to build the OS image.${NC}"
    exit 1
fi

if ! command -v xorriso &> /dev/null
then
    echo -e "${LRED}xorriso could not be found.${NC}"
    echo -e "${LRED}Please install xorriso to build the OS image.${NC}"
    exit 1
fi

if ! command -v mtools &> /dev/null
then
    echo -e "${LRED}GNU mtools could not be found.${NC}"
    echo -e "${LRED}Please install GNU mtools to build the OS image.${NC}"
    exit 1
fi

# Verify the caller has previously built and installed the kernel.
if [ ! -f $COSMO_ELF_FILE ]
then
    echo -e "${LRED}kernel.elf not detected!${NC}"
    echo -e "${LRED}Did you forget to run build_cosmo.sh?${NC}"
    exit 1
else
    echo -e "${LGREEN}kernel.elf found at '$COSMO_ELF_FILE'${NC}"
fi

# If the bin directory has not already been created, make it.
if [ ! -d $COSMO_BIN_DIR ];
then
    mkdir -pv $COSMO_BIN_DIR
fi

# Run grub-mkrescue on the contents of cosmo/iso. The output is the ISO image
# cosmo/bin/cosmo.iso.
if ${COSMO_GRUB_MKRESCUE} \
    -o ../bin/cosmo.iso   \
    ../iso;
then
    echo -e "${LGREEN}cosmo.iso successfully created. See ${COSMO_BIN_DIR}/cosmo.iso.${NC}"
    exit 0
else
    echo -e "${LRED}Unable to create cosmo.iso. See console messages above.${NC}"
    exit 1
fi
