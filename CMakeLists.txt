cmake_minimum_required(VERSION 3.10)
project(StreamingService CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code coverage generation" OFF)

if (ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

find_package(GTest REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tests
    ${GTEST_INCLUDE_DIR}
)

add_executable(ServicioStreamingTest
    tests/tests.cpp
    video.cpp
    episodio.cpp
    pelicula.cpp
    serie.cpp
    serviciostreaming.cpp
    main.cpp
)

target_link_libraries(ServicioStreamingTest
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    pthread
)

enable_testing()
add_test(NAME ServicioStreamingTest COMMAND ServicioStreamingTest)
