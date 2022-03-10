cmake_minimum_required(VERSION 3.12)
project(RayTracingSeries)

add_subdirectory(src/common)

# Create executable for Book 1 and Book 2 scenes
add_executable(firstbooks 
    src/first-books/main.cpp
    src/first-books/aarect.hpp
    src/first-books/box.hpp
    src/first-books/bvh.hpp
    src/first-books/constant_medium.hpp
    src/first-books/hittable_list.hpp
    src/first-books/hittable.hpp
    src/first-books/material.hpp
    src/first-books/moving_sphere.hpp
    src/first-books/scenes.hpp
    src/first-books/transform.hpp
)
target_compile_features(firstbooks PRIVATE cxx_std_17)
target_include_directories(firstbooks PRIVATE external)
target_link_libraries(firstbooks PRIVATE common_lib)