#!/bin/bash

echo "Install required dependencies"
apt-get update
apt install libeigen3-dev libpng++-dev libboost-all-dev git cmake

#Build misc library
echo "Build ogn library..."

cd ./cpp-lib/prj
ln -s def.compiler.gcc-5-ubuntu-14 def.compiler
ln -s def.platform.linux def.platform
make

cd ../..
mkdir ./build
cd ./build
cmake ..
make
