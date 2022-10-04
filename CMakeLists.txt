cmake_minimum_required(VERSION 3.0.0)
project(systemstuff VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB SRCS *.cpp *.hpp)

add_executable(systemstuff ${SRCS})
target_link_libraries(systemstuff gtest_main)
