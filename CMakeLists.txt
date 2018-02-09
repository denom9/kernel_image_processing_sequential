cmake_minimum_required(VERSION 3.9)
project(Kernel_image)

set(CMAKE_CXX_STANDARD 11)

add_executable(Kernel_image main.cpp Image.cpp Image.h PPM.cpp PPM.h Utils.h kernels.h)