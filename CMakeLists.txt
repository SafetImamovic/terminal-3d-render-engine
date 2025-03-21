cmake_minimum_required(VERSION 3.20)

project(3d-render-engine)

set(CMAKE_C_STANDARD 17)

include_directories(include)

file(GLOB SOURCES
    # src/core/*.c
    # src/engine/*.c
    # src/math/*.c
    src/main.c
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set C flags for platform-specific code (e.g., for Windows)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32)
endif()

# Custom target: "make run"
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

