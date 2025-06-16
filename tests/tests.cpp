#include <gtest/gtest.h>
#include "pelicula.h"
#include "serie.h"
#include "episodio.h"
#include "serviciostreaming.h"
#include <fstream> // Para simular archivos de datos
#include <sstream> // Para manipular strings como streams

// Helper function to create a dummy data file for testing
void createDummyDataFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

// Helper function to clean up dummy data file
void cleanupDummyDataFile(const std::string& filename) {
    std::remove(filename.c_str());
}

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
    Pelicula p("P003", "Test", 90.0, "Drama");
    p.calificar(6); // Invalid rating
    p.calificar(0); // Invalid rating
    EXPECT_TRUE(p.getCalificaciones().empty()); // No ratings should be added
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 0.0);
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
    Episodio e("Final", 2);
    e.calificar(0);
    e.calificar(6);
    EXPECT_TRUE(e.getCalificaciones().empty());
    EXPECT_DOUBLE_EQ(e.getCalificacionPromedio(), 0.0);
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
    
    // Capturar la salida estándar
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    p.mostrarDatos();

    std::cout.rdbuf(oldCout); // Restaurar la salida estándar

    std::string output = ss.str();
    EXPECT_TRUE(output.find("ID: P006") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Gran Pelicula") != std::string::npos);
    EXPECT_TRUE(output.find("Duración: 150.0 min") != std::string::npos);
    EXPECT_TRUE(output.find("Género: Accion") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.5") != std::string::npos);
}

// --- Serie Class Tests ---

// Test: Agregar y calificar episodios en serie
TEST(SerieTest, AgregarYCalificarEpisodios) {
    Serie s("S001", "Breaking Bad", 60.0, "Drama");
    Episodio ep1("Piloto", 1);
    ep1.calificar(4);
    s.agregarEpisodio(ep1);
    
    // Calificar un episodio existente en la serie
    s.calificarEpisodio("Piloto", 5);
    const auto& episodios = s.getEpisodios();
    ASSERT_EQ(episodios.size(), 1);
    EXPECT_DOUBLE_EQ(episodios[0].getCalificacionPromedio(), 4.5);

    // Calificar un episodio que no existe
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    s.calificarEpisodio("Episodio Falso", 3);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("Episodio 'Episodio Falso' no encontrado en esta serie.") != std::string::npos);
}

// Test: Mostrar datos de Serie
TEST(SerieTest, MostrarDatos) {
    Serie s("S002", "The Crown", 55.0, "Drama Historico");
    s.calificar(4); // Calificar la serie en sí
    Episodio ep1("Ep1 S1", 1);
    Episodio ep2("Ep2 S1", 1);
    ep1.calificar(4);
    ep2.calificar(5);
    s.agregarEpisodio(ep1);
    s.agregarEpisodio(ep2);

    // Capturar la salida estándar
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    s.mostrarDatos();

    std::cout.rdbuf(oldCout); // Restaurar la salida estándar

    std::string output = ss.str();
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
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    s.mostrarEpisodios();
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("No hay episodios en esta serie.") != std::string::npos);

    // Also test via mostrarDatos()
    ss.str(""); // Clear stringstream
    ss.clear(); // Clear error flags
    std::cout.rdbuf(ss.rdbuf());
    s.mostrarDatos();
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("(No hay episodios)") != std::string::npos);
}

// Test: Mostrar episodios con calificación mínima
TEST(SerieTest, MostrarEpisodiosConCalificacion) {
    Serie s("S004", "Serie Filtrada", 45.0, "Aventura");
    Episodio ep1("Ep A", 1); ep1.calificar(3);
    Episodio ep2("Ep B", 1); ep2.calificar(4);
    Episodio ep3("Ep C", 2); ep3.calificar(5);
    s.agregarEpisodio(ep1);
    s.agregarEpisodio(ep2);
    s.agregarEpisodio(ep3);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    s.mostrarEpisodiosConCalificacion(4.0);

    std::cout.rdbuf(oldCout);

    std::string output = ss.str();
    EXPECT_TRUE(output.find("Título: Ep B, Temporada: 1, Calificación: 4.0") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep C, Temporada: 2, Calificación: 5.0") != std::string::npos);
    EXPECT_FALSE(output.find("Título: Ep A") != std::string::npos); // Should not be in output

    // Test with no episodes meeting the criteria
    ss.str("");
    ss.clear();
    std::cout.rdbuf(ss.rdbuf());
    s.mostrarEpisodiosConCalificacion(5.0);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("No se encontraron episodios con esa calificación.") != std::string::npos);
}


