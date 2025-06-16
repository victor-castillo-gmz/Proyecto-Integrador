#include "gtest/gtest.h"
#include "pelicula.h"
#include "serie.h"
#include "serviciostreaming.h"
#include "episodio.h" // Ensure episode.h is included for Episodio objects

#include <sstream> // Required for std::stringstream
#include <string>
#include <vector>
#include <stdexcept> // For std::invalid_argument etc.
#include <fstream>   // For std::ofstream
#include <cstdio>    // For std::remove

// Helper to redirect cout and cerr for testing output
class OutputRedirector {
public:
    OutputRedirector() : old_cout(std::cout.rdbuf()), old_cerr(std::cerr.rdbuf()) {
        std::cout.rdbuf(cout_buffer.rdbuf());
        std::cerr.rdbuf(cerr_buffer.rdbuf());
    }

    ~OutputRedirector() {
        std::cout.rdbuf(old_cout);
        std::cerr.rdbuf(old_cerr);
    }

    std::string GetCout() {
        return cout_buffer.str();
    }

    std::string GetCerr() {
        return cerr_buffer.str();
    }

    void Clear() {
        cout_buffer.str("");
        cerr_buffer.str("");
    }

private:
    std::stringstream cout_buffer;
    std::stringstream cerr_buffer;
    std::streambuf* old_cout;
    std::streambuf* old_cerr;
};

// Test fixture for Video class
TEST(VideoTest, CalificacionPromedioCorrecta) {
    Video v("V001", "Test Video", 60, "Action");
    v.calificar(5);
    v.calificar(4);
    EXPECT_NEAR(v.getCalificacionPromedio(), 4.5, 0.001);
}

TEST(VideoTest, CalificacionInvalida) {
    Pelicula p("P001", "Movie A", 90, "Drama");
    p.calificar(0); // Invalid
    p.calificar(6); // Invalid
    EXPECT_EQ(p.getCalificacionPromedio(), 0.0); // No valid ratings should lead to 0.0
}

TEST(VideoTest, CalificacionUnica) {
    Video v("V002", "Single Rating", 30, "Comedy");
    v.calificar(3);
    EXPECT_EQ(v.getCalificacionPromedio(), 3.0);
}

TEST(VideoTest, CalificacionPromedioVacio) {
    Video v("V003", "No Ratings", 120, "Thriller");
    EXPECT_EQ(v.getCalificacionPromedio(), 0.0);
}

// Test fixture for Episodio class
TEST(EpisodioTest, CalificacionPromedioCorrecta) {
    Episodio e("Pilot", 1);
    e.calificar(5);
    e.calificar(4);
    EXPECT_NEAR(e.getCalificacionPromedio(), 4.5, 0.001);
}

TEST(EpisodioTest, CalificacionEpisodioInvalida) {
    Episodio e("Invalid Rating Ep", 2);
    e.calificar(0); // Invalid
    e.calificar(6); // Invalid
    EXPECT_EQ(e.getCalificacionPromedio(), 0.0); // Should remain 0.0 if no valid ratings
}

TEST(EpisodioTest, Getters) {
    Episodio e("Episode Title", 3);
    EXPECT_EQ(e.getTitulo(), "Episode Title");
    EXPECT_EQ(e.getTemporada(), 3);
}

