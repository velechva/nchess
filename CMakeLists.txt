cmake_minimum_required(VERSION 3.20)
project(nchess)

set(CMAKE_CXX_STANDARD 17)

file(GLOB src "*.h" "*.cpp")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main
        src/main.cpp
        src/util.h
        src/util.cpp
        src/file.h
        src/file.cpp
        src/log.h
        src/model/types.h
        src/model/piece.h
        src/model/state.h
        src/model/state.cpp
        src/ui.h
        src/ui.cpp
        src/validation/validate.h
        src/validation/typevalidator.h)

target_link_libraries(main ${CURSES_LIBRARIES})
