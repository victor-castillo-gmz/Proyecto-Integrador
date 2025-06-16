#include <gtest/gtest.h>
#include "pelicula.h"
#include "serie.h"
#include "episodio.h"
#include "serviciostreaming.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdio>

// Helper function to create a dummy data file for testing
void create_dummy_data_file(const std::string& filename, const std::string& content) { // Renamed
    std::ofstream file(filename);
    file << content;
    file.close();
}

// Helper function to clean up dummy data file
void cleanup_dummy_data_file(const std::string& filename) { // Renamed
    std::remove(filename.c_str());
}

// Helper for redirecting both cout and cerr
struct StreamRedirector {
    std::stringstream ss_out;
    std::stringstream ss_err;
    std::streambuf* old_cout;
    std::streambuf* old_cerr;

    StreamRedirector() {
        old_cout = std::cout.rdbuf();
        old_cerr = std::cerr.rdbuf();
        std::cout.rdbuf(ss_out.rdbuf());
        std::cerr.rdbuf(ss_err.rdbuf());
    }

    ~StreamRedirector() {
        std::cout.rdbuf(old_cout);
        std::cerr.rdbuf(old_cerr);
    }

    std::string GetCout() { // Renamed
        return ss_out.str();
    }

    std::string GetCerr() { // Renamed
        return ss_err.str();
    }

    void Clear() { // Renamed
        ss_out.str("");
        ss_out.clear();
        ss_err.str("");
        ss_err.clear();
    }
};

// --- Video Class Tests ---

// Test: Calificación promedio correcta para un video
TEST(VideoTest, CalificacionPromedioCorrecta) {
    Pelicula p("P001", "Inception", 120.0, "Ciencia Ficcion");
    p.calificar(4);
    p.calificar(5);
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 4.5);
}

// Test: Calificación inválida no se agrega
TEST(VideoTest, CalificacionInvalida) {
    StreamRedirector redirector; // Capture output
    Pelicula p("P003", "Test", 90.0, "Drama");
    p.calificar(6); // Invalid rating
    p.calificar(0); // Invalid rating
    EXPECT_TRUE(p.getCalificaciones().empty()); // No ratings should be added
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
    EXPECT_TRUE(redirector.GetCout().find("Calificación inválida. Debe ser de 1 a 5.") != std::string::npos);
}

// Test: Calificación promedio con un solo valor
TEST(VideoTest, CalificacionUnica) {
    Pelicula p("P004", "Solo Rating", 90.0, "Comedia");
    p.calificar(3);
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 3.0);
}

// Test: Calificación promedio de video sin calificaciones
TEST(VideoTest, CalificacionPromedioVacio) {
    Pelicula p("P005", "Sin Calificar", 100.0, "Misterio");
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
}

// --- Episodio Class Tests ---

// Test: Calificación promedio correcta para un episodio
TEST(EpisodioTest, CalificacionPromedioCorrecta) {
    Episodio e("Piloto", 1);
    e.calificar(3);
    e.calificar(5);
    EXPECT_DOUBLE_EQ(e.getCalificacionPromedio(), 4.0);
}

// Test: Calificación inválida en episodio no se agrega
TEST(EpisodioTest, CalificacionEpisodioInvalida) {
    StreamRedirector redirector; // Capture output
    Episodio e("Final", 2);
    e.calificar(0);
    e.calificar(6);
    EXPECT_TRUE(e.getCalificaciones().empty());
    EXPECT_DOUBLE_EQ(e.getCalificacionPromedio(), 0.0);
    EXPECT_TRUE(redirector.GetCout().find("Calificación de episodio inválida. Debe ser de 1 a 5.") != std::string::npos);
}

// Test: Getters de Episodio
TEST(EpisodioTest, Getters) {
    Episodio e("El Comienzo", 1);
    EXPECT_EQ(e.getTitulo(), "El Comienzo");
    EXPECT_EQ(e.getTemporada(), 1);
}

// --- Pelicula Class Tests ---

