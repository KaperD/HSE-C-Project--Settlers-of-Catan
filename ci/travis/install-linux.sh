#!/bin/bash
echo toolchain

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt install software-properties-common

sudo apt-get update

sudo apt-get install g++-7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 40
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 40

sudo apt-get install automake curl make unzip

sudo apt-get install build-essential autoconf libtool pkg-config

# SDL2
sudo apt-get install libsdl2-dev -y
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-net-dev 
sudo apt-get install libsdl2-ttf-dev 
sudo apt-get install libsdl2-mixer-dev