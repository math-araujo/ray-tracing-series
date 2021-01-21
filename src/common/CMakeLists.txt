cmake_minimum_required(VERSION 3.12)
project(Common)

add_library(common_lib INTERFACE)
target_include_directories(common_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})