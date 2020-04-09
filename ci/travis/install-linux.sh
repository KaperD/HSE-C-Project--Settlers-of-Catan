sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt install software-properties-common

sudo apt-get update

sudo apt-get install g++-7 > output
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 40
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 40

sudo apt-get install automake curl make unzip

sudo apt-get install build-essential autoconf libtool pkg-config

git clone -b v1.27.3 https://github.com/grpc/grpc
cd grpc
git submodule update --init

cd /home/travis/build/KaperD/HSE-C-Project--Settlers-of-Catan/grpc/third_party/protobuf
git submodule update --init --recursive
./autogen.sh

./configure
make -j8
make check -j8
sudo make install -j8
sudo ldconfig

cd /home/travis/build/KaperD/HSE-C-Project--Settlers-of-Catan/grpc/third_party/protobuf
make -j8
sudo make install -j8

cd /home/travis/build/KaperD/HSE-C-Project--Settlers-of-Catan/

# SDL2
sudo apt-get install libsdl2-dev -y
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-net-dev 
sudo apt-get install libsdl2-ttf-dev 
sudo apt-get install libsdl2-mixer-dev
