cmake_minimum_required(VERSION 3.25)
project(ConflictPhysics C)

set(CMAKE_C_STANDARD 99)

add_library(ConflictPhysics conflict_physics.c)
add_subdirectory(example)