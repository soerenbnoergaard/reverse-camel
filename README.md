# Reverse Camel

Clone of the amazing Camel Crusher based on black-box analysis. The reason for doing this is to get a simple Linux version so Wine is not required to get some of the great camel's tube-like distortion.

# Download

Release version can be downloaded from the [Github releases page](https://github.com/soerenbnoergaard/reverse-camel/releases).

# Building

On Linux for Linux:

    make

On OSX for OSX:

    brew install pkg-config
    make

On Linux for Windows:

    sudo apt install mingw-w64
    make WIN32=true CC=x86_64-w64-mingw32-gcc CXX=x86_64-w64-mingw32-g++

