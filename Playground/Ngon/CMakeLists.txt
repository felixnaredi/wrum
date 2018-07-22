# CMakeLists.txt
#
# Author: Felix Naredi
# Date: 2018-07-18 02:54:18 +0200
#

cmake_minimum_required(VERSION 3.10.3)
project(ngon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_LIBRARIES "-lGL -lGLEW -lglfw -lpthread")
set(CMAKE_CXX_COMPILER clang++)

include_directories(../../include)
file(GLOB SOURCES "*.cpp")

add_definitions(-DPLG_SHADER_DIR="../shaders/")

add_executable(run ${SOURCES})
# target_compile_options(run PUBLIC -O3)
