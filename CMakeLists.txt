cmake_minimum_required(VERSION 3.12)
project(chasm)

set(CMAKE_CXX_STANDARD 14)

set(SFML_ROOT "C:/Users/Finiks/Downloads/SFML-2.5.1-windows-vc15-32-bit/SFML-2.5.1")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(SFML COMPONENTS network audio graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR} "include")

set(SOURCE_FILES src/input.cpp src/emulator.cpp src/main.cpp)
add_executable(chasm ${SOURCE_FILES})
target_link_libraries(chasm ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
