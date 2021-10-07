cmake_minimum_required(VERSION 3.16)
project(InformationSecurity)

set(CMAKE_CXX_STANDARD 17)

add_executable(Kalyna kalyna_main.cpp Kalyna/kalyna.h Kalyna/constants.h)
add_executable(AES main.cpp AES/AES.h AES/constants.h)