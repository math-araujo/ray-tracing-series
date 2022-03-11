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

# Create executables for Book 3
add_executable(pi_v1 src/book3/pi_v1.cpp)
add_executable(pi_v2 src/book3/pi_v2.cpp)

# Function to set common properties to executables files, such as compiler version and library dependencies
function(set_executables_properties executable)
    target_compile_features(${executable} PRIVATE cxx_std_17)
    target_include_directories(${executable} PRIVATE external)
    target_link_libraries(${executable} PRIVATE common_lib)
endfunction()

# Call the previous function for each executable
set(executables firstbooks pi_v1 pi_v2)
foreach(executable ${executables})
    set_executables_properties(${executable})
endforeach()
