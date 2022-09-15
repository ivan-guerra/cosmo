# Create a cosmo developer's image. This Dockerfile will build a Fedora image
# which includes an i686 GCC cross compiler alongside additional tools needed
# to build and run cosmo. See the project README.md for instructions on
# intended usage.
#
# Credit to the author of ktool from which this script was taken nearly
# verbatim: https://github.com/mikerowehl/ktool/blob/master/Dockerfile

FROM fedora:latest

# Install all the packages needed to build and run cosmo in an emulator.
RUN dnf groupinstall 'Development Tools' -y --nodocs && \
    dnf install -y --nodocs \
        bochs             \
        cmake             \
        grub2             \
        grub2-tools-extra \
        xorriso           \
        mtools            \
        doxygen           \
        curl              \
        bison             \
        flex              \
        gmp               \
        gmp-c++           \
        gmp-devel         \
        mpc               \
        libmpc-devel      \
        mpfr              \
        mpfr-devel        \
        gcc-c++           \
        nasm              \
        texinfo

# Update these to the latest binutils/GCC available. The below versions were
# used when testing cosmo.
ARG BINUTILS_VER=2.36
ARG GCC_VER=11.1.0

ENV PREFIX /opt/cross
ENV TARGET i686-elf
ENV PATH $PREFIX/bin:$PATH

# Build the GCC cross compiler. The commands that follow were taken from
# the OSDev GCC Cross Compiler page: https://wiki.osdev.org/GCC_Cross-Compiler
RUN mkdir -p $HOME/src                                                            && \
  cd $HOME/src                                                                    && \
  curl -s -O https://ftp.gnu.org/gnu/binutils/binutils-"${BINUTILS_VER}".tar.gz   && \
  tar zxvf binutils-"${BINUTILS_VER}".tar.gz                                      && \
  curl -s -O https://ftp.gnu.org/gnu/gcc/gcc-"${GCC_VER}"/gcc-"${GCC_VER}".tar.gz && \
  tar zxvf gcc-"${GCC_VER}".tar.gz                                                && \
  mkdir build-binutils                                                            && \
  cd build-binutils                                                               && \
  ../binutils-"${BINUTILS_VER}"/configure --target=$TARGET --prefix="$PREFIX"        \
    --with-sysroot --disable-nls --disable-werror                                 && \
  make -j$(nproc)                                                                 && \
  make install                                                                    && \
  cd $HOME/src                                                                    && \
  mkdir build-gcc                                                                 && \
  cd build-gcc                                                                    && \
  ../gcc-"${GCC_VER}"/configure --target=$TARGET --prefix="$PREFIX"                  \
    --disable-nls --enable-languages=c,c++ --without-headers                      && \
  make -j$(nproc) all-gcc                                                         && \
  make -j$(nproc) all-target-libgcc                                               && \
  make -j$(nproc) install-gcc                                                     && \
  make -j$(nproc) install-target-libgcc                                           && \
  cd $HOME                                                                        && \
  rm -rf $HOME/src

WORKDIR /cosmo/scripts
