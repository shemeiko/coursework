cmake_minimum_required(VERSION 3.14)
project(app LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "$ENV{QTDIR}/**")

find_package(Qt6 REQUIRED COMPONENTS Core Core5Compat Widgets)

# Project files
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

add_executable(app ${SOURCES})

# Include directories
target_include_directories(app PRIVATE ${INCLUDE_DIR} ${SOURCE_DIR})

# Link Qt libraries
target_link_libraries(app PRIVATE Qt6::Core Qt6::Core5Compat Qt6::Widgets)