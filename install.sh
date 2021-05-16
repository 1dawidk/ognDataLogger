#!/bin/bash

# Exit if any subprogram exits with an error
set -e

echo "Install required dependencies"
apt-get update
apt install libeigen3-dev libpng++-dev libboost-all-dev libpng-dev git cmake

#Build ogn library
echo "Build ogn library..."

cd ./cpp-lib/
./scripts/build.sh
cd ..

mkdir ./build
cd ./build
cmake ..
make


# edit the file /etc/locale.gen
# uncomment the line:
# en_US.UTF-8 UTF-8
# finally run the command "sudo locale-gen"