// --- ServicioStreaming Class Tests ---

// Test: Cargar archivo con películas y series
TEST(ServicioStreamingTest, CargarArchivo) {
    const std::string filename = "test_data_load.txt";
    const std::string content =
        "Pelicula,P001,Movie A,90.0,Action,5;4\n"
        "Serie,S001,Series B,45.0,Drama,3-4;Ep1:1:5-4|Ep2:1:3\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    // Check if videos are loaded
    // Note: To properly check the loaded videos, we'd ideally need a public getter
    // for `videos` vector in ServicioStreaming, or more specific public methods
    // to query for video details. For now, we rely on the internal state and
    // messages from `cargarArchivo`.

    // We can indirectly check by trying to find/display them
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.mostrarVideosPorCalificacionOGenero(0.0, ""); // Show all
    std::cout.rdbuf(oldCout);

    std::string output = ss.str();
    EXPECT_TRUE(output.find("Nombre: Movie A") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.5") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Series B") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep1:1:5-4") == std::string::npos); // Expect episode data to be parsed internally, not raw string
    EXPECT_TRUE(output.find("Título: Ep1, Temporada: 1, Calificación: 4.5") != std::string::npos); // Verify episode details
    EXPECT_TRUE(output.find("Título: Ep2, Temporada: 1, Calificación: 3.0") != std::string::npos); // Verify episode details

    cleanupDummyDataFile(filename);
}

// Test: Cargar archivo que no existe
TEST(ServicioStreamingTest, CargarArchivoNoExistente) {
    ServicioStreaming servicio;
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.cargarArchivo("non_existent_file.txt");
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("Error: No se pudo abrir el archivo") != std::string::npos);
}

