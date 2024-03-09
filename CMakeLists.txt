cmake_minimum_required(VERSION 3.22)
project(KINO)


##if (NOT BUILD_TESTING STREQUAL OFF)
##    enable_testing()
##    add_subdirectory(test)
##endif()
include_directories(./include)
set(LIB_INCLUDES include/booking.h)
set(LIB_SOURCES src/booking.cpp)

set(KINO_SOURCES src/kino.cpp)

# Construct test executable
add_library(booking SHARED ${LIB_SOURCES})


add_executable(kino ${KINO_SOURCES})