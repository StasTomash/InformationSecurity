cmake_minimum_required(VERSION 3.16)
project(libKalyna)

set(CMAKE_CXX_STANDARD 14)

add_library(libKalyna kalyna.h constants.h)
set_target_properties(libKalyna PROPERTIES LINKER_LANGUAGE CXX)