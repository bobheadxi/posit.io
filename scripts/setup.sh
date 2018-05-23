#!/bin/sh

echo "Seting up submodules..."
git submodule update --init --recursive vendor

BUILD_TOOLS_NOTICE="Warning: cmake and libsodium are required to build this library - please install it manually"

echo "Installing cmake and libsodium..."
if [[ "$OSTYPE" == "linux-gnu" ]]; then
    # Linux - @TODO
    echo $BUILD_TOOLS_NOTICE
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX
    brew install libsodium cmake
elif [[ "$OSTYPE" == "win32" ]]; then
    # Windows - @TODO
    echo $BUILD_TOOLS_NOTICE
else
    echo $BUILD_TOOLS_NOTICE
fi
