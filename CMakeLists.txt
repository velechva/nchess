cmake_minimum_required(VERSION 3.20)
project(nchess)

set(CMAKE_CXX_STANDARD 17)

file(GLOB src "*.h" "*.cpp")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(main
        src/main.cpp
        src/model/types.h
        src/using.h
        src/model/piece.h
        src/model/state.h
        src/ui/ui.h
        src/util.h
        src/validation/validate.h
        src/file.h src/log.h src/validation/typevalidator.h)

target_link_libraries(main ${CURSES_LIBRARIES})
