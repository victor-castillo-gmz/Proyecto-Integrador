cmake_minimum_required(VERSION 3.15) # A slightly newer version than yours, but not as new as 3.30, for broader compatibility if needed. Your professor used 3.30, you could use that too.



project(StreamingService VERSION 1.0.0 LANGUAGES CXX) # Define project, version, and language



# Global C++ standard configuration

set(CMAKE_CXX_STANDARD 17) # Sticking to C++17 as in your original, but C++20 (like your professor's) is also an option if your code supports it.

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions for portability



# Include directories for your project headers

# CMAKE_SOURCE_DIR points to the root of your project where CMakeLists.txt resides.

include_directories(${CMAKE_SOURCE_DIR})



# Define your core application source files

# These are the files that make up your main application logic, excluding main.cpp and test files.

set(APP_SOURCES

    episodio.cpp

    pelicula.cpp

    serie.cpp

    serviciostreaming.cpp

    video.cpp

)



# Create a static library from your application sources

# This allows both your main executable and your test executable to link against the same core logic.

add_library(StreamingServiceLib STATIC ${APP_SOURCES})



# Create the main application executable

# This is the actual program the user would run.

add_executable(StreamingServiceApp main.cpp)

target_link_libraries(StreamingServiceApp PRIVATE StreamingServiceLib)





# --- Google Test Integration using FetchContent ---

# This method downloads Google Test as part of your build, making your project more self-contained.

include(FetchContent)

FetchContent_Declare(

    googletest

    GIT_REPOSITORY https://github.com/google/googletest.git

    GIT_TAG release-1.11.0 # Use a specific stable release tag

)

FetchContent_MakeAvailable(googletest)





# --- Unit Testing and Code Coverage Configuration ---



# Options that can be toggled via CMake flags (e.g., -DENABLE_TESTING=OFF)

option(ENABLE_TESTING "Enable a Unit Testing build." ON)

option(ENABLE_COVERAGE "Enable code coverage reporting." ON)



if(ENABLE_TESTING)

    enable_testing() # Enable CTest discovery



    # Function to create a test executable

    # This simplifies adding multiple test files and ensures consistent linking and flag application.

    function(add_my_test_executable name test_source)

        add_executable(${name} ${test_source})



        # Link the test executable against Google Test and your application library

        # `pthread` is often needed for Google Test on Linux.

        target_link_libraries(${name} PRIVATE gtest gtest_main StreamingServiceLib pthread)



        if (ENABLE_COVERAGE)

            # Apply coverage-specific compile and link options only to the test executable

            # '-g' ensures debug symbols are present, essential for coverage.

            target_compile_options(${name} PRIVATE -g -fprofile-arcs -ftest-coverage)

            target_link_options(${name} PRIVATE -fprofile-arcs -ftest-coverage)

        endif()



        # Register the executable as a test with CTest

        add_test(NAME ${name} COMMAND ${name})

    endfunction()



    # Add your main unit test executable

    # Your tests/tests.cpp will now test the StreamingServiceLib.

    add_my_test_executable(ServicioStreamingTest tests/tests.cpp)



    # --- Code Coverage Report Generation (integrated into CMake) ---

    if (ENABLE_COVERAGE)

        find_program(LCOV lcov) # Find the lcov tool on the system

        if (LCOV)

            # Define a custom CMake target to generate coverage reports.

            # You can run this with `cmake --build . --target coverage` after building and running tests.

            add_custom_target(coverage

                COMMAND ${CMAKE_CTEST_COMMAND} # Run all CTest tests first to generate .gcda files

                COMMAND ${LCOV} --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1 # Capture coverage data

                # Exclude Google Test's internal files and system headers from the report for cleaner results.

                COMMAND ${LCOV} --remove coverage.info '*/googletest/*' '*/usr/*' '*/tests/*' --output-file coverage_filtered.info

                # Generate HTML report

                COMMAND genhtml coverage_filtered.info --output-directory coverage_html

                COMMENT "Generating code coverage report in build/coverage_html"

                VERBATIM

            )

        else()

            message(WARNING "LCOV not found, coverage reporting will not be available. Please install lcov.")

        endif()



        # SonarQube needs coverage in a specific format, typically Cobertura XML.

        # We'll use gcovr to convert the lcov data to SonarQube XML.

        find_program(GCOVR gcovr)

        if (GCOVR)

            add_custom_target(sonarqube_coverage_xml

                COMMAND ${CMAKE_CTEST_COMMAND}

                COMMAND ${GCOVR} --root ${CMAKE_SOURCE_DIR} --exclude 'tests/*' --sonarqube -o coverage.xml

                COMMENT "Generating SonarQube coverage XML report in build/coverage.xml"

                VERBATIM

            )

        else()

            message(WARNING "GCOVR not found, SonarQube coverage XML will not be available. Please install gcovr.")

        endif()



    endif() # END ENABLE_COVERAGE

endif() # END ENABLE_TESTING"