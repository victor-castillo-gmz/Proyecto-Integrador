cmake_minimum_required(VERSION 3.16)
project(StreamingService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Opciones del Proyecto ===
option(ENABLE_TESTING "Habilitar la compilacion de pruebas unitarias" ON)
option(ENABLE_COVERAGE "Habilitar los reportes de cobertura de codigo" ON)

# === Directorios de Cabeceras ===
# Incluye el directorio raíz para que #include "video.h" funcione.
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
# Esta librería contiene toda la lógica de tu aplicación.
add_library(StreamingServiceLib STATIC ${APP_SOURCES})

# === Creación del Ejecutable Principal ===
# Este es el programa que interactúa con el usuario.
add_executable(StreamingServiceApp main.cpp)
target_link_libraries(StreamingServiceApp PRIVATE StreamingServiceLib)


# ===================================================================
# ================ CONFIGURACIÓN DE PRUEBAS Y COBERTURA =============
# ===================================================================

if(ENABLE_TESTING)
    # --- Integración de Google Test ---
    # Descarga y configura Google Test automáticamente.
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1 # Usar una etiqueta de release estable
    )
    FetchContent_MakeAvailable(googletest)
    
    # --- Configuración de Cobertura ---
    # **LA CORRECCIÓN CLAVE ESTÁ AQUÍ**
    # Si la cobertura está habilitada, se aplican las banderas a la LIBRERÍA
    # para que su código pueda ser medido.
    if(ENABLE_COVERAGE)
        # --coverage es un atajo para -fprofile-arcs -ftest-coverage (compilar) y -lgcov (linkear)
        target_compile_options(StreamingServiceLib PRIVATE --coverage)
        target_link_options(StreamingServiceLib PRIVATE --coverage)
    endif()

    # --- Creación del Ejecutable de Pruebas ---
    enable_testing() # Habilita CTest

    add_executable(ServicioStreamingTest tests/tests.cpp)
    
    # Se enlaza el ejecutable de pruebas con:
    # 1. La librería de Google Test (gtest_main para tener un main predefinido)
    # 2. Tu librería (StreamingServiceLib), que ahora está instrumentada para cobertura.
    # 3. pthread, a menudo requerido por Google Test.
    target_link_libraries(ServicioStreamingTest PRIVATE gtest_main StreamingServiceLib pthread)

    # Se añade a CTest para que pueda ser ejecutado.
    add_test(NAME UnitTests COMMAND ServicioStreamingTest)

endif()