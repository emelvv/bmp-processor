cmake_minimum_required(VERSION 3.17)
project(bmp_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")


file(GLOB_RECURSE SRC_FILES "src/*.cpp")


add_executable(bmp_processor ${SRC_FILES})

target_include_directories(bmp_processor PUBLIC include)