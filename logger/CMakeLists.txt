cmake_minimum_required(VERSION 3.16)

project(boost_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for your project
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/writer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/formatter
)

# Add all source files from each directory
file(GLOB_RECURSE FILTER_SRC
    src/filter/*.cpp
)

file(GLOB_RECURSE FORMATTER_SRC
    src/formatter/*.cpp
)

file(GLOB_RECURSE WRITER_SRC
    src/writer/*.cpp
)

file(GLOB_RECURSE OTHER_SRC
    src/*.cpp
)

# Create the executable target
add_executable(main
    main.cpp
    ${FILTER_SRC}
    ${FORMATTER_SRC}
    ${WRITER_SRC}
    ${OTHER_SRC}
)

# Find Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(main ${Boost_LIBRARIES} boost_thread)

# Enable export of compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (PROJECT_IS_TOP_LEVEL AND UNIX)
    # Create symlink to compile_commands.json for IDE to pick it up
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()
