cmake_minimum_required(VERSION 3.16)
project(StreamingService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Opciones del Proyecto ===
option(ENABLE_TESTING "Habilitar la compilacion de pruebas unitarias" ON)
option(ENABLE_COVERAGE "Habilitar los reportes de cobertura de codigo" ON)

# === Directorios de Cabeceras ===
include_directories(${CMAKE_SOURCE_DIR})

# === Fuentes de la Aplicación Principal ===
set(APP_SOURCES
    episodio.cpp
    pelicula.cpp
    serie.cpp
    serviciostreaming.cpp
    video.cpp
)

# === Creación de la Librería Principal ===
add_library(StreamingServiceLib STATIC ${APP_SOURCES})

# === Creación del Ejecutable Principal ===
add_executable(StreamingServiceApp main.cpp)
target_link_libraries(StreamingServiceApp PRIVATE StreamingServiceLib)

# ===================================================================
# ================ CONFIGURACIÓN DE PRUEBAS Y COBERTURA =============
# ===================================================================

# --- Configuración Global de Cobertura ---
if(ENABLE_COVERAGE)
    message(STATUS "Code coverage is enabled. Applying coverage flags.")
    target_compile_options(StreamingServiceLib PRIVATE --coverage)
    target_link_options(StreamingServiceLib PRIVATE --coverage)
    target_link_options(StreamingServiceApp PRIVATE --coverage)
endif()


if(ENABLE_TESTING)
    # --- Integración de Google Test ---
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
    
    # --- Creación del Ejecutable de Pruebas ---
    enable_testing() 

    add_executable(ServicioStreamingTest tests/tests.cpp)
    target_link_libraries(ServicioStreamingTest PRIVATE gtest_main StreamingServiceLib pthread)

    if(ENABLE_COVERAGE)
        target_link_options(ServicioStreamingTest PRIVATE --coverage)
    endif()

    add_test(NAME UnitTests COMMAND ServicioStreamingTest)

    # *** INICIO DE LA CORRECCIÓN ***
    # --- Generación de Reportes de Cobertura para SonarQube ---
    # Se vuelve a añadir la regla que tu sistema de CI necesita.
    if(ENABLE_COVERAGE)
        find_program(GCOVR_EXECUTABLE gcovr)
        if(GCOVR_EXECUTABLE)
            add_custom_target(sonarqube_coverage_xml
                # Primero, se corren las pruebas para generar los archivos .gcda
                COMMAND ${CMAKE_CTEST_COMMAND}
                # Luego, gcovr genera el reporte XML
                COMMAND ${GCOVR_EXECUTABLE} 
                        --root ${CMAKE_SOURCE_DIR} 
                        --exclude 'tests/.*' 
                        --sonarqube coverage.xml
                COMMENT "Generating SonarQube coverage XML report (coverage.xml)..."
                VERBATIM
            )
        else()
            message(WARNING "gcovr executable not found. The 'sonarqube_coverage_xml' target will not be available.")
        endif()
    endif()
    # *** FIN DE LA CORRECCIÓN ***

endif()