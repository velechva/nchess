cmake_minimum_required(VERSION 3.20)
project(nchess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

file(GLOB src "*.h" "*.cpp")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_COMPILER_FL)

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
        src/ui/ui.h
        src/ui/ui.cpp
        src/validation/validate.h
        src/validation/validate.cpp
        src/ui/colors.h)

target_link_libraries(main ${CURSES_LIBRARIES})
