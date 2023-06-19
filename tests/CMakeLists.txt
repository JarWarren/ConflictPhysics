add_executable(ConflictPhysicsTests main.c)

set(raylib_VERBOSE 1)
find_library(RAYLIB_LIBRARY NAMES raylib HINTS "/opt/homebrew/Cellar/raylib/4.5.0/lib")
message(STATUS "raylib library => ${RAYLIB_LIBRARY}")

target_link_libraries(ConflictPhysicsTests PRIVATE ${RAYLIB_LIBRARY} ConflictPhysics)
target_include_directories(ConflictPhysicsTests PRIVATE "/opt/homebrew/Cellar/raylib/4.5.0/include")
