#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "pelicula.h"
#include "serie.h"
#include "episodio.h"
#include "serviciostreaming.h"

// ========== TESTS DE EPISODIO ==========
class EpisodioTest : public ::testing::Test {
protected:
    void SetUp() override {
        episodio = new Episodio("Test Episode", 1);
    }
    
    void TearDown() override {
        delete episodio;
    }
    
    Episodio* episodio;
};

TEST_F(EpisodioTest, Constructor) {
    EXPECT_EQ(episodio->getTitulo(), "Test Episode");
    EXPECT_EQ(episodio->getTemporada(), 1);
    EXPECT_DOUBLE_EQ(episodio->getCalificacionPromedio(), 0.0);
    EXPECT_EQ(episodio->getCalificaciones().size(), 0);
}

TEST_F(EpisodioTest, CalificacionValida) {
    episodio->calificar(4);
    EXPECT_EQ(episodio->getCalificaciones().size(), 1);
    EXPECT_DOUBLE_EQ(episodio->getCalificacionPromedio(), 4.0);
}

TEST_F(EpisodioTest, CalificacionInvalida) {
    episodio->calificar(0);  // Muy baja
    episodio->calificar(6);  // Muy alta
    episodio->calificar(-1); // Negativa
    EXPECT_EQ(episodio->getCalificaciones().size(), 0);
    EXPECT_DOUBLE_EQ(episodio->getCalificacionPromedio(), 0.0);
}

TEST_F(EpisodioTest, MultipleCalificaciones) {
    episodio->calificar(3);
    episodio->calificar(5);
    episodio->calificar(4);
    EXPECT_EQ(episodio->getCalificaciones().size(), 3);
    EXPECT_DOUBLE_EQ(episodio->getCalificacionPromedio(), 4.0);
}

TEST_F(EpisodioTest, MostrarDatos) {
    episodio->calificar(5);
    // Esta prueba verifica que mostrarDatos no cause crash
    testing::internal::CaptureStdout();
    episodio->mostrarDatos();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// ========== TESTS DE PELICULA ==========
class PeliculaTest : public ::testing::Test {
protected:
    void SetUp() override {
        pelicula = new Pelicula("P001", "Test Movie", 120.0, "Drama");
    }
    
    void TearDown() override {
        delete pelicula;
    }
    
    Pelicula* pelicula;
};

TEST_F(PeliculaTest, Constructor) {
    EXPECT_EQ(pelicula->getID(), "P001");
    EXPECT_EQ(pelicula->getNombre(), "Test Movie");
    EXPECT_EQ(pelicula->getGenero(), "Drama");
    EXPECT_DOUBLE_EQ(pelicula->getCalificacionPromedio(), 0.0);
}

TEST_F(PeliculaTest, CalificacionSimple) {
    pelicula->calificar(4);
    EXPECT_DOUBLE_EQ(pelicula->getCalificacionPromedio(), 4.0);
}

TEST_F(PeliculaTest, CalificacionPromedio) {
    pelicula->calificar(4);
    pelicula->calificar(5);
    pelicula->calificar(3);
    EXPECT_DOUBLE_EQ(pelicula->getCalificacionPromedio(), 4.0);
}

TEST_F(PeliculaTest, CalificacionInvalida) {
    pelicula->calificar(0);
    pelicula->calificar(6);
    EXPECT_EQ(pelicula->getCalificaciones().size(), 0);
    EXPECT_DOUBLE_EQ(pelicula->getCalificacionPromedio(), 0.0);
}

TEST_F(PeliculaTest, MostrarDatos) {
    pelicula->calificar(4);
    testing::internal::CaptureStdout();
    pelicula->mostrarDatos();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Película") != std::string::npos);
    EXPECT_TRUE(output.find("Test Movie") != std::string::npos);
}

// ========== TESTS DE SERIE ==========
class SerieTest : public ::testing::Test {
protected:
    void SetUp() override {
        serie = new Serie("S001", "Test Series", 60.0, "Thriller");
    }
    
    void TearDown() override {
        delete serie;
    }
    
    Serie* serie;
};

TEST_F(SerieTest, Constructor) {
    EXPECT_EQ(serie->getID(), "S001");
    EXPECT_EQ(serie->getNombre(), "Test Series");
    EXPECT_EQ(serie->getGenero(), "Thriller");
    EXPECT_EQ(serie->getEpisodios().size(), 0);
}

TEST_F(SerieTest, AgregarEpisodio) {
    Episodio ep("Pilot", 1);
    serie->agregarEpisodio(ep);
    EXPECT_EQ(serie->getEpisodios().size(), 1);
}

TEST_F(SerieTest, CalificarEpisodioExistente) {
    Episodio ep("Pilot", 1);
    serie->agregarEpisodio(ep);
    
    testing::internal::CaptureStdout();
    serie->calificarEpisodio("Pilot", 5);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("calificado") != std::string::npos);
    EXPECT_DOUBLE_EQ(serie->getEpisodios()[0].getCalificacionPromedio(), 5.0);
}

