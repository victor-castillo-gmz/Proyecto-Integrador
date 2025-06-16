#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include "pelicula.h"
#include "serie.h"
#include "episodio.h"
#include "serviciostreaming.h"

// --- Pruebas de Clases Individuales ---

// Prueba de calificación y promedio para Pelicula
TEST(PeliculaTest, CalificacionPromedio) {
    Pelicula p("P001", "Inception", 120.0, "Ciencia Ficción");
    p.calificar(4);
    p.calificar(5);
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 4.5);
}

// Prueba para una calificación inválida (fuera del rango 1-5)
TEST(VideoTest, CalificacionInvalida) {
    Pelicula p("P003", "Test Movie", 90.0, "Drama");
    p.calificar(6); // no válida
    p.calificar(0); // no válida
    EXPECT_EQ(p.getCalificaciones().size(), 0);
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
}

// Prueba para un video sin calificaciones
TEST(VideoTest, CalificacionPromedioVacia) {
    Pelicula p("P004", "Unrated Film", 100.0, "Misterio");
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
}

// Prueba de calificación y promedio para Episodio
TEST(EpisodioTest, CalificacionPromedio) {
    Episodio e("Piloto", 1);
    e.calificar(3);
    e.calificar(5);
    EXPECT_DOUBLE_EQ(e.getCalificacionPromedio(), 4.0);
}

// Prueba para verificar la salida de datos de una Película
TEST(PeliculaTest, MostrarDatosOutput) {
    Pelicula p("P001", "Inception", 148.0, "Sci-Fi");
    p.calificar(5);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    p.mostrarDatos();

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_NE(output.find("--- Película ---"), std::string::npos);
    EXPECT_NE(output.find("ID: P001"), std::string::npos);
    EXPECT_NE(output.find("Nombre: Inception"), std::string::npos);
    EXPECT_NE(output.find("Duración: 148.0 min"), std::string::npos);
    EXPECT_NE(output.find("Género: Sci-Fi"), std::string::npos);
    EXPECT_NE(output.find("Calificación promedio: 5.0"), std::string::npos);
}

// Prueba para agregar episodios a una serie y calificar un episodio existente y uno no existente
TEST(SerieTest, AgregarYCalificarEpisodios) {
    Serie s("S001", "Breaking Bad", 45.0, "Drama");
    Episodio ep1("Pilot", 1);
    ep1.calificar(4);
    s.agregarEpisodio(ep1);

    // Calificar episodio existente (case-insensitive)
    s.calificarEpisodio("pilot", 5);
    const auto& episodios = s.getEpisodios();
    ASSERT_EQ(episodios.size(), 1);
    EXPECT_DOUBLE_EQ(episodios[0].getCalificacionPromedio(), 4.5);

    // Intentar calificar un episodio que no existe
    s.calificarEpisodio("Fly", 5);
    // La calificación del episodio original no debe cambiar
    EXPECT_DOUBLE_EQ(episodios[0].getCalificacionPromedio(), 4.5);
}

// --- Pruebas de la clase ServicioStreaming ---

// Fixture para pruebas de ServicioStreaming que necesitan un entorno limpio (redirigir cout/cerr)
class ServicioStreamingTest : public ::testing::Test {
protected:
    ServicioStreaming servicio;
    std::stringstream cout_buffer;
    std::stringstream cerr_buffer;
    std::streambuf* original_cout;
    std::streambuf* original_cerr;
    const char* test_filename = "test_data.txt";

    void SetUp() override {
        // Redirigir cout y cerr para capturar la salida
        original_cout = std::cout.rdbuf(cout_buffer.rdbuf());
        original_cerr = std::cerr.rdbuf(cerr_buffer.rdbuf());
    }

    void TearDown() override {
        // Restaurar cout y cerr
        std::cout.rdbuf(original_cout);
        std::cerr.rdbuf(original_cerr);
        // Eliminar archivo de prueba si existe
        remove(test_filename);
    }

    void CreateTestFile(const std::string& content) {
        std::ofstream outfile(test_filename);
        outfile << content;
        outfile.close();
    }
};

// Prueba de carga de archivo exitosa
TEST_F(ServicioStreamingTest, CargarArchivo_Success) {
    std::string data = "Pelicula,P001,Inception,148,Sci-Fi,5;4\n"
                       "Serie,S001,Stranger Things,50,Misterio,4;5|The Vanishing of Will Byers:1:5-5|The Weirdo on Maple Street:1:4-4\n";
    CreateTestFile(data);

    servicio.cargarArchivo(test_filename);
    const auto& videos = servicio.getVideos();

    ASSERT_EQ(videos.size(), 2);

    Pelicula* p = dynamic_cast<Pelicula*>(videos[0]);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->getNombre(), "Inception");
    EXPECT_NEAR(p->getCalificacionPromedio(), 4.5, 0.01);

    Serie* s = dynamic_cast<Serie*>(videos[1]);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->getNombre(), "Stranger Things");
    EXPECT_NEAR(s->getCalificacionPromedio(), 4.5, 0.01);
    ASSERT_EQ(s->getEpisodios().size(), 2);
    EXPECT_EQ(s->getEpisodios()[0].getTitulo(), "The Vanishing of Will Byers");
    EXPECT_NEAR(s->getEpisodios()[0].getCalificacionPromedio(), 5.0, 0.01);
}

