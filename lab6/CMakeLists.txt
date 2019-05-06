cmake_minimum_required(VERSION 3.13)
project(lab6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 14)

set(CMAKE_CXX_FLAGS
    ${CMAKE_CXX_FLAGS}
    -Wno-int-to-pointer-cast
)

file(GLOB lab6_src
    "*.h"
    "*.cpp"
)

include_directories("
  D:\\projects\\ui\\lab6\\db
")
link_directories("D:\\projects\\ui\\lab6\\db")

add_executable(lab6 ${lab6_src})
target_link_libraries(lab6
    sqlite3.dll
    comctl32
)