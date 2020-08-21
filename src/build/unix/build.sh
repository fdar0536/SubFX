#!/bin/sh

# get vcpkg
cd ../..
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg
chmod +x bootstrap-vcpkg.sh
./bootstrap-vcpkg.sh

# install libraries
vcpkg install \
    boost-regex[icu] boost-locale[icu] \
    python3[enable-shared] python3 \
    pybind11 nlohmann-json cxxopts

cd ..
mkdir build-unix
cd build-unix

install_prefix=""
if [[ -z ${PREFIX}]] then
    install_prefix="/usr/local"
else
    install_prefix="${PREFIX}"
fi

cmake -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_INSTALL_PREFIX=$install_prefix ..
make -j`getconf _NPROCESSORS_ONLN`
make install
echo "Done."
