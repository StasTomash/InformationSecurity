cmake_minimum_required(VERSION 3.16)
project(InformationSecurity)

set(CMAKE_CXX_STANDARD 17)

#add_executable(Kalyna kalyna_main.cpp Kalyna/kalyna.h Kalyna/constants.h)
#add_executable(AES main.cpp AES/AES.h AES/constants.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(kalyna_test Kalyna/kalyna.h Kalyna/constants.h Kalyna/kalyna_test.cpp)
target_link_libraries(kalyna_test gtest_main)