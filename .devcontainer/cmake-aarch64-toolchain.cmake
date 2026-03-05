# CMake Toolchain File for aarch64 Cross-Compilation
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=.devcontainer/cmake-aarch64-toolchain.cmake ..

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross-compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Set find_program to look in cross-compiler paths
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set compiler flags for aarch64
set(CMAKE_C_FLAGS "-march=armv8-a" CACHE STRING "C Flags")
set(CMAKE_CXX_FLAGS "-march=armv8-a" CACHE STRING "CXX Flags")
