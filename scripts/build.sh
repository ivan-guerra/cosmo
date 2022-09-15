#!/bin/bash

# This script builds the cosmo kernel ELF. build_cosmo.sh can process a number
# of commandline parameters. Run 'build.sh -h' to see all the options.

# Print a help message to the console.
Help()
{
    echo "Build the cosmo OS kernel ELF."
    echo
    echo "usage: build_cosmo.sh [d|h]"
    echo "options:"
    echo "d    Build project documentation (default OFF)."
    echo "h    Print this help message."
}

BUILD_DOC="OFF"

while getopts ":hd" flag
do
    case "${flag}" in
        d) BUILD_DOC="ON";;
        h) Help
           exit;;
       \?) echo "Error: Invalid option"
           Help
           exit;;
    esac
done

# Source the project configuration.
source config.sh

# Create the build directory if it does not already exist.
if [ ! -d $COSMO_BUILD_DIR ]
then
    mkdir -pv $COSMO_BUILD_DIR
fi

# Build and install the kernel.
pushd $COSMO_BUILD_DIR
    cmake                                                     \
        -DCMAKE_TOOLCHAIN_FILE=${COSMO_PROJECT_PATH}/cmake/i686-elf-gcc.cmake    \
        -DBUILD_DOC=${BUILD_DOC} ../                       && \
    make all                                               &&
    make install

    # Exit if any of the above commands fails.
    if [ $? -ne 0 ];
    then
        exit 1
    fi
popd

# Generate the OS ISO image. Exit if generate_iso.sh reports failure.
if ! ./generate_iso.sh;
then
    exit 1
fi