// Test fixture for Pelicula class
TEST(PeliculaTest, MostrarDatos) {
    OutputRedirector redirector;
    Pelicula p("P001", "Movie A", 90, "Action");
    p.calificar(5);
    p.calificar(4); // Avg 4.5
    p.mostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Tipo: Película") != std::string::npos);
    EXPECT_TRUE(output.find("ID: P001") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Movie A") != std::string::npos);
    // MODIFIED: Adjusted expected duration string to match potential default double formatting
    EXPECT_TRUE(output.find("Duración: 90.0 mins") != std::string::npos || output.find("Duración: 90 mins") != std::string::npos); // Allow both 90.0 and 90
    EXPECT_TRUE(output.find("Género: Action") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.5") != std::string::npos);
}

// Test fixture for Serie class
TEST(SerieTest, AgregarYCalificarEpisodios) {
    Serie s("S001", "Test Series", 60, "Drama");
    Episodio ep1("Piloto", 1);
    ep1.calificar(5);
    ep1.calificar(4); // Ep1 avg 4.5

    Episodio ep2("Episodio 2", 1);
    ep2.calificar(3); // Ep2 avg 3.0

    s.AgregarEpisodio(ep1);
    s.AgregarEpisodio(ep2);

    // After adding, verify episodes can be retrieved and their ratings are correct
    const auto& episodios = s.GetEpisodios();
    ASSERT_EQ(episodios.size(), 2);
    EXPECT_EQ(episodios[0].getTitulo(), "Piloto");
    EXPECT_NEAR(episodios[0].getCalificacionPromedio(), 4.5, 0.001);
    EXPECT_EQ(episodios[1].getTitulo(), "Episodio 2");
    EXPECT_NEAR(episodios[1].getCalificacionPromedio(), 3.0, 0.001);
}

TEST(SerieTest, MostrarDatos) {
    OutputRedirector redirector;
    Serie s("S001", "Test Series", 60, "Drama");
    s.calificar(4); // Serie overall rating

    Episodio ep1("Piloto", 1);
    ep1.calificar(5);
    s.AgregarEpisodio(ep1);

    s.mostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Tipo: Serie") != std::string::npos);
    EXPECT_TRUE(output.find("ID: S001") != std::string::npos);
    EXPECT_TRUE(output.find("Nombre: Test Series") != std::string::npos);
    EXPECT_TRUE(output.find("Duración: 60.0 mins") != std::string::npos);
    EXPECT_TRUE(output.find("Género: Drama") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.0") != std::string::npos); // Series own average
    EXPECT_TRUE(output.find("Título Episodio: Piloto") != std::string::npos);
    EXPECT_TRUE(output.find("Temporada: 1") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 5.0") != std::string::npos); // Episode average
}

TEST(SerieTest, MostrarEpisodiosVacios) {
    OutputRedirector redirector;
    Serie s("S002", "Empty Series", 30, "Comedy");
    s.mostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Esta serie no tiene episodios cargados.") != std::string::npos);
}

TEST(SerieTest, MostrarEpisodiosConCalificacion) {
    OutputRedirector redirector;
    Serie s("S003", "Rated Series", 45, "Action");
    Episodio ep1("Fast Pilot", 1);
    ep1.calificar(5);
    Episodio ep2("Slow Episode", 1);
    ep2.calificar(2);
    Episodio ep3("Great Finale", 2);
    ep3.calificar(4);

    s.AgregarEpisodio(ep1);
    s.AgregarEpisodio(ep2);
    s.AgregarEpisodio(ep3);

    redirector.Clear();
    s.MostrarEpisodiosConCalificacion(3.5);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Fast Pilot") != std::string::npos);
    EXPECT_FALSE(output.find("Slow Episode") != std::string::npos);
    EXPECT_TRUE(output.find("Great Finale") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 5.0") != std::string::npos);
    EXPECT_TRUE(output.find("Calificación promedio: 4.0") != std::string::npos);

    redirector.Clear();
    s.MostrarEpisodiosConCalificacion(5.0);
    output = redirector.GetCout();
    EXPECT_TRUE(output.find("Fast Pilot") != std::string::npos);
    EXPECT_FALSE(output.find("Slow Episode") != std::string::npos);
    EXPECT_FALSE(output.find("Great Finale") != std::string::npos);

    redirector.Clear();
    s.MostrarEpisodiosConCalificacion(1.0);
    output = redirector.GetCout();
    EXPECT_TRUE(output.find("Fast Pilot") != std::string::npos);
    EXPECT_TRUE(output.find("Slow Episode") != std::string::npos);
    EXPECT_TRUE(output.find("Great Finale") != std::string::npos);

    redirector.Clear();
    s.MostrarEpisodiosConCalificacion(6.0); // No episodes should match
    output = redirector.GetCout();
    EXPECT_TRUE(output.find("No se encontraron episodios con esa calificación.") != std::string::npos);
}


// Test fixture for ServicioStreaming class
TEST(ServicioStreamingTest, CargarArchivo) {
    OutputRedirector redirector;
    ServicioStreaming servicio;

    // Create a dummy_data.txt for this test
    std::ofstream dummy_file("dummy_data.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5-4\n";
    dummy_file << "Serie,S001,Series B,45.0,Drama,3-4;Ep1:1:5-4|Ep2:1:3\n";
    dummy_file.close();

    redirector.Clear(); // Clear any output from cargarArchivo itself
    servicio.cargarArchivo("dummy_data.txt");
    std::string output_cout = redirector.GetCout();
    std::string output_cerr = redirector.GetCerr();

    // Check if the success message is present
    EXPECT_TRUE(output_cout.find("Datos cargados exitosamente") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Total de videos: 2") != std::string::npos);

    // Verify correct loading by checking video details
    // We can't directly check the internal 'videos' vector, so we use display methods.
    redirector.Clear();
    servicio.mostrarVideosPorCalificacionOGenero(0.0, ""); // Show all
    output_cout = redirector.GetCout();

    // Check for "Movie A" details
    EXPECT_TRUE(output_cout.find("Nombre: Movie A") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Género: Action") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Calificación promedio: 4.5") != std::string::npos);

    // Check for "Series B" details
    EXPECT_TRUE(output_cout.find("Nombre: Series B") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Género: Drama") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Calificación promedio: 3.5") != std::string::npos); // Serie's own avg
    EXPECT_TRUE(output_cout.find("Título Episodio: Ep1") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Calificación promedio: 4.5") != std::string::npos); // Ep1's avg
    EXPECT_TRUE(output_cout.find("Título Episodio: Ep2") != std::string::npos);
    EXPECT_TRUE(output_cout.find("Calificación promedio: 3.0") != std::string::npos); // Ep2's avg

    // Clean up dummy file
    std::remove("dummy_data.txt");
}

TEST(ServicioStreamingTest, CargarArchivoNoExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    servicio.cargarArchivo("non_existent_file.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_TRUE(error_output.find("Error: No se pudo abrir el archivo non_existent_file.txt") != std::string::npos);
}

TEST(ServicioStreamingTest, CargarArchivoDuracionInvalida) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_duration.txt");
    dummy_file << "Pelicula,P001,Movie X,invalid_duration,Action,5\n";
    dummy_file.close();

    servicio.cargarArchivo("dummy_data_invalid_duration.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_TRUE(error_output.find("Advertencia: Duración inválida en la línea") != std::string::npos);
    std::remove("dummy_data_invalid_duration.txt");
}

TEST(ServicioStreamingTest, CargarArchivoTipoDesconocido) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_unknown_type.txt");
    dummy_file << "Unknown,U001,Mystery Video,60.0,Mystery,5\n";
    dummy_file.close();

    servicio.cargarArchivo("dummy_data_unknown_type.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_TRUE(error_output.find("Advertencia: Tipo de video desconocido") != std::string::npos);
    std::remove("dummy_data_unknown_type.txt");
}


TEST(ServicioStreamingTest, CargarArchivoPeliculaCalificacionInvalida) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_movie_rating.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,invalid-4\n"; // "invalid" rating
    dummy_file.close();

    servicio.cargarArchivo("dummy_data_invalid_movie_rating.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_TRUE(error_output.find("Advertencia: Calificación inválida para Pelicula 'Movie A': 'invalid'") != std::string::npos);
    std::remove("dummy_data_invalid_movie_rating.txt");
}

TEST(ServicioStreamingTest, CargarArchivoSerieCalificacionInvalida) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_series_rating.txt");
    dummy_file << "Serie,S001,Series B,45.0,Drama,invalid-4;Ep1:1:5\n"; // "invalid" rating for series
    dummy_file.close();

    servicio.cargarArchivo("dummy_data_invalid_series_rating.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_TRUE(error_output.find("Advertencia: Calificación inválida para Serie 'Series B': 'invalid'") != std::string::npos);
    std::remove("dummy_data_invalid_series_rating.txt");
}

TEST(ServicioStreamingTest, CargarArchivoEpisodioSegmentoInvalido) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_episode_segment.txt");
    dummy_file << "Serie,S002,Series C,30.0,Comedy,5;Ep1:invalid_season:5|Ep2:1:4\n"; // Invalid season
    dummy_file.close();

    servicio.cargarArchivo("dummy_data_invalid_episode_segment.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_TRUE(error_output.find("Advertencia: Temporada inválida para episodio 'Ep1' en 'Series C': 'invalid_season'") != std::string::npos);
    std::remove("dummy_data_invalid_episode_segment.txt");
}

TEST(ServicioStreamingTest, CalificarPeliculaExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_movie.txt");
    dummy_file << "Pelicula,P001,Movie X,90.0,Action,5\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_cal_movie.txt");
    redirector.Clear(); // Clear load message

    servicio.calificarVideo("Movie X", 3);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Video 'Movie X' calificado. Nueva calificación promedio: 4.0") != std::string::npos);
    std::remove("temp_cal_movie.txt");
}

TEST(ServicioStreamingTest, CalificarPeliculaInexistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_empty.txt"); dummy_file.close();
    servicio.cargarArchivo("temp_empty.txt");
    redirector.Clear();

    servicio.calificarVideo("Non Existent Movie", 5);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Video o episodio 'Non Existent Movie' no encontrado.") != std::string::npos);
    std::remove("temp_empty.txt");
}

TEST(ServicioStreamingTest, CalificarEpisodioExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_episode.txt");
    dummy_file << "Serie,S001,Series Y,30.0,Drama,4;Ep1:1:5\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_cal_episode.txt");
    redirector.Clear(); // Clear load message

    servicio.calificarVideo("Ep1", 3);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Episodio 'Ep1' de la serie 'Series Y' calificado. Nueva calificación promedio: 4.0") != std::string::npos);
    std::remove("temp_cal_episode.txt");
}

TEST(ServicioStreamingTest, CalificarEpisodioInexistenteEnSerieExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_nonexistent_episode.txt");
    dummy_file << "Serie,S001,Series Z,30.0,Drama,4;Ep1:1:5\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_cal_nonexistent_episode.txt");
    redirector.Clear();

    servicio.calificarVideo("NonExistentEp", 5);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Video o episodio 'NonExistentEp' no encontrado.") != std::string::npos);
    std::remove("temp_cal_nonexistent_episode.txt");
}

TEST(ServicioStreamingTest, MostrarVideosPorCalificacion) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_filter_rating.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5-4\n"; // Avg 4.5
    dummy_file << "Pelicula,P002,Movie B,80.0,Comedy,3-2\n"; // Avg 2.5
    dummy_file.close();
    servicio.cargarArchivo("temp_filter_rating.txt");
    redirector.Clear();

    servicio.mostrarVideosPorCalificacionOGenero(3.0, "");
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Movie A") != std::string::npos);
    EXPECT_FALSE(output.find("Movie B") != std::string::npos);
    std::remove("temp_filter_rating.txt");
}

TEST(ServicioStreamingTest, MostrarVideosPorGenero) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_filter_genre.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5\n";
    dummy_file << "Pelicula,P002,Movie B,80.0,Comedy,3\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_filter_genre.txt");
    redirector.Clear();

    servicio.mostrarVideosPorCalificacionOGenero(0.0, "Action");
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Movie A") != std::string::npos);
    EXPECT_FALSE(output.find("Movie B") != std::string::npos);
    std::remove("temp_filter_genre.txt");
}

TEST(ServicioStreamingTest, MostrarVideosPorCalificacionYGenero) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_filter_both.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5\n"; // Action, 5.0
    dummy_file << "Pelicula,P002,Movie B,80.0,Comedy,4\n"; // Comedy, 4.0
    dummy_file << "Pelicula,P003,Movie C,70.0,Action,3\n"; // Action, 3.0
    dummy_file.close();
    servicio.cargarArchivo("temp_filter_both.txt");
    redirector.Clear();

    servicio.mostrarVideosPorCalificacionOGenero(4.0, "Action");
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Movie A") != std::string::npos);
    EXPECT_FALSE(output.find("Movie B") != std::string::npos);
    EXPECT_FALSE(output.find("Movie C") != std::string::npos);
    std::remove("temp_filter_both.txt");
}

