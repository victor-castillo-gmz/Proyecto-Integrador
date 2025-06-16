cmake_minimum_required(VERSION 3.15)

project(StreamingService VERSION 1.0.0 LANGUAGES CXX)

# Global C++ standard configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories for your project headers
include_directories(${CMAKE_SOURCE_DIR})

# Define your core application source files
set(APP_SOURCES
    episodio.cpp
    pelicula.cpp
    serie.cpp
    serviciostreaming.cpp
    video.cpp
)

# Create a static library from your application sources
add_library(StreamingServiceLib STATIC ${APP_SOURCES})

# Create the main application executable
add_executable(StreamingServiceApp main.cpp)
target_link_libraries(StreamingServiceApp PRIVATE StreamingServiceLib)

# --- Google Test Integration using FetchContent ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# --- Unit Testing and Code Coverage Configuration ---
option(ENABLE_TESTING "Enable a Unit Testing build." ON)
option(ENABLE_COVERAGE "Enable code coverage reporting." ON)

if(ENABLE_TESTING)
    enable_testing()

    # Function to create a test executable
    function(add_my_test_executable name test_source)
        add_executable(${name} ${test_source})

        # Link the test executable against Google Test and your application library
        target_link_libraries(${name} PRIVATE gtest gtest_main StreamingServiceLib pthread)

        if (ENABLE_COVERAGE)
            # Apply coverage-specific compile and link options only to the test executable
            target_compile_options(${name} PRIVATE -g -fprofile-arcs -ftest-coverage)
            target_link_options(${name} PRIVATE -fprofile-arcs -ftest-coverage)
        endif()

        # Register the executable as a test with CTest
        add_test(NAME ${name} COMMAND ${name})
    endfunction()

    # Add your main unit test executable
    add_my_test_executable(ServicioStreamingTest tests/tests.cpp)

    # --- Code Coverage Report Generation ---
    if (ENABLE_COVERAGE)
        find_program(LCOV lcov)
        if (LCOV)
            add_custom_target(coverage
                COMMAND ${CMAKE_CTEST_COMMAND}
                COMMAND ${LCOV} --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
                COMMAND ${LCOV} --remove coverage.info '*/googletest/*' '*/usr/*' '*/tests/*' --output-file coverage_filtered.info
                COMMAND genhtml coverage_filtered.info --output-directory coverage_html
                COMMENT "Generating code coverage report in build/coverage_html"
                VERBATIM
            )
        else()
            message(WARNING "LCOV not found, coverage reporting will not be available. Please install lcov.")
        endif()

        find_program(GCOVR gcovr)
        if (GCOVR)
            add_custom_target(sonarqube_coverage_xml
                COMMAND ${CMAKE_CTEST_COMMAND}
                COMMAND ${GCOVR} --root ${CMAKE_SOURCE_DIR} --exclude 'tests/*' --exclude 'main.cpp' --sonarqube -o coverage.xml
                COMMENT "Generating SonarQube coverage XML report in build/coverage.xml"
                VERBATIM
            )
        else()
            message(WARNING "GCOVR not found, SonarQube coverage XML will not be available. Please install gcovr.")
        endif()

    endif() # END ENABLE_COVERAGE
endif() # END ENABLE_TESTING