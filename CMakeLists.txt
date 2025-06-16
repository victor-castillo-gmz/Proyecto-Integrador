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
# Si la cobertura está habilitada, se aplican las banderas a la LIBRERÍA
# para que su código pueda ser medido.
if(ENABLE_COVERAGE)
    message(STATUS "Code coverage is enabled. Applying coverage flags.")
    # --coverage es un atajo para -fprofile-arcs -ftest-coverage (compilar) y -lgcov (linkear)
    target_compile_options(StreamingServiceLib PRIVATE --coverage)
    target_link_options(StreamingServiceLib PRIVATE --coverage)

    # *** INICIO DE LA CORRECCIÓN FINAL ***
    # El ejecutable principal TAMBIÉN necesita enlazarse con gcov.
    target_link_options(StreamingServiceApp PRIVATE --coverage)
    # *** FIN DE LA CORRECCIÓN FINAL ***
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
    
    # Se enlaza el ejecutable de pruebas con gtest, tu librería y pthread.
    target_link_libraries(ServicioStreamingTest PRIVATE gtest_main StreamingServiceLib pthread)

    # El ejecutable de pruebas también necesita las opciones de cobertura para funcionar.
    if(ENABLE_COVERAGE)
        target_link_options(ServicioStreamingTest PRIVATE --coverage)
    endif()

    # Se añade a CTest para que pueda ser ejecutado.
    add_test(NAME UnitTests COMMAND ServicioStreamingTest)
endif()