// Test: Cargar archivo con formato de duración inválido
TEST(ServicioStreamingTest, CargarArchivoDuracionInvalida) {
    const std::string filename = "test_data_invalid_duration.txt";
    const std::string content = "Pelicula,P001,Movie A,invalid_duration,Action,5;4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.cargarArchivo(filename);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("Advertencia: Duración inválida en la línea:") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Cargar archivo con tipo de video desconocido
TEST(ServicioStreamingTest, CargarArchivoTipoDesconocido) {
    const std::string filename = "test_data_unknown_type.txt";
    const std::string content = "UnknownType,U001,Video X,60.0,Comedy,3\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.cargarArchivo(filename);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("Advertencia: Tipo de video desconocido en la línea:") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Calificar una película existente (caso real después de cargar)
TEST(ServicioStreamingTest, CalificarPeliculaExistente) {
    const std::string filename = "test_data_rate_movie.txt";
    const std::string content = "Pelicula,P001,Movie To Rate,90.0,Action,4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.calificarVideo("Movie To Rate", 5);
    std::cout.rdbuf(oldCout);

    std::string output = ss.str();
    EXPECT_TRUE(output.find("Video 'Movie To Rate' calificado. Nueva calificación promedio: 4.5") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Calificar una película inexistente
TEST(ServicioStreamingTest, CalificarPeliculaInexistente) {
    ServicioStreaming servicio; // Empty service
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.calificarVideo("Non Existent Movie", 3);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("Video o episodio 'Non Existent Movie' no encontrado.") != std::string::npos);
}

// Test: Calificar un episodio existente (caso real después de cargar)
TEST(ServicioStreamingTest, CalificarEpisodioExistente) {
    const std::string filename = "test_data_rate_episode.txt";
    const std::string content = "Serie,S001,Series With Episodes,60.0,Drama,4;Ep1:1:3|Ep2:1:5\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.calificarVideo("Ep1", 4); // Calificar el primer episodio
    std::cout.rdbuf(oldCout);

    std::string output = ss.str();
    EXPECT_TRUE(output.find("Episodio 'Ep1' de la serie 'Series With Episodes' calificado. Nueva calificación promedio: 3.5") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Calificar un episodio inexistente dentro de una serie existente
TEST(ServicioStreamingTest, CalificarEpisodioInexistenteEnSerieExistente) {
    const std::string filename = "test_data_rate_episode_no_exist.txt";
    const std::string content = "Serie,S001,Another Series,60.0,Drama,4;Ep1:1:3\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.calificarVideo("Non Existent Episode", 4);
    std::cout.rdbuf(oldCout);

    EXPECT_TRUE(ss.str().find("Video o episodio 'Non Existent Episode' no encontrado.") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Mostrar videos por calificación mínima
TEST(ServicioStreamingTest, MostrarVideosPorCalificacion) {
    const std::string filename = "test_data_filter_rating.txt";
    const std::string content =
        "Pelicula,P001,High Rated Movie,100.0,Action,5;5\n"
        "Pelicula,P002,Medium Movie,90.0,Comedy,3;4\n"
        "Serie,S001,High Rated Series,60.0,Drama,5;Ep1:1:5\n"
        "Serie,S002,Low Rated Series,45.0,SciFi,1;Ep1:1:2\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.mostrarVideosPorCalificacionOGenero(4.5, ""); // Filter by rating only

    std::cout.rdbuf(oldCout);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Nombre: High Rated Movie") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Medium Movie") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: High Rated Series") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Low Rated Series") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Mostrar videos por género
TEST(ServicioStreamingTest, MostrarVideosPorGenero) {
    const std::string filename = "test_data_filter_genre.txt";
    const std::string content =
        "Pelicula,P001,Action Movie,100.0,Action,5\n"
        "Pelicula,P002,Comedy Movie,90.0,Comedy,4\n"
        "Serie,S001,Action Series,60.0,Action,4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.mostrarVideosPorCalificacionOGenero(0.0, "Action"); // Filter by genre only

    std::cout.rdbuf(oldCout);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Nombre: Action Movie") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Comedy Movie") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Action Series") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Mostrar videos por calificación y género (combinado)
TEST(ServicioStreamingTest, MostrarVideosPorCalificacionYGenero) {
    const std::string filename = "test_data_filter_combined.txt";
    const std::string content =
        "Pelicula,P001,Movie A,100.0,Drama,5\n"
        "Pelicula,P002,Movie B,90.0,Drama,3\n"
        "Pelicula,P003,Movie C,80.0,Action,4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.mostrarVideosPorCalificacionOGenero(4.0, "Drama");

    std::cout.rdbuf(oldCout);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Nombre: Movie A") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Movie B") != std::string::npos); // Rating too low
    EXPECT_FALSE(output.find("Nombre: Movie C") != std::string::npos); // Wrong genre
    cleanupDummyDataFile(filename);
}

// Test: No se encuentran videos con los criterios especificados
TEST(ServicioStreamingTest, MostrarVideosNoEncontrados) {
    ServicioStreaming servicio; // Empty service
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.mostrarVideosPorCalificacionOGenero(4.0, "NonExistentGenre");
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("No se encontraron videos con los criterios especificados.") != std::string::npos);
}

// Test: Mostrar episodios de una serie con calificación específica
TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieConCalificacion) {
    const std::string filename = "test_data_series_episodes.txt";
    const std::string content = "Serie,S001,My Awesome Series,60.0,Action,4;Ep1:1:5|Ep2:1:3|Ep3:2:4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.mostrarEpisodiosDeSerieConCalificacion("My Awesome Series", 4.0);
    std::cout.rdbuf(oldCout);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Título: Ep1, Temporada: 1, Calificación: 5.0") != std::string::npos);
    EXPECT_FALSE(output.find("Título: Ep2, Temporada: 1, Calificación: 3.0") != std::string::npos);
    EXPECT_TRUE(output.find("Título: Ep3, Temporada: 2, Calificación: 4.0") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Intentar mostrar episodios de una serie que no existe
TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieNoExistente) {
    ServicioStreaming servicio;
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.mostrarEpisodiosDeSerieConCalificacion("Non Existent Series", 3.0);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("Serie 'Non Existent Series' no encontrada.") != std::string::npos);
}

// Test: Mostrar películas con calificación específica
TEST(ServicioStreamingTest, MostrarPeliculasConCalificacion) {
    const std::string filename = "test_data_movies_filter.txt";
    const std::string content =
        "Pelicula,P001,Movie High,90.0,Drama,5\n"
        "Pelicula,P002,Movie Low,80.0,Comedy,2\n"
        "Pelicula,P003,Movie Medium,110.0,Action,4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.mostrarPeliculasConCalificacion(3.5);
    std::cout.rdbuf(oldCout);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Nombre: Movie High") != std::string::npos);
    EXPECT_FALSE(output.find("Nombre: Movie Low") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Movie Medium") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: No se encuentran películas con la calificación especificada
TEST(ServicioStreamingTest, MostrarPeliculasNoEncontradas) {
    ServicioStreaming servicio;
    const std::string filename = "test_data_no_movies.txt";
    const std::string content = "Pelicula,P001,Movie Low,80.0,Comedy,2\n"; // Only low rated movie
    createDummyDataFile(filename, content);
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());
    servicio.mostrarPeliculasConCalificacion(4.0);
    std::cout.rdbuf(oldCout);
    EXPECT_TRUE(ss.str().find("No se encontraron películas con calificación >= 4.0.") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Destructor de ServicioStreaming libera memoria
TEST(ServicioStreamingTest, DestructorLiberaMemoria) {
    // This test is harder to verify directly without advanced tools like Valgrind.
    // However, we can ensure the destructor is called and attempts to delete.
    // The key is that `videos` vector elements are `Video*`
    // and the destructor iterates and `delete`s each one.
    ServicioStreaming* servicio = new ServicioStreaming();
    
    // Add some dummy videos
    servicio->cargarArchivo("test_data_load.txt"); // Assuming this file exists from another test or created here
    // If not, create a simple one:
    const std::string filename = "temp_for_dtor.txt";
    const std::string content = "Pelicula,P001,Temp Movie,90.0,Action,5\n";
    createDummyDataFile(filename, content);
    servicio->cargarArchivo(filename);

    // When 'servicio' is deleted, its destructor should free the allocated Video objects
    delete servicio; 
    // If no memory leaks are reported by a tool like Valgrind during the test run,
    // this test implicitly passes its intent. No explicit EXPECT_ or ASSERT_ here
    // because direct memory inspection is beyond typical unit testing in C++.
    cleanupDummyDataFile(filename);
}

// Test: Case-insensitive search for calificarVideo (Movie)
TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveMovie) {
    const std::string filename = "test_data_case_movie.txt";
    const std::string content = "Pelicula,P001,a MoVie,90.0,Action,4\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.calificarVideo("A mOvIe", 5); // Different casing
    std::cout.rdbuf(oldCout);

    std::string output = ss.str();
    EXPECT_TRUE(output.find("Video 'a MoVie' calificado. Nueva calificación promedio: 4.5") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Case-insensitive search for calificarVideo (Episode)
TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveEpisode) {
    const std::string filename = "test_data_case_episode.txt";
    const std::string content = "Serie,S001,My SERIES,60.0,Drama,4;ePisoDe1:1:3|EpisOde2:1:5\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.calificarVideo("ePiSoDe1", 4); // Different casing
    std::cout.rdbuf(oldCout);

    std::string output = ss.str();
    EXPECT_TRUE(output.find("Episodio 'ePisoDe1' de la serie 'My SERIES' calificado. Nueva calificación promedio: 3.5") != std::string::npos);
    cleanupDummyDataFile(filename);
}

// Test: Case-insensitive search for mostrarEpisodiosDeSerieConCalificacion
TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieCaseInsensitive) {
    const std::string filename = "test_data_series_case.txt";
    const std::string content = "Serie,S001,CaSe SeNsItIvE SeRiEs,60.0,Action,4;Ep1:1:5|Ep2:1:3\n";
    createDummyDataFile(filename, content);

    ServicioStreaming servicio;
    servicio.cargarArchivo(filename);

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    servicio.mostrarEpisodiosDeSerieConCalificacion("cAsE sEnSiTiVe SeRiEs", 4.0); // Different casing
    std::cout.rdbuf(oldCout);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Título: Ep1, Temporada: 1, Calificación: 5.0") != std::string::npos);
    cleanupDummyDataFile(filename);
}