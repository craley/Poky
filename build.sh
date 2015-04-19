#!/usr/bin/env sh

if [ ! -d /build ]
then
		mkdir -p build
fi

cd build
cmake ..
make
make install
