cmake_minimum_required(VERSION 2.5)
project(RotorEncyptionDiffieHellman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp RotorEncryption.cpp RotorEncryption.h DiffieHellman.cpp DiffieHellman.h)
add_executable(RotorEncyptionDiffieHellman ${SOURCE_FILES})