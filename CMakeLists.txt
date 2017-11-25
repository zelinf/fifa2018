cmake_minimum_required(VERSION 3.6)
project(fifa2018)

set(CMAKE_CXX_STANDARD 14)

# Dependencies
add_subdirectory(libs/catch)
add_subdirectory(libs/json)

enable_testing()
# Our own components

add_subdirectory(src/app)
add_subdirectory(src/fifa2018)