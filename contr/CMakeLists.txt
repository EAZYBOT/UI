cmake_minimum_required(VERSION 3.13)
project(contr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static-libstdc++ -static-libgcc -static -lpthread")

add_executable(contr main.cpp)