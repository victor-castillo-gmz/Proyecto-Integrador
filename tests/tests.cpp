/**
 * @file tests.cpp
 * @brief Pruebas unitarias para las clases del Servicio de Streaming.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "gtest/gtest.h"
#include "pelicula.h"
#include "serie.h"
#include "serviciostreaming.h"
#include "episodio.h"

#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cstdio>

// Helper para redirigir cout y cerr para testear la salida a consola
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

// --- Tests para la clase Video (usando Pelicula como instancia concreta) ---

TEST(VideoTest, CalificacionPromedioCorrecta) {
    Pelicula v("V001", "Test Video", 60, "Action");
    v.Calificar(5);
    v.Calificar(4);
    EXPECT_NEAR(v.GetCalificacionPromedio(), 4.5, 0.001);
}

TEST(VideoTest, CalificacionInvalida) {
    Pelicula p("P001", "Movie A", 90, "Drama");
    p.Calificar(0); // Invalida
    p.Calificar(6); // Invalida
    EXPECT_EQ(p.GetCalificacionPromedio(), 0.0);
}

TEST(VideoTest, CalificacionUnica) {
    Pelicula v("V002", "Single Rating", 30, "Comedy");
    v.Calificar(3);
    EXPECT_EQ(v.GetCalificacionPromedio(), 3.0);
}

TEST(VideoTest, CalificacionPromedioVacio) {
    Pelicula v("V003", "No Ratings", 120, "Thriller");
    EXPECT_EQ(v.GetCalificacionPromedio(), 0.0);
}

// --- Tests para la clase Episodio ---

TEST(EpisodioTest, CalificacionPromedioCorrecta) {
    Episodio e("Pilot", 1);
    e.Calificar(5);
    e.Calificar(4);
    EXPECT_NEAR(e.GetCalificacionPromedio(), 4.5, 0.001);
}

TEST(EpisodioTest, CalificacionEpisodioInvalida) {
    Episodio e("Invalid Rating Ep", 2);
    e.Calificar(0); // Invalida
    e.Calificar(6); // Invalida
    EXPECT_EQ(e.GetCalificacionPromedio(), 0.0);
}

TEST(EpisodioTest, Getters) {
    Episodio e("Episode Title", 3);
    EXPECT_EQ(e.GetTitulo(), "Episode Title");
    EXPECT_EQ(e.GetTemporada(), 3);
}

// --- Tests para la clase Pelicula ---

TEST(PeliculaTest, MostrarDatos) {
    OutputRedirector redirector;
    Pelicula p("P001", "Movie A", 90, "Action");
    p.Calificar(5);
    p.Calificar(4); // Avg 4.5
    p.MostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Tipo: Pelicula"), std::string::npos);
    EXPECT_NE(output.find("ID: P001"), std::string::npos);
    EXPECT_NE(output.find("Nombre: Movie A"), std::string::npos);
    EXPECT_NE(output.find("Calificacion promedio: 4.5"), std::string::npos);
}

// --- Tests para la clase Serie ---

TEST(SerieTest, AgregarYCalificarEpisodios) {
    Serie s("S001", "Test Series", 60, "Drama");
    Episodio ep1("Piloto", 1);
    ep1.Calificar(5);
    ep1.Calificar(4); // Ep1 avg 4.5

    Episodio ep2("Episodio 2", 1);
    ep2.Calificar(3); // Ep2 avg 3.0

    s.AgregarEpisodio(ep1);
    s.AgregarEpisodio(ep2);

    const auto& episodios = s.GetEpisodios();
    ASSERT_EQ(episodios.size(), 2);
    EXPECT_EQ(episodios[0].GetTitulo(), "Piloto");
    EXPECT_NEAR(episodios[0].GetCalificacionPromedio(), 4.5, 0.001);
    EXPECT_EQ(episodios[1].GetTitulo(), "Episodio 2");
    EXPECT_NEAR(episodios[1].GetCalificacionPromedio(), 3.0, 0.001);
}

TEST(SerieTest, MostrarDatos) {
    OutputRedirector redirector;
    Serie s("S001", "Test Series", 60, "Drama");
    s.Calificar(4); // Serie overall rating

    Episodio ep1("Piloto", 1);
    ep1.Calificar(5);
    s.AgregarEpisodio(ep1);

    s.MostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Tipo: Serie"), std::string::npos);
    EXPECT_NE(output.find("Nombre: Test Series"), std::string::npos);
    EXPECT_NE(output.find("Calificacion promedio: 4.0"), std::string::npos);
    EXPECT_NE(output.find("Titulo Episodio: Piloto"), std::string::npos);
    EXPECT_NE(output.find("Calificacion promedio: 5.0"), std::string::npos);
}

TEST(SerieTest, MostrarEpisodiosVacios) {
    OutputRedirector redirector;
    Serie s("S002", "Empty Series", 30, "Comedy");
    s.MostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Esta serie no tiene episodios cargados."), std::string::npos);
}

TEST(SerieTest, MostrarEpisodiosConCalificacion) {
    OutputRedirector redirector;
    Serie s("S003", "Rated Series", 45, "Action");
    Episodio ep1("Fast Pilot", 1);
    ep1.Calificar(5);
    Episodio ep2("Slow Episode", 1);
    ep2.Calificar(2);
    Episodio ep3("Great Finale", 2);
    ep3.Calificar(4);

    s.AgregarEpisodio(ep1);
    s.AgregarEpisodio(ep2);
    s.AgregarEpisodio(ep3);

    redirector.Clear();
    s.MostrarEpisodiosConCalificacion(3.5);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Fast Pilot"), std::string::npos);
    EXPECT_EQ(output.find("Slow Episode"), std::string::npos);
    EXPECT_NE(output.find("Great Finale"), std::string::npos);
}

// --- Tests para la clase ServicioStreaming ---

TEST(ServicioStreamingTest, CargarArchivo) {
    OutputRedirector redirector;
    ServicioStreaming servicio;

    std::ofstream dummy_file("dummy_data.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5-4\n";
    dummy_file << "Serie,S001,Series B,45.0,Drama,3-4;Ep1:1:5-4|Ep2:1:3\n";
    dummy_file.close();

    redirector.Clear();
    servicio.CargarArchivo("dummy_data.txt");
    std::string output_cout = redirector.GetCout();

    EXPECT_NE(output_cout.find("Datos cargados exitosamente"), std::string::npos);

    redirector.Clear();
    servicio.MostrarVideosPorCalificacionOGenero(0.0, ""); // Show all
    output_cout = redirector.GetCout();
    
    EXPECT_NE(output_cout.find("Nombre: Movie A"), std::string::npos);
    EXPECT_NE(output_cout.find("Calificacion promedio: 4.5"), std::string::npos);
    EXPECT_NE(output_cout.find("Nombre: Series B"), std::string::npos);
    EXPECT_NE(output_cout.find("Calificacion promedio: 3.5"), std::string::npos);
    EXPECT_NE(output_cout.find("Titulo Episodio: Ep1"), std::string::npos);
    EXPECT_NE(output_cout.find("Calificacion promedio: 4.5"), std::string::npos);
    EXPECT_NE(output_cout.find("Titulo Episodio: Ep2"), std::string::npos);
    EXPECT_NE(output_cout.find("Calificacion promedio: 3.0"), std::string::npos);

    std::remove("dummy_data.txt");
}

TEST(ServicioStreamingTest, CargarArchivoNoExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    servicio.CargarArchivo("non_existent_file.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_NE(error_output.find("Error: No se pudo abrir el archivo non_existent_file.txt"), std::string::npos);
}

TEST(ServicioStreamingTest, CargarArchivoDuracionInvalida) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_duration.txt");
    dummy_file << "Pelicula,P001,Movie X,invalid_duration,Action,5\n";
    dummy_file.close();

    servicio.CargarArchivo("dummy_data_invalid_duration.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_NE(error_output.find("Advertencia: Duracion invalida en la linea"), std::string::npos);
    std::remove("dummy_data_invalid_duration.txt");
}

TEST(ServicioStreamingTest, CargarArchivoTipoDesconocido) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_unknown_type.txt");
    dummy_file << "Unknown,U001,Mystery Video,60.0,Mystery,5\n";
    dummy_file.close();

    servicio.CargarArchivo("dummy_data_unknown_type.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_NE(error_output.find("Advertencia: Tipo de video desconocido"), std::string::npos);
    std::remove("dummy_data_unknown_type.txt");
}


TEST(ServicioStreamingTest, CargarArchivoPeliculaCalificacionInvalida) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_movie_rating.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,invalid-4\n";
    dummy_file.close();

    servicio.CargarArchivo("dummy_data_invalid_movie_rating.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_NE(error_output.find("Advertencia: Calificacion invalida para Pelicula 'Movie A': 'invalid'"), std::string::npos);
    std::remove("dummy_data_invalid_movie_rating.txt");
}

TEST(ServicioStreamingTest, CargarArchivoSerieCalificacionInvalida) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_series_rating.txt");
    dummy_file << "Serie,S001,Series B,45.0,Drama,invalid-4;Ep1:1:5\n";
    dummy_file.close();

    servicio.CargarArchivo("dummy_data_invalid_series_rating.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_NE(error_output.find("Advertencia: Calificacion invalida para Serie 'Series B': 'invalid'"), std::string::npos);
    std::remove("dummy_data_invalid_series_rating.txt");
}

TEST(ServicioStreamingTest, CargarArchivoEpisodioSegmentoInvalido) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("dummy_data_invalid_episode_segment.txt");
    dummy_file << "Serie,S002,Series C,30.0,Comedy,5;Ep1:invalid_season:5|Ep2:1:4\n";
    dummy_file.close();

    servicio.CargarArchivo("dummy_data_invalid_episode_segment.txt");
    std::string error_output = redirector.GetCerr();
    EXPECT_NE(error_output.find("Advertencia: Temporada invalida para episodio 'Ep1' en 'Series C': 'invalid_season'"), std::string::npos);
    std::remove("dummy_data_invalid_episode_segment.txt");
}

TEST(ServicioStreamingTest, CalificarPeliculaExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_movie.txt");
    dummy_file << "Pelicula,P001,Movie X,90.0,Action,5\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_cal_movie.txt");
    redirector.Clear();

    servicio.CalificarVideo("Movie X", 3);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Video 'Movie X' calificado. Nueva calificacion promedio: 4.0"), std::string::npos);
    std::remove("temp_cal_movie.txt");
}

TEST(ServicioStreamingTest, CalificarPeliculaInexistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_empty.txt"); dummy_file.close();
    servicio.CargarArchivo("temp_empty.txt");
    redirector.Clear();

    servicio.CalificarVideo("Non Existent Movie", 5);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Video o episodio 'Non Existent Movie' no encontrado."), std::string::npos);
    std::remove("temp_empty.txt");
}

TEST(ServicioStreamingTest, CalificarEpisodioExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_episode.txt");
    dummy_file << "Serie,S001,Series Y,30.0,Drama,4;Ep1:1:5\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_cal_episode.txt");
    redirector.Clear();

    servicio.CalificarVideo("Ep1", 3);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Episodio 'Ep1' calificado. Nueva calificacion promedio: 4.0"), std::string::npos);
    std::remove("temp_cal_episode.txt");
}

TEST(ServicioStreamingTest, CalificarEpisodioInexistenteEnSerieExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_nonexistent_episode.txt");
    dummy_file << "Serie,S001,Series Z,30.0,Drama,4;Ep1:1:5\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_cal_nonexistent_episode.txt");
    redirector.Clear();

    servicio.CalificarVideo("NonExistentEp", 5);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Video o episodio 'NonExistentEp' no encontrado."), std::string::npos);
    std::remove("temp_cal_nonexistent_episode.txt");
}

TEST(ServicioStreamingTest, MostrarVideosPorCalificacion) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_filter_rating.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5-4\n"; // Avg 4.5
    dummy_file << "Pelicula,P002,Movie B,80.0,Comedy,3-2\n"; // Avg 2.5
    dummy_file.close();
    servicio.CargarArchivo("temp_filter_rating.txt");
    redirector.Clear();

    servicio.MostrarVideosPorCalificacionOGenero(3.0, "");
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Movie A"), std::string::npos);
    EXPECT_EQ(output.find("Movie B"), std::string::npos);
    std::remove("temp_filter_rating.txt");
}

TEST(ServicioStreamingTest, MostrarVideosPorGenero) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_filter_genre.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5\n";
    dummy_file << "Pelicula,P002,Movie B,80.0,Comedy,3\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_filter_genre.txt");
    redirector.Clear();

    servicio.MostrarVideosPorCalificacionOGenero(0.0, "Action");
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Movie A"), std::string::npos);
    EXPECT_EQ(output.find("Movie B"), std::string::npos);
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
    servicio.CargarArchivo("temp_filter_both.txt");
    redirector.Clear();

    servicio.MostrarVideosPorCalificacionOGenero(4.0, "Action");
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Movie A"), std::string::npos);
    EXPECT_EQ(output.find("Movie B"), std::string::npos);
    EXPECT_EQ(output.find("Movie C"), std::string::npos);
    std::remove("temp_filter_both.txt");
}

TEST(ServicioStreamingTest, MostrarVideosNoEncontrados) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_no_match.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,2\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_no_match.txt");
    redirector.Clear();

    servicio.MostrarVideosPorCalificacionOGenero(4.0, "Comedy");
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("No se encontraron videos con los criterios especificados."), std::string::npos);
    std::remove("temp_no_match.txt");
}

TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieConCalificacion) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_serie_episodes_filter.txt");
    dummy_file << "Serie,S001,My Series,60.0,Drama,5;Ep1:1:5|Ep2:1:2|Ep3:2:4\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_serie_episodes_filter.txt");
    redirector.Clear();

    servicio.MostrarEpisodiosDeSerieConCalificacion("My Series", 3.5);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Titulo Episodio: Ep1"), std::string::npos);
    EXPECT_EQ(output.find("Titulo Episodio: Ep2"), std::string::npos);
    EXPECT_NE(output.find("Titulo Episodio: Ep3"), std::string::npos);
    std::remove("temp_serie_episodes_filter.txt");
}

TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieNoExistente) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_empty_for_series_test.txt"); dummy_file.close();
    servicio.CargarArchivo("temp_empty_for_series_test.txt");
    redirector.Clear();

    servicio.MostrarEpisodiosDeSerieConCalificacion("Non Existent Series", 3.0);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Serie 'Non Existent Series' no encontrada."), std::string::npos);
    std::remove("temp_empty_for_series_test.txt");
}

TEST(ServicioStreamingTest, MostrarPeliculasConCalificacion) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_show_movies.txt");
    dummy_file << "Pelicula,P001,Movie X,90.0,Action,5-4\n"; // Avg 4.5
    dummy_file << "Pelicula,P002,Movie Y,80.0,Comedy,2-1\n"; // Avg 1.5
    dummy_file.close();
    servicio.CargarArchivo("temp_show_movies.txt");
    redirector.Clear();

    servicio.MostrarPeliculasConCalificacion(3.0);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Movie X"), std::string::npos);
    EXPECT_EQ(output.find("Movie Y"), std::string::npos);
    std::remove("temp_show_movies.txt");
}

TEST(ServicioStreamingTest, MostrarPeliculasNoEncontradas) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_no_movies.txt");
    dummy_file << "Pelicula,P001,Movie X,90.0,Action,2\n"; // Avg 2.0
    dummy_file.close();
    servicio.CargarArchivo("temp_no_movies.txt");
    redirector.Clear();

    servicio.MostrarPeliculasConCalificacion(4.0);
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("No se encontraron peliculas con calificacion >="), std::string::npos);
    std::remove("temp_no_movies.txt");
}

TEST(ServicioStreamingTest, DestructorLiberaMemoria) {
    ServicioStreaming* servicio = new ServicioStreaming();
    std::ofstream dummy_file("temp_destructor_test.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5\n";
    dummy_file.close();
    servicio->CargarArchivo("temp_destructor_test.txt");
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
    servicio.CargarArchivo("temp_cal_movie_case.txt");
    redirector.Clear();

    servicio.CalificarVideo("the great movie", 3); // Test lowercase
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Video 'The Great Movie' calificado. Nueva calificacion promedio: 4.0"), std::string::npos);

    redirector.Clear();
    servicio.CalificarVideo("THE GREAT MOVIE", 2); // Test uppercase
    output = redirector.GetCout();
    EXPECT_NE(output.find("Video 'The Great Movie' calificado. Nueva calificacion promedio: 3.3"), std::string::npos);
    std::remove("temp_cal_movie_case.txt");
}

TEST(ServicioStreamingTest, CalificarVideoCaseInsensitiveEpisode) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_cal_episode_case.txt");
    dummy_file << "Serie,S001,Epic Series,30.0,Drama,4;EpisodeOne:1:5\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_cal_episode_case.txt");
    redirector.Clear();

    servicio.CalificarVideo("episodeone", 3); // Test lowercase
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Episodio 'EpisodeOne' calificado. Nueva calificacion promedio: 4.0"), std::string::npos);

    redirector.Clear();
    servicio.CalificarVideo("EPISODEONE", 2); // Test uppercase
    output = redirector.GetCout();
    EXPECT_NE(output.find("Episodio 'EpisodeOne' calificado. Nueva calificacion promedio: 3.3"), std::string::npos);
    std::remove("temp_cal_episode_case.txt");
}

TEST(ServicioStreamingTest, MostrarEpisodiosDeSerieCaseInsensitive) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_show_series_case.txt");
    dummy_file << "Serie,S001,My Awesome Series,60.0,Action,5;Pilot:1:5\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_show_series_case.txt");
    redirector.Clear();

    servicio.MostrarEpisodiosDeSerieConCalificacion("my awesome series", 0.0); // Test lowercase
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Episodios de la serie 'My Awesome Series'"), std::string::npos);
    EXPECT_NE(output.find("Titulo Episodio: Pilot"), std::string::npos);

    redirector.Clear();
    servicio.MostrarEpisodiosDeSerieConCalificacion("MY AWESOME SERIES", 0.0); // Test uppercase
    output = redirector.GetCout();
    EXPECT_NE(output.find("Episodios de la serie 'My Awesome Series'"), std::string::npos);
    EXPECT_NE(output.find("Titulo Episodio: Pilot"), std::string::npos);
    std::remove("temp_show_series_case.txt");
}


// ============================================================================================
// ============================= NUEVAS PRUEBAS UNITARIAS =====================================
// ============================================================================================

// NUEVO: Prueba los getters de Video a través de Pelicula para asegurar la cobertura de video.cpp y pelicula.cpp
TEST(PeliculaTest, Getters) {
    Pelicula p("P123", "Getter Movie", 111, "TestGenre");
    EXPECT_EQ(p.GetId(), "P123");
    EXPECT_EQ(p.GetNombre(), "Getter Movie");
    EXPECT_EQ(p.GetDuracion(), 111);
    EXPECT_EQ(p.GetGenero(), "TestGenre");
}

// NUEVO: Prueba explícitamente la salida de MostrarDatos de Episodio para cubrir episodio.cpp
TEST(EpisodioTest, MostrarDatos) {
    OutputRedirector redirector;
    Episodio e("Coverage Episode", 7);
    e.Calificar(5);
    e.MostrarDatos();
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("Titulo Episodio: Coverage Episode"), std::string::npos);
    EXPECT_NE(output.find("Temporada: 7"), std::string::npos);
    EXPECT_NE(output.find("Calificacion promedio: 5.0"), std::string::npos);
}

// NUEVO: Prueba el getter mutable de episodios para cubrir esa función en serie.cpp
TEST(SerieTest, GetEpisodiosMutables) {
    Serie s("S456", "Mutable Series", 50, "Sci-Fi");
    s.AgregarEpisodio(Episodio("Ep Original", 1));
    
    // Obtiene el vector mutable y modifica un episodio
    s.GetEpisodiosMutables()[0].Calificar(1);
    
    // Verifica que el cambio se refleje al obtener el vector de forma constante
    const auto& episodios = s.GetEpisodios();
    EXPECT_EQ(episodios[0].GetCalificacionPromedio(), 1.0);
}

// NUEVO: Prueba el mensaje de "no encontrados" en MostrarEpisodiosConCalificacion
TEST(SerieTest, MostrarEpisodiosConCalificacionSinResultados) {
    OutputRedirector redirector;
    Serie s("S789", "No Match Series", 50, "Fantasy");
    s.AgregarEpisodio(Episodio("Low Rated Ep", 1));
    s.GetEpisodiosMutables()[0].Calificar(2); // Calificación de 2.0

    s.MostrarEpisodiosConCalificacion(4.0); // Filtro que no encontrará nada
    std::string output = redirector.GetCout();
    EXPECT_NE(output.find("No se encontraron episodios con esa calificacion."), std::string::npos);
}

// NUEVO: Prueba qué pasa si se le pide mostrar episodios a una Pelicula
TEST(ServicioStreamingTest, MostrarEpisodiosDeVideoQueNoEsSerie) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_movie_not_serie.txt");
    dummy_file << "Pelicula,P999,Just a Movie,90.0,Comedy,5\n";
    dummy_file.close();
    servicio.CargarArchivo("temp_movie_not_serie.txt");
    redirector.Clear();
    
    servicio.MostrarEpisodiosDeSerieConCalificacion("Just a Movie", 3.0);
    std::string output = redirector.GetCout();

    // Debe indicar que la serie no fue encontrada, porque "Just a Movie" es una película
    EXPECT_NE(output.find("Serie 'Just a Movie' no encontrada."), std::string::npos);
    std::remove("temp_movie_not_serie.txt");
}

// NUEVO: Prueba la carga de un archivo que contiene una línea vacía
TEST(ServicioStreamingTest, CargarArchivoConLineaVacia) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_empty_line.txt");
    dummy_file << "Pelicula,P001,Movie A,90.0,Action,5\n";
    dummy_file << "\n"; // Línea vacía
    dummy_file << "Pelicula,P002,Movie B,80.0,Comedy,4\n";
    dummy_file.close();

    servicio.CargarArchivo("temp_empty_line.txt");
    redirector.Clear();
    
    servicio.MostrarVideosPorCalificacionOGenero(0.0, ""); // Mostrar todo
    std::string output = redirector.GetCout();

    // Ambas películas deben haber sido cargadas, la línea vacía debe ser ignorada.
    EXPECT_NE(output.find("Movie A"), std::string::npos);
    EXPECT_NE(output.find("Movie B"), std::string::npos);

    std::remove("temp_empty_line.txt");
}

// NUEVO: Prueba la carga de una serie que no tiene sección de episodios en el archivo
TEST(ServicioStreamingTest, CargarSerieSinSeccionDeEpisodios) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_serie_no_episodes.txt");
    // Formato válido, pero sin el punto y coma que indica episodios
    dummy_file << "Serie,S999,Series without Episodes,50.0,Drama,5-4\n";
    dummy_file.close();

    servicio.CargarArchivo("temp_serie_no_episodes.txt");
    redirector.Clear();

    servicio.MostrarVideosPorCalificacionOGenero(0.0, "Drama");
    std::string output = redirector.GetCout();
    
    // La serie debe cargarse con su calificación, pero indicar que no tiene episodios.
    EXPECT_NE(output.find("Nombre: Series without Episodes"), std::string::npos);
    EXPECT_NE(output.find("Calificacion promedio: 4.5"), std::string::npos);
    EXPECT_NE(output.find("Esta serie no tiene episodios cargados."), std::string::npos);

    std::remove("temp_serie_no_episodes.txt");
}

// NUEVO: Prueba la carga de episodios cuando uno tiene formato incorrecto
TEST(ServicioStreamingTest, CargarEpisodioConFormatoIncorrecto) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_malformed_episode.txt");
    // Ep1 no tiene calificaciones, Ep2 no tiene temporada ni calificaciones
    dummy_file << "Serie,S998,Malformed Series,45.0,Comedy,5;Ep1:1:|Ep2|Ep3:2:4\n";
    dummy_file.close();

    servicio.CargarArchivo("temp_malformed_episode.txt");
    redirector.Clear();
    
    servicio.MostrarEpisodiosDeSerieConCalificacion("Malformed Series", 0.0);
    std::string output = redirector.GetCout();

    // Solo Ep3 (bien formado) y Ep1 (parcialmente bien formado) deben cargarse. Ep2 se ignora.
    EXPECT_NE(output.find("Ep1"), std::string::npos);
    EXPECT_EQ(output.find("Ep2"), std::string::npos);
    EXPECT_NE(output.find("Ep3"), std::string::npos);

    std::remove("temp_malformed_episode.txt");
}

// NUEVO: Prueba la carga de un episodio que tiene una calificación vacía
TEST(ServicioStreamingTest, CargarEpisodioConCalificacionVacia) {
    OutputRedirector redirector;
    ServicioStreaming servicio;
    std::ofstream dummy_file("temp_empty_ep_rating.txt");
    // La calificación de Ep1 es `5--4`, el `--` debe ser ignorado.
    dummy_file << "Serie,S996,Series With Empty Ep Rating,60.0,Action,5;Ep1:1:5--4\n";
    dummy_file.close();

    servicio.CargarArchivo("temp_empty_ep_rating.txt");
    redirector.Clear();

    servicio.MostrarEpisodiosDeSerieConCalificacion("Series With Empty Ep Rating", 0.0);
    std::string output = redirector.GetCout();
    
    // El promedio debe ser (5+4)/2 = 4.5
    EXPECT_NE(output.find("Calificacion promedio: 4.5"), std::string::npos);
    std::remove("temp_empty_ep_rating.txt");
}