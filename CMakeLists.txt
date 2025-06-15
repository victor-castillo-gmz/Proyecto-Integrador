cmake_minimum_required(VERSION 3.10)
project(StreamTest)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

# Incluir gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests.cpp
    Video.cpp
    Pelicula.cpp
    Serie.cpp
    Episodio.cpp
    ServicioStreaming.cpp
)

target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
add_test(NAME AllTests COMMAND tests)
