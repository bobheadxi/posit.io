#!/bin/sh

echo "Seting up submodules..."
git submodule update --init --recursive vendor

echo "Installing libsodium..."
if [[ "$OSTYPE" == "linux-gnu" ]]; then
    # Linux - @TODO
    echo "Please find out how to install libsodium!"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX
    brew install libsodium
elif [[ "$OSTYPE" == "win32" ]]; then
    # Windows - @TODO
    echo "Please find out how to install libsodium!"
else
    echo "Please find out how to install libsodium!"
fi
