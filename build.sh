#!/bin/sh

cd $TRAVIS_BUILD_DIR
mkdir bin
mkdir obj
cd obj
mkdir hexconv
mkdir usim

cd $TRAVIS_BUILD_DIR/src
make
