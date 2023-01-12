# FIPP
Fast Image Processing Pipeline Systeme implemented in C++17 without bigger dependencies. See wiki for further informations.
Plugins for FIPP are located in [FIPP-Plugins](https://github.com/mual-de/FIPP-Plugins).
FIPP imageProcessing Library is planned as direct interface between FIPP and opencv/nppi.

![Build and Test](https://github.com/mual-de/FIPP/actions/workflows/build_cpp.yml/badge.svg)


## Build

This project depends on the libstdc++11 ABI. Conan needs to be updated to this ABI:
`conan profile update settings.compiler.libcxx=libstdc++11 default`

The last two lines are the important ones, otherwise gtest will fail.

Than do the normal stuff:
```bash
mkdir build
cd build
conan install ..
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON ..
cmake --build ..
```