# dsptk
DSP Toolkit is a collection of Digital Signal Processing classes that I'm using to learn the subject.
Already exists a lot of open source libraries so this work may not be useful for anyone except for me.

# Build & install
## Windows

## MacOs
* mkdir build & cd build
* cmake .. -GXcode -DCMAKE_HOST_SYSTEM_PROCESSOR=arm64 -DCMAKE_SYSTEM_PROCESSOR=arm64
* cmake --build . 
* sudo cmake --install . --config Debug

# TODO
* Classes documentation
* Test coverage
