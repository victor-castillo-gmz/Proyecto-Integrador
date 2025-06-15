cmake_minimum_required(VERSION 3.10) # Use a version compatible with your runner, 3.10 is common
project(StreamingService CXX)

set(CMAKE_CXX_STANDARD 17) # Or 14, or 11, depending on your code features
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable coverage if the option is passed (from GitHub Actions)
option(ENABLE_COVERAGE "Enable code coverage generation" OFF)
if (ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Find Google Test
# Line 9, as per your error message, is likely here.
find_package(GTest REQUIRED)

# Include directories for your project headers
include_directories(
    ${CMAKE_SOURCE_DIR} # Assumes your .h files are in the root
    ${GTEST_INCLUDE_DIR}
)

# Define your project's source files
set(PROJECT_SOURCES
    Video.cpp
    Episodio.cpp
    Pelicula.cpp
    Serie.cpp
    ServicioStreaming.cpp
)

# Add your main library or executable (if you have one)
# For now, let's just focus on the tests.

# Add the test executable
add_executable(streaming_service_tests streaming_service_tests.cpp ${PROJECT_SOURCES}) # Link all source files with the test file

# Link the test executable with GTest libraries
target_link_libraries(streaming_service_tests
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES} # Or just ${GTEST_LIBRARIES} if GTEST_MAIN_LIBRARIES is part of it.
    pthread # Google Test often needs pthread on Linux
)

# Enable testing for CTest
enable_testing()

# Add the tests to CTest
add_test(NAME StreamingServiceUnitTests COMMAND streaming_service_tests)