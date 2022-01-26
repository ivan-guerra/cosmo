#!/bin/bash

# This script will install the GNU cross compilation toolchain for the User
# specified TARGET. Fill out the following global variables before running
# the script:
#   GCC_VER        : GCC version.
#   BINUTILS_VER   : Binutils version.
#   TARGET         : Target machine (e.g., i686, i386, etc.).
#   CC_INSTALL_DIR : Directory where the CC toolchain will be installed.
#   CC_SRC_DIR     : The temporary directory that will hold the source/build
#                    artifacts.
# This script implements the steps in https://wiki.osdev.org/GCC_Cross-Compiler
# The meat of this script was taken from mikerowehl's Dockerfile:
# https://github.com/mikerowehl/ktool/blob/master/Dockerfile

LGREEN='\033[1;32m'
LRED='\033[1;31m'
NC='\033[0m'

# Define desired GCC and Binutils versions.
GCC_VER="11.1.0"
BINUTILS_VER="2.36"

# Define the target machine.
TARGET="i686-elf"

# Define the installation directory and configure script prefix.
CC_INSTALL_DIR="$HOME/opt"
PREFIX="${CC_INSTALL_DIR}/cross"

# Create the cross compiler source directory.
CC_SRC_DIR="${CC_INSTALL_DIR}/src"
if [ ! -d ${CC_SRC_DIR} ]
then
    mkdir -p ${CC_SRC_DIR}
fi

# Download and unarchive parameter source files.
#   $1 URL to source tar ball.
#   $2 Utility name including version number (e.g., binutils-X.Y.Z).
DownloadAndUnarchive()
{
    pushd ${CC_SRC_DIR}
    echo -e "Downloading  and unarchiving $2..."

    # Fetch the source code archive and unarchive it.
    curl -s  -O $1 && \
    tar zxf $2.tar.gz

    if [ $? -ne 0 ]
    then
        echo -e "${LRED}Failed to checkout $2.${NC}"
        echo -e "${LRED}See console log for details.${NC}"
    else
        echo -e "${LGREEN}Checked out latest $2 successfully.${NC}"
    fi
    popd

    return $?
}

# Build binutils for the target.
BuildBinUtils()
{
    pushd ${CC_SRC_DIR}
    if [ ! -d build-binutils ]
    then
        mkdir build-binutils
    fi

    pushd build-binutils
    ../binutils-${BINUTILS_VER}/configure \
        --target=$TARGET                  \
        --prefix="$PREFIX"                \
        --with-sysroot                    \
        --disable-nls                     \
        --disable-werror               && \
    make -j$(nproc)                    && \
    make -j$(nproc) install

    if [ $? -ne 0 ]
    then
        echo -e "${LRED}Failed to build binutils-${BINUTILS_VER}.${NC}"
        echo -e "${LRED}See console log for details.${NC}"
    else
        echo -e "${LGREEN}binutils-${BINUTILS_VER} built successfully.${NC}"
    fi

    popd # Pop out of build-binutils.
    popd # Pop out of ${CC_SRC_DIR}.

    return $?
}

# Build GCC for the target.
BuildGCC()
{
    pushd ${CC_SRC_DIR}
    if [ ! -d build-gcc ]
    then
        mkdir -p build-gcc
    fi

    pushd build-gcc
    ../gcc-${GCC_VER}/configure              \
        --target=$TARGET                     \
        --prefix="$PREFIX"                   \
        --disable-nls                        \
        --enable-languages=c,c++             \
        --without-headers                 && \
    make -j$(nproc) all-gcc               && \
    make -j$(nproc) all-target-libgcc     && \
    make -j$(nproc) install-gcc           && \
    make -j$(nproc) install-target-libgcc

    if [ $? -ne 0 ]
    then
        echo -e "${LRED}Failed to build gcc-${GCC_VER}.${NC}"
        echo -e "${LRED}See console log for details.${NC}"
    else
        echo -e "${LGREEN}gcc-${GCC_VER} built successfully.${NC}"
    fi

    popd # Pop out of build-gcc.
    popd # Pop out of ${CC_SRC_DIR}.

    return $?
}

# Execute four commands in sequence. If any one of them fails, an error message
# will be printed by the failing function and the error code will be returned
# to the shell.
DownloadAndUnarchive                                                    \
    https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz    \
    binutils-${BINUTILS_VER}                                         && \
DownloadAndUnarchive                                                    \
    https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz    \
    gcc-${GCC_VER}                                                   && \
BuildBinUtils                                                        && \
BuildGCC

# Blow away source code and intermediate build files.
rm -rf ${CC_SRC_DIR}

# Drop into the install directory so you can see what all was installed.
cd ${CC_INSTALL_DIR}
