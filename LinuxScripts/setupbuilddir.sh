#!/bin/bash
#Setup the build directory structure execute from the Linuxscript folder

DEBUG="debug"
INSTALL="install"
RELEASE="release"
TESTS="Tests"

cd ..
mkdir -p build
cd build
mkdir -p $DEBUG $INSTALL $RELEASE $TESTS $INSTALL/Images $INSTALL/NeuralNet $INSTALL/Settings

current_dir=$(pwd)
export LD_LIBRARY_PATH=$current_dir/$INSTALL:${LD_LIBRARY_PATH}

cd ../src/
for d in */ ; do
    mkdir -p ../build/$DEBUG/$d
done
rm -rf ../build/$DEBUG/Tests

for d in */ ; do
    mkdir -p ../build/$RELEASE/$d
done
rm -rf ../build/$RELEASE/Tests

cd $TESTS
for d in */ ; do
    mkdir -p ../../build/$TESTS/$d
done


