cmake_minimum_required(VERSION 3.10)
project(StreamingService CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code coverage generation" OFF)

if (ENABLE_COVERAGE)
    message(STATUS "Code coverage habilitado.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tests
    ${GTEST_INCLUDE_DIRS}
)

# Libreria principal
add_library(StreamingLibrary
    Pelicula.cpp
    Serie.cpp
    Episodio.cpp
    ServicioStreaming.cpp
)

# Executable principal
add_executable(StreamingApp main.cpp)
target_link_libraries(StreamingApp PRIVATE StreamingLibrary)

# Executable de pruebas
add_executable(ServicioStreamingTest
    tests/tests.cpp
)
set_target_properties(ServicioStreamingTest PROPERTIES CXX_STANDARD 17)
target_link_libraries(ServicioStreamingTest PRIVATE GTest::gtest GTest::gtest_main StreamingLibrary)

enable_testing()
add_test(NAME ServicioStreamingTest COMMAND ServicioStreamingTest)