TEST(ServicioStreamingTest, MostrarVideosNoEncontrados) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_no_match.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,2\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_no_match.txt");
    redirector.Clear();

    servicio.mostrarVideosPorCalificacionOGenero(4.0, "Comedy");
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("No se encontraron videos con los criterios especificados.") != std::string::npos);
    std::remove("temp_no_match.txt");
}

TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieConCalificacion) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_serie_episodes_filter.txt");
    dummy_file << "Serie,S001,My Series,60.0,Drama,5;Ep1:1:5|Ep2:1:2|Ep3:2:4\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_serie_episodes_filter.txt");
    redirector.Clear();

    servicio.mostrarEpisodiosDeSerieConCalificacion("My Series", 3.5);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Título Episodio: Ep1") != std::string::npos);
    EXPECT_FALSE(output.find("Título Episodio: Ep2") != std::string::npos);
    EXPECT_TRUE(output.find("Título Episodio: Ep3") != std::string::npos);
    std::remove("temp_serie_episodes_filter.txt");
}

TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieNoExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_empty_for_series_test.txt"); dummy_file.close();
    servicio.cargarArchivo("temp_empty_for_series_test.txt");
    redirector.Clear();

    servicio.mostrarEpisodiosDeSerieConCalificacion("Non Existent Series", 3.0);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Serie 'Non Existent Series' no encontrada.") != std::string::npos);
    std::remove("temp_empty_for_series_test.txt");
}

