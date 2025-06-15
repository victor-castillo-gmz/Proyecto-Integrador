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
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${GTEST_INCLUDE_DIR}
)

# Define all your application's source files.
# These are the .cpp files that make up your core classes.
set(PROJECT_SOURCES
    Video.cpp
    Episodio.cpp
    Pelicula.cpp
    Serie.cpp
    ServicioStreaming.cpp
    main.cpp # If main.cpp is part of your main application build, include it here.
             # If main.cpp is ONLY for running the application and not part of unit tests,
             # you might separate it, but for a simple project with GTest, including it
             # in the test executable is often simplest.
)

# Create the test executable.
# This executable will compile all your project sources AND your tests.cpp.
# The name of your GTest file is 'tests.cpp' as per your file structure.
add_executable(streaming_service_tests
    tests.cpp # Your Google Test source file
    ${PROJECT_SOURCES} # All your application's source files
)

# Link the test executable against the Google Test libraries.
# GTEST_LIBRARIES and GTEST_MAIN_LIBRARIES are variables set by find_package(GTest).
# 'pthread' is often required for GTest on Linux systems.
target_link_libraries(streaming_service_tests
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
add_test(NAME StreamingServiceUnitTests COMMAND streaming_service_tests)

# Optional: Set properties for the test if you want to categorize or add labels.
# set_tests_properties(StreamingServiceUnitTests PROPERTIES LABELS "Unit")