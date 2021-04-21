#!/bin/bash

sudo rm -rf build
mkdir build
cd build
cmake ..
make

sudo rm /usr/local/lib/libbtbb.so.1.0
sudo rm /usr/local/lib/libbtbb.so.1
sudo rm /usr/local/lib/libbtbb.so
sudo rm /usr/local/include/btbb.h

sudo make install