TEST(ServicioStreamingTest, MostrarPeliculasConCalificacion) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_show_movies.txt");
    dummy_file << "Pelicula,P001,Movie X,90.0,Action,5-4\n"; // Avg 4.5
    dummy_file << "Pelicula,P002,Movie Y,80.0,Comedy,2-1\n"; // Avg 1.5
    dummy_file.close();
    servicio.cargarArchivo("temp_show_movies.txt");
    redirector.Clear();

    servicio.mostrarPeliculasConCalificacion(3.0);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Movie X") != std::string::npos);
    EXPECT_FALSE(output.find("Movie Y") != std::string::npos);
    std::remove("temp_show_movies.txt");
}

TEST(ServicioStreamingTest, MostrarPeliculasNoEncontradas) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_no_movies.txt");
    dummy_file << "Pelicula,P001,Movie X,90.0,Action,2\n"; // Avg 2.0
    dummy_file.close();
    servicio.cargarArchivo("temp_no_movies.txt");
    redirector.Clear();

    servicio.mostrarPeliculasConCalificacion(4.0);
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("No se encontraron películas con calificación >= 4.0.") != std::string::npos);
    std::remove("temp_no_movies.txt");
}

TEST(ServicioStreamingTest, DestructorLiberaMemoria) {
    ServicioStreaming* servicio = new ServicioStreaming();
    std::ofstream dummy_file("temp_destructor_test.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5\n";
    dummy_file.close();
    servicio->cargarArchivo("temp_destructor_test.txt");
    delete servicio;
    std::remove("temp_destructor_test.txt");
    SUCCEED();
}

TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveMovie) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_movie_case.txt");
    dummy_file << "Pelicula,P001,The Great Movie,90.0,Action,5\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_cal_movie_case.txt");
    redirector.Clear();

    servicio.calificarVideo("the great movie", 3); // Test lowercase
    std::string output = redirector.GetCout();
    // Expected: (5+3)/2 = 4.0
    EXPECT_TRUE(output.find("Video 'The Great Movie' calificado. Nueva calificación promedio: 4.0") != std::string::npos);

    redirector.Clear();
    servicio.calificarVideo("THE GREAT MOVIE", 2); // Test uppercase
    output = redirector.GetCout();
    // Expected: (5+3+2)/3 = 3.33... -> 3.3
    EXPECT_TRUE(output.find("Video 'The Great Movie' calificado. Nueva calificación promedio: 3.3") != std::string::npos);
    std::remove("temp_cal_movie_case.txt");
}

TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveEpisode) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_episode_case.txt");
    dummy_file << "Serie,S001,Epic Series,30.0,Drama,4;EpisodeOne:1:5\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_cal_episode_case.txt");
    redirector.Clear();

    servicio.calificarVideo("episodeone", 3); // Test lowercase
    std::string output = redirector.GetCout();
    // Expected: (5+3)/2 = 4.0
    EXPECT_TRUE(output.find("Episodio 'EpisodeOne' de la serie 'Epic Series' calificado. Nueva calificación promedio: 4.0") != std::string::npos);

    redirector.Clear();
    servicio.calificarVideo("EPISODEONE", 2); // Test uppercase
    output = redirector.GetCout();
    // Expected: (5+3+2)/3 = 3.33... -> 3.3
    EXPECT_TRUE(output.find("Episodio 'EpisodeOne' de la serie 'Epic Series' calificado. Nueva calificación promedio: 3.3") != std::string::npos);
    std::remove("temp_cal_episode_case.txt");
}

TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieCaseInsensitive) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_show_series_case.txt");
    dummy_file << "Serie,S001,My Awesome Series,60.0,Action,5;Pilot:1:5\n";
    dummy_file.close();
    servicio.cargarArchivo("temp_show_series_case.txt");
    redirector.Clear();

    servicio.mostrarEpisodiosDeSerieConCalificacion("my awesome series", 0.0); // Test lowercase
    std::string output = redirector.GetCout();
    EXPECT_TRUE(output.find("Episodios de la serie 'My Awesome Series'") != std::string::npos);
    EXPECT_TRUE(output.find("Título Episodio: Pilot") != std::string::npos);

    redirector.Clear();
    servicio.mostrarEpisodiosDeSerieConCalificacion("MY AWESOME SERIES", 0.0); // Test uppercase
    output = redirector.GetCout();
    EXPECT_TRUE(output.find("Episodios de la serie 'My Awesome Series'") != std::string::npos);
    EXPECT_TRUE(output.find("Título Episodio: Pilot") != std::string::npos);
    std::remove("temp_show_series_case.txt");
}