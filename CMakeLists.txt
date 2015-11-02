cmake_minimum_required(VERSION 3.0)
project(jmidiexec)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

#find_package(jack REQUIRED)

set(SOURCES jmemain.cpp)
add_executable(jmidiexec ${SOURCES})
target_link_libraries(jmidiexec jack)