// Test: Mostrar datos de Pelicula
TEST(PeliculaTest, MostrarDatos) {
    Pelicula p("P006", "Gran Pelicula", 150.0, "Accion");
    p.calificar(5);
    p.calificar(4);

    StreamRedirector redirector;

    p.mostrarDatos();

    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("ID: P006") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Gran Pelicula") != std::string::npos);
    EXPECT_TRUE(output.find("Duración: 150.0 min") != std::string::npos || output.find("Duración: 150 min") != std::string::npos);
    EXPECT_TRUE(output.find("Género: Accion") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.5") != std::string::npos);
}

// --- Serie Class Tests ---

// Test: Agregar y calificar episodios en serie
TEST(SerieTest, AgregarYCalificarEpisodios) {
    Serie s("S001", "Breaking Bad", 60.0, "Drama");
    Episodio ep1("Piloto", 1);
    ep1.calificar(4);
    s.AgregarEpisodio(ep1); // Renamed call

    StreamRedirector redirector; // Capture output for calificarEpisodio

    // Calificar un episodio existente en la serie
    s.CalificarEpisodio("Piloto", 5); // Renamed call
    const auto& episodios = s.GetEpisodios(); // Renamed call
    ASSERT_EQ(episodios.size(), 1);
    EXPECT_DOUBLE_EQ(episodios[0].getCalificacionPromedio(), 4.5);
    EXPECT_TRUE(redirector.GetCout().find("Episodio 'Piloto' calificado. Nueva calificación promedio: 4.5") != std::string::npos);

    redirector.Clear(); // Clear stream for next check

    // Calificar un episodio que no existe
    s.CalificarEpisodio("Episodio Falso", 3); // Renamed call
    EXPECT_TRUE(redirector.GetCout().find("Episodio 'Episodio Falso' no encontrado en esta serie.") != std::string::npos);
}

// Test: Mostrar datos de Serie
TEST(SerieTest, MostrarDatos) {
    Serie s("S002", "The Crown", 55.0, "Drama Historico");
    s.calificar(4); // Calificar la serie en sí
    Episodio ep1("Ep1 S1", 1);
    Episodio ep2("Ep2 S1", 1);
    ep1.calificar(4);
    ep2.calificar(5);
    s.AgregarEpisodio(ep1); // Renamed call
    s.AgregarEpisodio(ep2); // Renamed call

    StreamRedirector redirector;

    s.mostrarDatos();

    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("--- Serie ---") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: The Crown") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.0") != std::string::npos); // Serie's own rating
    EXPECT_TRUE(output.find("Episodios:") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep1 S1, Temporada: 1, Calificación: 4.0") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep2 S1, Temporada: 1, Calificación: 5.0") != std::string::npos);
}

// Test: Mostrar episodios de serie vacía
TEST(SerieTest, MostrarEpisodiosVacios) {
    Serie s("S003", "Serie Vacia", 30.0, "Animacion");
    StreamRedirector redirector;

    s.MostrarEpisodios(); // Renamed call
    EXPECT_TRUE(redirector.GetCout().find("No hay episodios en esta serie.") != std::string::npos);

    redirector.Clear(); // Clear stringstream

    // Also test via mostrarDatos()
    s.mostrarDatos(); // This will call s.mostrarEpisodios() internally, which prints "No hay episodios en esta serie."
    EXPECT_TRUE(redirector.GetCout().find("No hay episodios en esta serie.") != std::string::npos);
}

// Test: Mostrar episodios con calificación mínima
TEST(SerieTest, MostrarEpisodiosConCalificacion) {
    Serie s("S004", "Serie Filtrada", 45.0, "Aventura");
    Episodio ep1("Ep A", 1);
    ep1.calificar(3);
    Episodio ep2("Ep B", 1);
    ep2.calificar(4);
    Episodio ep3("Ep C", 2);
    ep3.calificar(5);
    s.AgregarEpisodio(ep1); // Renamed call
    s.AgregarEpisodio(ep2); // Renamed call
    s.AgregarEpisodio(ep3); // Renamed call

    StreamRedirector redirector;

    // First part: Test finding episodes that match the criteria (e.g., >= 4.0)
    s.MostrarEpisodiosConCalificacion(4.0); // Renamed call

    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Título: Ep B, Temporada: 1, Calificación: 4.0") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep C, Temporada: 2, Calificación: 5.0") != std::string::npos);
    EXPECT_FALSE(output.find("Título: Ep A") != std::string::npos); // Should not be in output

    redirector.Clear(); // Clear stream for next check

    // Second part: Test with no episodes meeting the criteria
    s.MostrarEpisodiosConCalificacion(6.0); // No episode will have a 6.0 rating (Renamed call)

    // Ahora esperamos el mensaje de "no encontrados"
    EXPECT_TRUE(redirector.GetCout().find("No se encontraron episodios con esa calificación.") != std::string::npos);
}


// --- ServicioStreaming Class Tests ---

