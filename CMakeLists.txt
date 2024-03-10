cmake_minimum_required(VERSION 3.22)
project(KINO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
    add_subdirectory(test)
endif()

find_package(RapidJSON REQUIRED)
target_link_libraries(rapidjson)

include_directories(./include)
set(LIB_INCLUDES include/booking.h)
set(LIB_SOURCES src/booking.cpp)

set(KINO_SOURCES src/kino.cpp)

##made it static to simplify testing && containerization
add_library(booking STATIC ${LIB_SOURCES})
target_link_libraries(booking rapidjson)

add_executable(kino ${KINO_SOURCES})
target_link_libraries(kino booking)
target_link_libraries(kino rapidjson)