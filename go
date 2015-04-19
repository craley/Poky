#!/bin/bash

#create build dir if not present
if [ ! -d thirdparty/gwen/build ]
then
		mkdir -p build
fi

#compile third party
cd thirdparty/gwen/build
cmake ..
make
make install

#move back to base dir
cd ../../../
#create proj build if not present
if [ ! -d build ]
then
		mkdir -p build
fi

#compile project
cd build
cmake ..
make
make install
