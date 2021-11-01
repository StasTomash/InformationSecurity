cmake_minimum_required(VERSION 3.16)
project(InformationSecurity)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(sha256_test SHA256/sha256.h SHA256/constants.h SHA256/sha256_test.cpp)
target_link_libraries(sha256_test gtest_main)

add_executable(kupyna_test Kupyna256/kupyna256.h Kupyna256/constants.h Kupyna256/kupyna_test256.cpp)
target_link_libraries(kupyna_test gtest_main)

add_executable(benchmark benchmark.cpp AES/AES.h AES/constants.h Kalyna/kalyna.h Kalyna/constants.h RC4/rc4.h Salsa20/salsa20.h)
add_executable(hash_benchmark hash_benchmark.cpp Kupyna256/kupyna256.h Kupyna256/constants.h SHA256/sha256.h SHA256/constants.h)