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
# NOW: Headers are relative to the 'build' directory where CMake is run.
include_directories(
    .               # For your core project headers (e.g., Video.h) which are now in the 'build' directory
    tests           # For any headers within your 'tests' directory (now 'build/tests/')
    ${GTEST_INCLUDE_DIR} # For Google Test headers
)

# Create the test executable.
# Paths are now relative to the 'build' directory itself.
add_executable(ServicioStreamingTest
    tests/tests.cpp       # Your Google Test source file, now relative to 'build/tests/'
    Video.cpp             # Core project source, now relative to 'build/'
    Episodio.cpp          # Core project source
    Pelicula.cpp          # Core project source
    Serie.cpp             # Core project source
    ServicioStreaming.cpp # Core project source
    # main.cpp is typically NOT included in the unit test executable,
    # as the Google Test framework provides its own main function.
)

# Link the test executable against the Google Test libraries.
target_link_libraries(ServicioStreamingTest
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    pthread
)

# Enable testing for CTest.
enable_testing()

# Add a test entry for CTest.
add_test(NAME ServicioStreamingTest COMMAND ServicioStreamingTest)