// Test: Cargar archivo con películas y series
TEST(ServicioStreamingTest, CargarArchivo) {
    const std::string filename = "test_data_load.txt";
    const std::string content =
        "Pelicula,P001,Movie A,90.0,Action,5;4\n"
        "Serie,S001,Series B,45.0,Drama,3-4;Ep1:1:5-4|Ep2:1:3\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // To capture success message

    servicio.cargarArchivo(filename);

    std::string output_cout = redirector.GetCout();
    EXPECT_TRUE(output_cout.find("Datos cargados exitosamente desde test_data_load.txt. Total de videos: 2") != std::string::npos);

    // We can indirectly check by trying to find/display them
    redirector.Clear(); // Clear for next operation
    servicio.mostrarVideosPorCalificacionOGenero(0.0, ""); // Show all
    output_cout = redirector.GetCout();

    EXPECT_TRUE(output_cout.find("Nombre: Movie A") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Calificación promedio: 4.5") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Nombre: Series B") != std::string::npos);
    // Split long line
    EXPECT_TRUE(output_cout.find("Título: Ep1, Temporada: 1, Calificación: 4.5") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Título: Ep2, Temporada: 1, Calificación: 3.0") != std::string::npos);

    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Cargar archivo que no existe
TEST(ServicioStreamingTest, CargarArchivoNoExistente) {
    ServicioStreaming servicio;
    StreamRedirector redirector;

    servicio.cargarArchivo("non_existent_file.txt");
    EXPECT_TRUE(redirector.GetCerr().find("Error: No se pudo abrir el archivo non_existent_file.txt") != std::string::npos);
}

// Test: Cargar archivo con formato de duración inválido
TEST(ServicioStreamingTest, CargarArchivoDuracionInvalida) {
    const std::string filename = "test_data_invalid_duration.txt";
    const std::string content = "Pelicula,P001,Movie A,invalid_duration,Action,5;4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector;

    servicio.cargarArchivo(filename);
    EXPECT_TRUE(redirector.GetCerr().find("Advertencia: Duración inválida en la línea: Pelicula,P001,Movie A,invalid_duration,Action,5;4") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Cargar archivo con tipo de video desconocido
TEST(ServicioStreamingTest, CargarArchivoTipoDesconocido) {
    const std::string filename = "test_data_unknown_type.txt";
    const std::string content = "UnknownType,U001,Video X,60.0,Comedy,3\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector;

    servicio.cargarArchivo(filename);
    EXPECT_TRUE(redirector.GetCerr().find("Advertencia: Tipo de video desconocido en la línea: UnknownType,U001,Video X,60.0,Comedy,3") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Cargar archivo con calificaciones inválidas para Pelicula
TEST(ServicioStreamingTest, CargarArchivoPeliculaCalificacionInvalida) {
    const std::string filename = "test_data_invalid_movie_rating.txt";
    const std::string content = "Pelicula,P001,Movie A,90.0,Action,5;invalid;4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector;
    servicio.cargarArchivo(filename);
    EXPECT_TRUE(redirector.GetCerr().find("Advertencia: Calificación inválida para Pelicula 'Movie A': 'invalid'") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Cargar archivo con calificaciones inválidas para Serie
TEST(ServicioStreamingTest, CargarArchivoSerieCalificacionInvalida) {
    const std::string filename = "test_data_invalid_series_rating.txt";
    const std::string content = "Serie,S001,Series B,45.0,Drama,3-invalid;Ep1:1:5\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector;
    servicio.cargarArchivo(filename);
    EXPECT_TRUE(redirector.GetCerr().find("Advertencia: Calificación inválida para Serie 'Series B': 'invalid'") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Cargar archivo con formato de segmento de episodio inválido
TEST(ServicioStreamingTest, CargarArchivoEpisodioSegmentoInvalido) {
    const std::string filename = "test_data_invalid_episode_segment.txt";
    const std::string content = "Serie,S001,Series C,45.0,Drama,3;Ep1:invalid_season:5|Ep2:1:3\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector;
    servicio.cargarArchivo(filename);
    EXPECT_TRUE(redirector.GetCerr().find("Advertencia: Temporada inválida para episodio 'Ep1' en 'Series C': 'invalid_season'") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Calificar una película existente (caso real después de cargar)
TEST(ServicioStreamingTest, CalificarPeliculaExistente) {
    const std::string filename = "test_data_rate_movie.txt";
    const std::string content = "Pelicula,P001,Movie To Rate,90.0,Action,4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.calificarVideo("Movie To Rate", 5);
    EXPECT_TRUE(redirector.GetCout().find("Video 'Movie To Rate' calificado. Nueva calificación promedio: 4.5") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Calificar una película inexistente
TEST(ServicioStreamingTest, CalificarPeliculaInexistente) {
    ServicioStreaming servicio; // Empty service
    StreamRedirector redirector;
    servicio.calificarVideo("Non Existent Movie", 3);
    EXPECT_TRUE(redirector.GetCout().find("Video o episodio 'Non Existent Movie' no encontrado.") != std::string::npos);
}

// Test: Calificar un episodio existente (caso real después de cargar)
TEST(ServicioStreamingTest, CalificarEpisodioExistente) {
    const std::string filename = "test_data_rate_episode.txt";
    const std::string content = "Serie,S001,Series With Episodes,60.0,Drama,4;Ep1:1:3|Ep2:1:5\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.calificarVideo("Ep1", 4); // Calificar el primer episodio
    EXPECT_TRUE(redirector.GetCout().find("Episodio 'Ep1' de la serie 'Series With Episodes' calificado. Nueva calificación promedio: 3.5") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Calificar un episodio inexistente dentro de una serie existente
TEST(ServicioStreamingTest, CalificarEpisodioInexistenteEnSerieExistente) {
    const std::string filename = "test_data_rate_episode_no_exist.txt";
    const std::string content = "Serie,S001,Another Series,60.0,Drama,4;Ep1:1:3\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.calificarVideo("Non Existent Episode", 4);
    EXPECT_TRUE(redirector.GetCout().find("Video o episodio 'Non Existent Episode' no encontrado.") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Mostrar videos por calificación mínima
TEST(ServicioStreamingTest, MostrarVideosPorCalificacion) {
    const std::string filename = "test_data_filter_rating.txt";
    const std::string content =
        "Pelicula,P001,High Rated Movie,100.0,Action,5;5\n"
        "Pelicula,P002,Medium Movie,90.0,Comedy,3;4\n"
        "Serie,S001,High Rated Series,60.0,Drama,5;Ep1:1:5\n"
        "Serie,S002,Low Rated Series,45.0,SciFi,1;Ep1:1:2\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarVideosPorCalificacionOGenero(4.5, ""); // Filter by rating only

    std::string output = redirector.GetCout();

    EXPECT_TRUE(output.find("Nombre: High Rated Movie") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Medium Movie") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: High Rated Series") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Low Rated Series") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Mostrar videos por género
TEST(ServicioStreamingTest, MostrarVideosPorGenero) {
    const std::string filename = "test_data_filter_genre.txt";
    const std::string content =
        "Pelicula,P001,Action Movie,100.0,Action,5\n"
        "Pelicula,P002,Comedy Movie,90.0,Comedy,4\n"
        "Serie,S001,Action Series,60.0,Action,4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarVideosPorCalificacionOGenero(0.0, "Action"); // Filter by genre only

    std::string output = redirector.GetCout();

    EXPECT_TRUE(output.find("Nombre: Action Movie") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Comedy Movie") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Action Series") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Mostrar videos por calificación y género (combinado)
TEST(ServicioStreamingTest, MostrarVideosPorCalificacionYGenero) {
    const std::string filename = "test_data_filter_combined.txt";
    const std::string content =
        "Pelicula,P001,Movie A,100.0,Drama,5\n"
        "Pelicula,P002,Movie B,90.0,Drama,3\n"
        "Pelicula,P003,Movie C,80.0,Action,4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarVideosPorCalificacionOGenero(4.0, "Drama");

    std::string output = redirector.GetCout();

    EXPECT_TRUE(output.find("Nombre: Movie A") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Movie B") != std::string::npos); // Rating too low
    EXPECT_FALSE(output.find("Nombre: Movie C") != std::string::npos); // Wrong genre
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: No se encuentran videos con los criterios especificados
TEST(ServicioStreamingTest, MostrarVideosNoEncontrados) {
    ServicioStreaming servicio; // Empty service
    StreamRedirector redirector;
    servicio.mostrarVideosPorCalificacionOGenero(4.0, "NonExistentGenre");
    EXPECT_TRUE(redirector.GetCout().find("No se encontraron videos con los criterios especificados.") != std::string::npos);
}

// Test: Mostrar episodios de una serie con calificación específica
TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieConCalificacion) {
    const std::string filename = "test_data_series_episodes.txt";
    const std::string content = "Serie,S001,My Awesome Series,60.0,Action,4;Ep1:1:5|Ep2:1:3|Ep3:2:4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarEpisodiosDeSerieConCalificacion("My Awesome Series", 4.0);
    std::string output = redirector.GetCout();

    EXPECT_TRUE(output.find("Título: Ep1, Temporada: 1, Calificación: 5.0") != std::string::npos);
    EXPECT_FALSE(output.find("Título: Ep2, Temporada: 1, Calificación: 3.0") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep3, Temporada: 2, Calificación: 4.0") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Intentar mostrar episodios de una serie que no existe
TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieNoExistente) {
    ServicioStreaming servicio;
    StreamRedirector redirector;
    servicio.mostrarEpisodiosDeSerieConCalificacion("Non Existent Series", 3.0);
    EXPECT_TRUE(redirector.GetCout().find("Serie 'Non Existent Series' no encontrada.") != std::string::npos);
}

// Test: Mostrar películas con calificación específica
TEST(ServicioStreamingTest, MostrarPeliculasConCalificacion) {
    const std::string filename = "test_data_movies_filter.txt";
    const std::string content =
        "Pelicula,P001,Movie High,90.0,Drama,5\n"
        "Pelicula,P002,Movie Low,80.0,Comedy,2\n"
        "Pelicula,P003,Movie Medium,110.0,Action,4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarPeliculasConCalificacion(3.5);
    std::string output = redirector.GetCout();

    EXPECT_TRUE(output.find("Nombre: Movie High") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Movie Low") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Movie Medium") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: No se encuentran películas con la calificación especificada
TEST(ServicioStreamingTest, MostrarPeliculasNoEncontradas) {
    ServicioStreaming servicio;
    const std::string filename = "test_data_no_movies.txt";
    const std::string content = "Pelicula,P001,Movie Low,80.0,Comedy,2\n"; // Only low rated movie
    create_dummy_data_file(filename, content); // Renamed call

    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarPeliculasConCalificacion(4.0);
    EXPECT_TRUE(redirector.GetCout().find("No se encontraron películas con calificación >= 4.0.") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Destructor de ServicioStreaming libera memoria
TEST(ServicioStreamingTest, DestructorLiberaMemoria) {
    ServicioStreaming* servicio = new ServicioStreaming();

    const std::string filename = "temp_for_dtor.txt";
    const std::string content = "Pelicula,P001,Temp Movie,90.0,Action,5\n";
    create_dummy_data_file(filename, content); // Renamed call

    StreamRedirector redirector; // Capture output for loading success/errors during setup
    servicio->cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    // When 'servicio' is deleted, its destructor should free the allocated Video objects
    delete servicio;
    cleanup_dummy_data_file(filename); // Renamed call
    // No explicit EXPECT_ or ASSERT_ here as memory deallocation is hard to test directly
}

// Test: Case-insensitive search for calificarVideo (Movie)
TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveMovie) {
    const std::string filename = "test_data_case_movie.txt";
    const std::string content = "Pelicula,P001,a MoVie,90.0,Action,4\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.calificarVideo("A mOvIe", 5);
    EXPECT_TRUE(redirector.GetCout().find("Video 'a MoVie' calificado. Nueva calificación promedio: 4.5") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Case-insensitive search for calificarVideo (Episode)
TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveEpisode) {
    const std::string filename = "test_data_case_episode.txt";
    const std::string content = "Serie,S001,My SERIES,60.0,Drama,4;ePisoDe1:1:3|EpisOde2:1:5\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.calificarVideo("ePiSoDe1", 4); // Different casing
    EXPECT_TRUE(redirector.GetCout().find("Episodio 'ePisoDe1' de la serie 'My SERIES' calificado. Nueva calificación promedio: 3.5") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}

// Test: Case-insensitive search for mostrarEpisodiosDeSerieConCalificacion
TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieCaseInsensitive) {
    const std::string filename = "test_data_series_case.txt";
    const std::string content = "Serie,S001,CaSe SeNsItIvE SeRiEs,60.0,Action,4;Ep1:1:5|Ep2:1:3\n";
    create_dummy_data_file(filename, content); // Renamed call

    ServicioStreaming servicio;
    StreamRedirector redirector; // Capture output for loading success
    servicio.cargarArchivo(filename);
    redirector.Clear(); // Clear output from loading

    servicio.mostrarEpisodiosDeSerieConCalificacion("cAsE sEnSiTiVe SeRiEs", 4.0); // Different casing
    std::string output = redirector.GetCout();

    EXPECT_TRUE(output.find("Título: Ep1, Temporada: 1, Calificación: 5.0") != std::string::npos);
    cleanup_dummy_data_file(filename); // Renamed call
}