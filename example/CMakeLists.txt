cmake_minimum_required(VERSION 3.11)
project(ConflictPhysicsExample)

add_executable(ConflictPhysicsExample main.c)

set(raylib_VERBOSE 1)
find_library(RAYLIB_LIBRARY NAMES raylib)

if(NOT RAYLIB_LIBRARY)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.5.0
    )
    FetchContent_GetProperties(raylib)
    if(NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
    target_link_libraries(ConflictPhysicsExample PRIVATE raylib ConflictPhysics)
else()
    message(STATUS "raylib library => ${RAYLIB_LIBRARY}")
    target_link_libraries(ConflictPhysicsExample PRIVATE ${RAYLIB_LIBRARY} ConflictPhysics)
    target_include_directories(ConflictPhysicsExample PRIVATE "/opt/homebrew/Cellar/raylib/4.5.0/include")
endif()
