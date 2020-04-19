#!/bin/bash

# Linux
sudo rm -rf bin build
make
sudo chown soren:soren bin
mv bin linux

# Windows
# docker pull multiarch/crossbuild
sudo rm -rf bin build
docker run -it --rm -v $(pwd):/workdir -e CROSS_TRIPLE=x86_64-w64-mingw32  multiarch/crossbuild make WIN32=true
sudo chown soren:soren bin
mv bin win64

# MacOS
# docker pull multiarch/crossbuild
sudo rm -rf bin build
docker run -it --rm -v $(pwd):/workdir -e CROSS_TRIPLE=x86_64-apple-darwin  multiarch/crossbuild make MACOS=true CXX=c++ CXXFLAGS=-stdlib=libc++
sudo chown soren:soren bin
mv bin macos

# Clean up and zip
zip release.zip -r linux win64 macos
sudo rm -rf bin build linux win64 macos
