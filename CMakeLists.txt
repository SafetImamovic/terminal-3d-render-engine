cmake_minimum_required(VERSION 3.20)

project(3d-render-engine)

set(CMAKE_C_STANDARD 17)

# Ensure build type is set (defaults to Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Include directories
include_directories(include)

# Collect source files
file(GLOB SOURCES
    src/core/*.c
    src/globals.c
    src/engine/*.c
    src/math/*.c
    src/main.c
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set platform-specific linker flags
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 user32 gdi32)
else()
    target_link_libraries(${PROJECT_NAME} m)
endif()

# Enable warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Ensure debug symbols are enabled in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 )
endif()

# Custom target: "make run"
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

