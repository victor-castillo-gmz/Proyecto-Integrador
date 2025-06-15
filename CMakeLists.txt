# Minimum required CMake version.
cmake_minimum_required(VERSION 3.10)

# Define your project.
project(StreamingService CXX)

# Set the C++ standard to C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to enable code coverage.
option(ENABLE_COVERAGE "Enable code coverage generation" OFF)
if (ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Find Google Test.
find_package(GTest REQUIRED)

# Create the test executable.
add_executable(ServicioStreamingTest
    tests/tests.cpp
    video.cpp
    episodio.cpp
    pelicula.cpp
    serie.cpp
)

# ====================================================================
# SOLUCIÓN: Especificar las rutas de inclusión para el target.
# Esto le dice a 'ServicioStreamingTest' que busque headers (.h)
# en el directorio raíz del proyecto.
# ====================================================================
target_include_directories(ServicioStreamingTest PUBLIC
    ${CMAKE_SOURCE_DIR}
)

# Link the test executable against the Google Test libraries.
target_link_libraries(ServicioStreamingTest PUBLIC
    GTest::gtest_main
    pthread
)

# Enable testing for CTest.
enable_testing()

# Add a test entry for CTest.
add_test(NAME ServicioStreamingTest COMMAND ServicioStreamingTest)