TEST_F(SerieTest, CalificarEpisodioInexistente) {
    testing::internal::CaptureStdout();
    serie->calificarEpisodio("Nonexistent", 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("no encontrado") != std::string::npos);
}

TEST_F(SerieTest, CalificarEpisodioCaseInsensitive) {
    Episodio ep("Pilot", 1);
    serie->agregarEpisodio(ep);
    
    testing::internal::CaptureStdout();
    serie->calificarEpisodio("PILOT", 4);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("calificado") != std::string::npos);
}

TEST_F(SerieTest, MostrarEpisodios) {
    Episodio ep1("Episode 1", 1);
    Episodio ep2("Episode 2", 1);
    ep1.calificar(4);
    ep2.calificar(5);
    
    serie->agregarEpisodio(ep1);
    serie->agregarEpisodio(ep2);
    
    testing::internal::CaptureStdout();
    serie->mostrarEpisodios();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Episode 1") != std::string::npos);
    EXPECT_TRUE(output.find("Episode 2") != std::string::npos);
}

TEST_F(SerieTest, MostrarEpisodiosSinEpisodios) {
    testing::internal::CaptureStdout();
    serie->mostrarEpisodios();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("No hay episodios") != std::string::npos);
}

TEST_F(SerieTest, MostrarEpisodiosConCalificacion) {
    Episodio ep1("Good Episode", 1);
    Episodio ep2("Bad Episode", 1);
    ep1.calificar(5);
    ep2.calificar(2);
    
    serie->agregarEpisodio(ep1);
    serie->agregarEpisodio(ep2);
    
    testing::internal::CaptureStdout();
    serie->mostrarEpisodiosConCalificacion(4.0);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Good Episode") != std::string::npos);
    EXPECT_FALSE(output.find("Bad Episode") != std::string::npos);
}

TEST_F(SerieTest, MostrarEpisodiosConCalificacionSinResultados) {
    Episodio ep("Low Rated", 1);
    ep.calificar(2);
    serie->agregarEpisodio(ep);
    
    testing::internal::CaptureStdout();
    serie->mostrarEpisodiosConCalificacion(4.0);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("No se encontraron episodios") != std::string::npos);
}

TEST_F(SerieTest, MostrarDatos) {
    Episodio ep("Test Episode", 1);
    ep.calificar(4);
    serie->agregarEpisodio(ep);
    serie->calificar(5);
    
    testing::internal::CaptureStdout();
    serie->mostrarDatos();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Serie") != std::string::npos);
    EXPECT_TRUE(output.find("Test Series") != std::string::npos);
    EXPECT_TRUE(output.find("1") != std::string::npos); // Número de episodios
}

TEST_F(SerieTest, MostrarDatosSinEpisodios) {
    testing::internal::CaptureStdout();
    serie->mostrarDatos();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("No hay episodios") != std::string::npos);
}

// ========== TESTS DE SERVICIO STREAMING ==========
class ServicioStreamingTest : public ::testing::Test {
protected:
    void SetUp() override {
        servicio = new ServicioStreaming();
        // Crear archivo de prueba
        crearArchivoPrueba();
    }
    
    void TearDown() override {
        delete servicio;
        // Limpiar archivo de prueba
        std::remove("test_data.txt");
    }
    
