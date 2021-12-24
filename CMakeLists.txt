cmake_minimum_required(VERSION 3.20)
project(nchess)

set(CMAKE_CXX_STANDARD 17)

file(GLOB src "*.h" "*.cpp")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main
        src/main.cpp
        src/types.h
        src/using.h
        src/piece.h
        src/state.h
        src/ui.h
        src/util.h
        src/validate.h
)

target_link_libraries(main ${CURSES_LIBRARIES})
