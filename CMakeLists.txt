cmake_minimum_required(VERSION 3.12)
project(RayTracingWeekend)

add_subdirectory(src/common)

add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE src/external)
target_link_libraries(main PRIVATE common_lib)
