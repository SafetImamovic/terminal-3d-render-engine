cmake_minimum_required(VERSION 3.20)

project(3d-render-engine)

set(CMAKE_C_STANDARD 17)

include_directories(include)

file(GLOB SOURCES
    src/core/*.c
    src/globals.c
    # src/engine/*.c
    src/math/*.c
    src/main.c
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set C flags for platform-specific code (e.g., for Windows)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32)
else()
    target_link_libraries(${PROJECT_NAME} m)
endif()

# Enable warnings and extra warnings
add_compile_options(-Wall -Wextra)

# Set debugging flags for Debug build type
set(CMAKE_C_FLAGS_DEBUG "-g")  # This will enable debugging symbols (-g)

# Optionally, add specific flags for different build types
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
endif()

# Custom target: "make run"
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

