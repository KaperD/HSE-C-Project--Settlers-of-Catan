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
 
# Build and install protobuf
cd ./third_party/protobuf
./autogen.sh
./configure --prefix=/usr/local
make -j10
sudo make install
sudo ldconfig
  
# Build and install gRPC
cd ../..
make -j10 PROTOC=/usr/local/bin/protoc 
sudo make prefix=/usr install

cd /home/travis/build/KaperD/HSE-C-Project--Settlers-of-Catan/

# SDL2
sudo apt-get install libsdl2-dev -y
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-net-dev 
sudo apt-get install libsdl2-ttf-dev 
sudo apt-get install libsdl2-mixer-dev

protoc --version