    void crearArchivoPrueba() {
        std::ofstream file("test_data.txt");
        file << "Pelicula,P001,Test Movie,120,Drama,4;5;3\n";
        file << "Serie,S001,Test Series,60,Thriller,5-4;Episode1:1:5-4|Episode2:1:3-2\n";
        file << "Pelicula,P002,Action Movie,90,Accion,5;5;5\n";
        file.close();
    }
    
    ServicioStreaming* servicio;
};

TEST_F(ServicioStreamingTest, CargarArchivoExistente) {
    testing::internal::CaptureStdout();
    servicio->cargarArchivo("test_data.txt");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("cargados exitosamente") != std::string::npos);
}

TEST_F(ServicioStreamingTest, CargarArchivoInexistente) {
    testing::internal::CaptureStderr();
    servicio->cargarArchivo("nonexistent.txt");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(output.find("Error") != std::string::npos);
}

TEST_F(ServicioStreamingTest, CalificarPeliculaExistente) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->calificarVideo("Test Movie", 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("calificado") != std::string::npos);
}

TEST_F(ServicioStreamingTest, CalificarVideoInexistente) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->calificarVideo("Nonexistent Movie", 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("no encontrado") != std::string::npos);
}

TEST_F(ServicioStreamingTest, CalificarEpisodio) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->calificarVideo("Episode1", 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("calificado") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarPeliculasConCalificacion) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarPeliculasConCalificacion(4.5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Action Movie") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarPeliculasConCalificacionAlta) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarPeliculasConCalificacion(5.0);
    std::string output = testing::internal::GetCapturedStdout();
    // Solo Action Movie debería aparecer (calificación 5.0)
    EXPECT_TRUE(output.find("Action Movie") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarEpisodiosDeSerieConCalificacion) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarEpisodiosDeSerieConCalificacion("Test Series", 4.0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Episode1") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarEpisodiosDeSerieInexistente) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarEpisodiosDeSerieConCalificacion("Nonexistent Series", 4.0);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("no encontrada") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarVideosPorCalificacionYGenero) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarVideosPorCalificacionOGenero(4.0, "Drama");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Test Movie") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarVideosPorGeneroSolo) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarVideosPorCalificacionOGenero(0.0, "Accion");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Action Movie") != std::string::npos);
}

TEST_F(ServicioStreamingTest, MostrarVideosSinFiltros) {
    servicio->cargarArchivo("test_data.txt");
    
    testing::internal::CaptureStdout();
    servicio->mostrarVideosPorCalificacionOGenero(0.0, "");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Test Movie") != std::string::npos);
    EXPECT_TRUE(output.find("Test Series") != std::string::npos);
}

// ========== TESTS DE CASOS EDGE ==========
TEST(EdgeCaseTest, ArchivoConDatosInvalidos) {
    std::ofstream file("invalid_data.txt");
    file << "TipoInvalido,ID001,Name,Duration,Genre\n";
    file << "Pelicula,P001,Invalid Duration,invalid,Drama,4;5\n";
    file.close();
    
    ServicioStreaming servicio;
    testing::internal::CaptureStderr();
    servicio.cargarArchivo("invalid_data.txt");
    std::string output = testing::internal::GetCapturedStderr();
    
    EXPECT_TRUE(output.find("Advertencia") != std::string::npos);
    std::remove("invalid_data.txt");
}

TEST(EdgeCaseTest, CalificacionesInvalidasEnArchivo) {
    std::ofstream file("invalid_ratings.txt");
    file << "Pelicula,P001,Test Movie,120,Drama,4;invalid;5\n";
    file.close();
    
    ServicioStreaming servicio;
    testing::internal::CaptureStderr();
    servicio.cargarArchivo("invalid_ratings.txt");
    std::string output = testing::internal::GetCapturedStderr();
    
    EXPECT_TRUE(output.find("Advertencia") != std::string::npos);
    std::remove("invalid_ratings.txt");
}

TEST(EdgeCaseTest, CalificacionPromedioCero) {
    Pelicula p("P001", "Test", 90, "Drama");
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
    p.calcularPromedio(); // Llamar explícitamente
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
}

TEST(EdgeCaseTest, DestructorServicioStreaming) {
    ServicioStreaming* servicio = new ServicioStreaming();
    // El destructor debería limpiar correctamente
    delete servicio;
    // Si llegamos aquí sin crash, el destructor funciona
    SUCCEED();
}