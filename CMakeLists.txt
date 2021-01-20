cmake_minimum_required(VERSION 3.12)
project(RayTracingWeekend)

add_executable(main 
    src/one_weekend/main.cpp
)

target_compile_features(main PRIVATE cxx_std_17)