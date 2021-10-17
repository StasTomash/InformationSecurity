cmake_minimum_required(VERSION 3.16)
project(InformationSecurity)

set(CMAKE_CXX_STANDARD 17)

add_executable(kalyna kalyna_main.cpp Kalyna/kalyna.h Kalyna/constants.h)
add_executable(aes aes_main.cpp AES/AES.h AES/constants.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(aes_test AES/aes_test.cpp AES/AES.h AES/constants.h)
target_link_libraries(aes_test gtest_main)

add_executable(kalyna_test Kalyna/kalyna_test.cpp Kalyna/kalyna.h Kalyna/constants.h)
target_link_libraries(kalyna_test gtest_main)

add_executable(rc4_test RC4/rc4_test.cpp RC4/rc4.h)
target_link_libraries(rc4_test gtest_main)

add_executable(salsa20_test Salsa20/salsa20_test.cpp Salsa20/salsa20.h)
target_link_libraries(salsa20_test gtest_main)