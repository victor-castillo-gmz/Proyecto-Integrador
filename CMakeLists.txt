# Minimum required CMake version. Version 3.10 is widely compatible.
cmake_minimum_required(VERSION 3.10)

# Define your project. Set CXX as the language for C++ projects.
project(StreamingService CXX)

# Set the C++ standard to C++17.
# This ensures modern C++ features are available.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Prefer strict standard compliance

# Option to enable code coverage, controlled by the GitHub Actions workflow.
# It defaults to OFF if not explicitly set (e.g., -DENABLE_COVERAGE=ON).
option(ENABLE_COVERAGE "Enable code coverage generation" OFF)
if (ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled.")
    # Add compiler flags for GCOV/LCOV.
    # -fprofile-arcs and -ftest-coverage are for instrumenting the code.
    # -g is for including debug information, essential for coverage tools.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Find Google Test. The 'REQUIRED' keyword will cause CMake to fail if GTest is not found.
# The GitHub Actions workflow now ensures GTest is installed.
find_package(GTest REQUIRED)

# Include directories where your compiler should look for header files.
# CMAKE_SOURCE_DIR points to the root of your project where your .h files are.
# GTEST_INCLUDE_DIR is found by find_package(GTest).
# Also include the 'tests' directory for tests.cpp to find its headers (like gtest/gtest.h)
include_directories(
    ${CMAKE_SOURCE_DIR}       # For your core project headers (Pelicula.h, Serie.h, etc.)
    ${CMAKE_SOURCE_DIR}/tests # For any headers within your 'tests' directory if tests.cpp needs them
    ${GTEST_INCLUDE_DIR}      # For Google Test headers
)

# Create the test executable.
# All source files are now explicitly listed here. This rules out any issues
# with the 'PROJECT_SOURCES' variable or its expansion.
add_executable(ServicioStreamingTest
    tests/tests.cpp       # Your Google Test source file
    Video.cpp             # Core project source
    Episodio.cpp          # Core project source
    Pelicula.cpp          # Core project source
    Serie.cpp             # Core project source
    ServicioStreaming.cpp # Core project source
    # main.cpp is typically NOT included in the unit test executable,
    # as the Google Test framework provides its own main function.
    # Including it here would likely lead to 'multiple definition of main' errors.
)

# Link the test executable against the Google Test libraries.
# GTEST_LIBRARIES and GTEST_MAIN_LIBRARIES are variables set by find_package(GTest).
# 'pthread' is often required for GTest on Linux systems.
target_link_libraries(ServicioStreamingTest
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    pthread
)

# Enable testing for CTest.
# This makes your tests discoverable by the 'ctest' command in your workflow.
enable_testing()

# Add a test entry for CTest.
# The NAME is what CTest will call the test.
# The COMMAND is the executable that CTest will run.
add_test(NAME ServicioStreamingTest COMMAND ServicioStreamingTest)