// Prueba de carga de un archivo que no existe
TEST_F(ServicioStreamingTest, CargarArchivo_FileNotFound) {
    servicio.cargarArchivo("non_existent_file.txt");
    std::string error_output = cerr_buffer.str();
    EXPECT_NE(error_output.find("Error: No se pudo abrir el archivo"), std::string::npos);
}

// Prueba de carga de un archivo con datos inválidos (duración no numérica)
TEST_F(ServicioStreamingTest, CargarArchivo_InvalidData) {
    std::string data = "Pelicula,P002,The Godfather,abc,Crimen,5\n";
    CreateTestFile(data);
    servicio.cargarArchivo(test_filename);

    std::string error_output = cerr_buffer.str();
    EXPECT_NE(error_output.find("Advertencia: Duración inválida"), std::string::npos);
    EXPECT_EQ(servicio.getVideos().size(), 0); // El video inválido no debe ser agregado
}

// Prueba para mostrar películas con una calificación mínima
TEST_F(ServicioStreamingTest, MostrarPeliculasConCalificacion) {
    std::string data = "Pelicula,P001,Goodfellas,146,Crimen,5\n"
                       "Pelicula,P002,Home Alone,103,Comedia,3\n";
    CreateTestFile(data);
    servicio.cargarArchivo(test_filename);

    servicio.mostrarPeliculasConCalificacion(4.0);
    std::string output = cout_buffer.str();

    EXPECT_NE(output.find("Goodfellas"), std::string::npos);
    EXPECT_EQ(output.find("Home Alone"), std::string::npos); // No debe aparecer
}

// Prueba para mostrar episodios de una serie con una calificación mínima
TEST_F(ServicioStreamingTest, MostrarEpisodiosDeSerieConCalificacion) {
    std::string data = "Serie,S001,The Office,22,Comedia,5|Pilot:1:3|Diversity Day:1:5\n";
    CreateTestFile(data);
    servicio.cargarArchivo(test_filename);

    // Caso: Serie encontrada
    servicio.mostrarEpisodiosDeSerieConCalificacion("The Office", 4.0);
    std::string output = cout_buffer.str();
    EXPECT_NE(output.find("Diversity Day"), std::string::npos);
    EXPECT_EQ(output.find("Pilot"), std::string::npos); // Calificación muy baja

    cout_buffer.str(""); // Limpiar buffer

    // Caso: Serie no encontrada
    servicio.mostrarEpisodiosDeSerieConCalificacion("Friends", 4.0);
    output = cout_buffer.str();
    EXPECT_NE(output.find("Serie 'Friends' no encontrada."), std::string::npos);
}

// Prueba para calificar un video (película y episodio)
TEST_F(ServicioStreamingTest, CalificarVideo) {
    std::string data = "Pelicula,P001,Interstellar,169,Sci-Fi,5\n"
                       "Serie,S002,Sherlock,90,Misterio,5|A Study in Pink:1:4\n";
    CreateTestFile(data);
    servicio.cargarArchivo(test_filename);

    // Calificar una película
    servicio.calificarVideo("Interstellar", 3);
    const auto& videos = servicio.getVideos();
    Pelicula* p = dynamic_cast<Pelicula*>(videos[0]);
    EXPECT_NEAR(p->getCalificacionPromedio(), 4.0, 0.01);

    // Calificar un episodio (búsqueda case-insensitive)
    servicio.calificarVideo("a study in pink", 5);
    Serie* s = dynamic_cast<Serie*>(videos[1]);
    const auto& episodios = s->getEpisodios();
    EXPECT_NEAR(episodios[0].getCalificacionPromedio(), 4.5, 0.01);
    
    // Calificar video no existente
    cout_buffer.str(""); // Limpiar buffer
    servicio.calificarVideo("Titanic", 5);
    std::string output = cout_buffer.str();
    EXPECT_NE(output.find("Video o episodio 'Titanic' no encontrado."), std::string::npos);

}

// Prueba de filtrado general por género y calificación
TEST_F(ServicioStreamingTest, MostrarVideosPorCalificacionOGenero) {
    std::string data = "Pelicula,P001,Joker,122,Drama,4\n"
                       "Pelicula,P002,Parasite,132,Thriller,5\n"
                       "Serie,S001,Chernobyl,60,Drama,5\n";
    CreateTestFile(data);
    servicio.cargarArchivo(test_filename);
    
    // Filtrar por género "Drama"
    servicio.mostrarVideosPorCalificacionOGenero(0.0, "Drama");
    std::string output = cout_buffer.str();

    EXPECT_NE(output.find("Joker"), std::string::npos);
    EXPECT_NE(output.find("Chernobyl"), std::string::npos);
    EXPECT_EQ(output.find("Parasite"), std::string::npos);
}