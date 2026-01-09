cmake_minimum_required(VERSION 3.21)

project(LiftoffSimulator)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# set(CMAKE_C_COMPILER mpicc ${CMAKE_C_FLAGS} ${CMAKE_LDFLAGS})

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Add additional include directories for standard libraries
include_directories(SYSTEM ${CMAKE_C_COMPILER_INCLUDE_DIRS})

# Set additional compiler flags (e.g., include paths for standard libraries)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${CMAKE_C_COMPILER_INCLUDE_DIRS}")

# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INCLUDE_DIRECTORIES
    ${PROJECT_DIR}/include
)

set(GENERIC_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -pedantic
    )

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/src/*.c
    ${PROJECT_DIR}/src/*.cpp)


# Append to this list
include_directories(
    # Needed for linking protobuf-c on alliancecan systems
    ~/.local/include
)

# Append to this list
link_directories(
    # Needed for linking protobuf-c on alliancecan systems
    ~/.local/lib
    # Library Directories for MPI on RHEL Systems
    /usr/lib/
)

add_executable(${EXECUTABLE}
    ${PROJECT_SOURCES}
)

target_include_directories(${EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES}
    )

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    ${GENERIC_COMPILE_OPTIONS}
    # Include Debug Symbols
    -g
    # Maximum Optimization
     -O0
     -march=native
)

target_link_libraries(${EXECUTABLE}
    -lm
    pthread
)
