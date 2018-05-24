#!/bin/sh

echo "Seting up submodules..."
git submodule update --init --recursive

BUILD_TOOLS_NOTICE="Warning: cmake and libsodium are required to build this library - please install it manually"

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    # Linux
    echo "Running linux-gnu libsodium install instructions..."
    wget https://download.libsodium.org/libsodium/releases/LATEST.tar.gz -O /tmp/libsodium.tar.gz
    pushd .
    cd /tmp
    tar -zxvf /tmp/libsodium.tar.gz
    cd libsodium-stable
    ./configure
    make && make check
    sudo make install
    popd
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX
    echo "Installing libsodium and cmake via Homebrew..."
    brew install libsodium cmake
elif [[ "$OSTYPE" == "win32" ]]; then
    # Windows - @TODO
    echo $BUILD_TOOLS_NOTICE
else
    echo $BUILD_TOOLS_NOTICE
fi
