cmake_minimum_required(VERSION 3.20)
project(nchess)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main src/main.cpp)
target_link_libraries(main ${CURSES_LIBRARIES})
