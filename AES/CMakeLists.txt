cmake_minimum_required(VERSION 3.16)
project(libAES)

set(CMAKE_CXX_STANDARD 14)

add_library(libAES AES.h constants.h)
set_target_properties(libAES PROPERTIES LINKER_LANGUAGE CXX)