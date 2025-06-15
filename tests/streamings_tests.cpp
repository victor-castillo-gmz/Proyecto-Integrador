#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Video.h"
#include "Pelicula.h"
#include "Serie.h"
#include "Episodio.h"
#include "ServicioStreaming.h"

// Simple test framework
class SimpleTest {
private:
    static int tests_run;
    static int tests_passed;
    static std::vector<std::string> failed_tests;
    
public:
    static void assert_equal(int expected, int actual, const std::string& test_name) {
        tests_run++;
        if (expected == actual) {
            tests_passed++;
            std::cout << "[PASS] " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name);
            std::cout << "[FAIL] " << test_name 
                      << " - Expected: " << expected 
                      << ", Got: " << actual << std::endl;
        }
    }
    
    static void assert_equal(double expected, double actual, const std::string& test_name) {
        tests_run++;
        if (abs(expected - actual) < 0.001) {
            tests_passed++;
            std::cout << "[PASS] " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name);
            std::cout << "[FAIL] " << test_name 
                      << " - Expected: " << expected 
                      << ", Got: " << actual << std::endl;
        }
    }
    
    static void assert_equal(const std::string& expected, const std::string& actual, const std::string& test_name) {
        tests_run++;
        if (expected == actual) {
            tests_passed++;
            std::cout << "[PASS] " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name);
            std::cout << "[FAIL] " << test_name 
                      << " - Expected: '" << expected 
                      << "', Got: '" << actual << "'" << std::endl;
        }
    }
    
    static void assert_true(bool condition, const std::string& test_name) {
        tests_run++;
        if (condition) {
            tests_passed++;
            std::cout << "[PASS] " << test_name << std::endl;
        } else {
            failed_tests.push_back(test_name);
            std::cout << "[FAIL] " << test_name << " - Expected true, got false" << std::endl;
        }
    }
    
    static void print_summary() {
        std::cout << "\n=== TEST SUMMARY ===" << std::endl;
        std::cout << "Tests run: " << tests_run << std::endl;
        std::cout << "Tests passed: " << tests_passed << std::endl;
        std::cout << "Tests failed: " << (tests_run - tests_passed) << std::endl;
        
        if (!failed_tests.empty()) {
            std::cout << "\nFailed tests:" << std::endl;
            for (const auto& test : failed_tests) {
                std::cout << "  - " << test << std::endl;
            }
        }
        
        if (tests_passed == tests_run) {
            std::cout << "\nALL TESTS PASSED! ✓" << std::endl;
        } else {
            std::cout << "\nSOME TESTS FAILED! ✗" << std::endl;
        }
    }
};

// Static member definitions
int SimpleTest::tests_run = 0;
int SimpleTest::tests_passed = 0;
std::vector<std::string> SimpleTest::failed_tests;

// Test functions
void test_episodio() {
    std::cout << "\n=== TESTING EPISODIO ===" << std::endl;
    
    Episodio ep("Pilot", 1);
    SimpleTest::assert_equal(std::string("Pilot"), ep.getTitulo(), "Episodio constructor - titulo");
    SimpleTest::assert_equal(1, ep.getTemporada(), "Episodio constructor - temporada");
    SimpleTest::assert_equal(0.0, ep.getCalificacionPromedio(), "Episodio constructor - calificacion inicial");
    
    ep.calificar(4);
    SimpleTest::assert_equal(4.0, ep.getCalificacionPromedio(), "Episodio calificar - primera calificacion");
    
    ep.calificar(5);
    SimpleTest::assert_equal(4.5, ep.getCalificacionPromedio(), "Episodio calificar - promedio de dos calificaciones");
}

