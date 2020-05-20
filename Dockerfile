FROM ubuntu:18.04

ENV PATH=/usr/local/n64chain/bin:${PATH}
ENV N64_TOOLCHAIN=/usr/local/n64chain/bin
ENV ROOT=/etc/n64

WORKDIR /usr/local

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get -y install build-essential git wget libmpfr-dev libmpc-dev libgmp-dev flex bison && \
    apt-get clean && \
    git clone https://github.com/CrashOveride95/n64chain.git ./n64chain && \
    cd n64chain && \
    ./build-posix64-toolchain.sh && \
    mkdir -p gcc-source/libgcc/config/mips && \
    touch gcc-source/libgcc/config/mips/t-mips64 && \
    cd gcc-build && \
    make all-target-libgcc CC_FOR_TARGET=$N64_TOOLCHAIN/mips64-elf-gcc CFLAGS_FOR_TARGET="-D_MIPS_SZLONG=32 -D_MIPS_SZINT=32 -mabi=32 -march=vr4300 -mtune=vr4300 -mfix4300" && \
    make install-target-libgcc && \
    echo "deb [trusted=yes] https://coneyislanddiscopalace.xyz/apt ./" > /etc/apt/sources.list.d/coneyisland.list && \
    apt update && \
    apt-get -y install n64sdk && \
    apt-get -y install spicy && \
    apt-get -y install makemask && \
    apt-get -y install root-compatibility-enviroment
