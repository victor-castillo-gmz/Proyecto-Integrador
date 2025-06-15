# Minimum required CMake version. Version 3.10 is widely compatible.
cmake_minimum_required(VERSION 3.10)

# Define your project. Set CXX as the language for C++ projects.
project(StreamingService CXX)

# Set the C++ standard to C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Prefer strict standard compliance

# Option to enable code coverage, controlled by the GitHub Actions workflow.
option(ENABLE_COVERAGE "Enable code coverage generation" OFF)
if (ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled.")
    # Add compiler flags for GCOV/LCOV.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Find Google Test.
find_package(GTest REQUIRED)

# Include directories where your compiler should look for header files.
# CMAKE_SOURCE_DIR correctly points to the repository root.
include_directories(
    ${CMAKE_SOURCE_DIR}       # For your core project headers (e.g., video.h, episodio.h)
    ${GTEST_INCLUDE_DIR}      # For Google Test headers
)

# Create the test executable.
# NOTE: Removed 'ServicioStreaming.cpp' as it does not exist in the file list.
add_executable(ServicioStreamingTest
    tests/tests.cpp
    video.cpp
    episodio.cpp
    pelicula.cpp
    serie.cpp
    # main.cpp is NOT included in the unit test executable.
)

# Link the test executable against the Google Test libraries.
target_link_libraries(ServicioStreamingTest
    GTest::gtest_main # Modern way to link GTest
    pthread
)

# Enable testing for CTest.
enable_testing()

# Add a test entry for CTest.
add_test(NAME ServicioStreamingTest COMMAND ServicioStreamingTest)