void test_pelicula() {
    std::cout << "\n=== TESTING PELICULA ===" << std::endl;
    
    Pelicula pelicula("P001", "Inception", 148.0, "Sci-Fi");
    SimpleTest::assert_equal(std::string("P001"), pelicula.getID(), "Pelicula constructor - ID");
    SimpleTest::assert_equal(std::string("Inception"), pelicula.getNombre(), "Pelicula constructor - nombre");
    SimpleTest::assert_equal(std::string("Sci-Fi"), pelicula.getGenero(), "Pelicula constructor - genero");
    SimpleTest::assert_equal(0.0, pelicula.getCalificacionPromedio(), "Pelicula constructor - calificacion inicial");
    
    pelicula.calificar(5);
    SimpleTest::assert_equal(5.0, pelicula.getCalificacionPromedio(), "Pelicula calificar - calificacion unica");
    
    pelicula.calificar(3);
    SimpleTest::assert_equal(4.0, pelicula.getCalificacionPromedio(), "Pelicula calificar - promedio");
}

void test_serie() {
    std::cout << "\n=== TESTING SERIE ===" << std::endl;
    
    Serie serie("S001", "Breaking Bad", 45.0, "Drama");
    SimpleTest::assert_equal(std::string("S001"), serie.getID(), "Serie constructor - ID");
    SimpleTest::assert_equal(std::string("Breaking Bad"), serie.getNombre(), "Serie constructor - nombre");
    SimpleTest::assert_equal(0, (int)serie.getEpisodios().size(), "Serie constructor - episodios vacios");
    
    Episodio ep1("Pilot", 1);
    Episodio ep2("Cat's in the Bag", 1);
    
    serie.agregarEpisodio(ep1);
    serie.agregarEpisodio(ep2);
    SimpleTest::assert_equal(2, (int)serie.getEpisodios().size(), "Serie agregar episodios");
}

void test_servicio_streaming() {
    std::cout << "\n=== TESTING SERVICIO STREAMING ===" << std::endl;
    
    // Create test file
    std::ofstream testFile("simple_test_data.txt");
    testFile << "P,P001,Test Movie,120,Action\n";
    testFile << "S,S001,Test Series,30,Comedy\n";
    testFile << "E,S001,Episode 1,1\n";
    testFile.close();
    
    ServicioStreaming servicio;
    
    // Test loading file - this will depend on your implementation
    // For now, we'll just test that it doesn't crash
    try {
        servicio.cargarArchivo("simple_test_data.txt");
        SimpleTest::assert_true(true, "ServicioStreaming cargar archivo - no exception");
    } catch (...) {
        SimpleTest::assert_true(false, "ServicioStreaming cargar archivo - exception caught");
    }
    
    // Test rating a video
    try {
        servicio.calificarVideo("Test Movie", 4);
        SimpleTest::assert_true(true, "ServicioStreaming calificar video - no exception");
    } catch (...) {
        SimpleTest::assert_true(false, "ServicioStreaming calificar video - exception caught");
    }
    
    // Clean up
    std::remove("simple_test_data.txt");
}

void test_edge_cases() {
    std::cout << "\n=== TESTING EDGE CASES ===" << std::endl;
    
    // Test invalid ratings
    Episodio ep("Test", 1);
    double before = ep.getCalificacionPromedio();
    ep.calificar(0); // Invalid
    SimpleTest::assert_equal(before, ep.getCalificacionPromedio(), "Episodio calificacion invalida - no cambia promedio");
    
    ep.calificar(6); // Invalid
    SimpleTest::assert_equal(before, ep.getCalificacionPromedio(), "Episodio calificacion invalida alta - no cambia promedio");
    
    // Test multiple ratings
    Pelicula pelicula("P001", "Test", 100, "Drama");
    pelicula.calificar(1);
    pelicula.calificar(2);
    pelicula.calificar(3);
    pelicula.calificar(4);
    pelicula.calificar(5);
    SimpleTest::assert_equal(3.0, pelicula.getCalificacionPromedio(), "Pelicula multiples calificaciones - promedio correcto");
}

int main() {
    std::cout << "Running Simple Tests for Streaming Service..." << std::endl;
    
    test_episodio();
    test_pelicula();
    test_serie();
    test_servicio_streaming();
    test_edge_cases();
    
    SimpleTest::print_summary();
    
    return 0;
}