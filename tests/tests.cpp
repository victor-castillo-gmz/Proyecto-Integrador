#include <gtest/gtest.h>
#include "pelicula.h"
#include "serie.h"
#include "episodio.h"
#include "serviciostreaming.h"

// Test básico de Pelicula
TEST(PeliculaTest, CalificacionPromedioCorrecta) {
    Pelicula p("P001", "Inception", 120.0, "Ciencia Ficción");
    p.calificar(4);
    p.calificar(5);
    EXPECT_DOUBLE_EQ(p.getCalificacionPromedio(), 4.5);
}

// Test de episodio individual
TEST(EpisodioTest, CalificacionPromedioCorrecta) {
    Episodio e("Piloto", 1);
    e.calificar(3);
    e.calificar(5);
    EXPECT_DOUBLE_EQ(e.getCalificacionPromedio(), 4.0);
}

// Test de agregar episodios y calificar en serie
TEST(SerieTest, AgregarYCalificarEpisodios) {
    Serie s("S001", "Breaking Bad", 60.0, "Drama");
    Episodio ep1("Piloto", 1);
    ep1.calificar(4);
    s.agregarEpisodio(ep1);

    s.calificarEpisodio("Piloto", 5);
    const auto& episodios = s.getEpisodios();
    ASSERT_EQ(episodios.size(), 1);
    EXPECT_DOUBLE_EQ(episodios[0].getCalificacionPromedio(), 4.5);
}

// Test de ServicioStreaming calificar video (película)
TEST(ServicioStreamingTest, CalificarPelicula) {
    ServicioStreaming servicio;
    auto* p = new Pelicula("P002", "Matrix", 130.0, "Acción");
    // Simular carga manual (no hay método público para agregar, así que este test solo verifica que no truene)
    servicio.calificarVideo("Matrix", 4); // No debe hacer nada (aún no está cargado)
    delete p; // limpiar
}

// Test de validación de calificación inválida
TEST(VideoTest, CalificacionInvalida) {
    Pelicula p("P003", "Test", 90.0, "Drama");
    p.calificar(6); // no válida
    EXPECT_EQ(p.getCalificaciones().size(), 0);
}

// --- PRUEBAS ADICIONALES ---

// Prueba de método mostrarDatos de Pelicula
TEST(PeliculaTest, MostrarDatosNoLanza) {
    Pelicula p("P004", "Interestelar", 150.0, "Ciencia Ficción");
    EXPECT_NO_THROW(p.mostrarDatos());
}

// Prueba de método mostrarDatos de Episodio
TEST(EpisodioTest, MostrarDatosNoLanza) {
    Episodio e("Final", 2);
    e.calificar(5);
    EXPECT_NO_THROW(e.mostrarDatos());
}

// Prueba de método mostrarDatos de Serie con y sin episodios
TEST(SerieTest, MostrarDatosSinYEpisodios) {
    Serie s("S002", "Dark", 50.0, "Misterio");
    EXPECT_NO_THROW(s.mostrarDatos());
    Episodio ep("Inicio", 1);
    s.agregarEpisodio(ep);
    EXPECT_NO_THROW(s.mostrarDatos());
}

// Prueba mostrarEpisodios y mostrarEpisodiosConCalificacion
TEST(SerieTest, MostrarEpisodiosYFiltrado) {
    Serie s("S003", "The Office", 20.0, "Comedia");
    Episodio ep1("Ep1", 1); ep1.calificar(5);
    Episodio ep2("Ep2", 1); ep2.calificar(3);
    s.agregarEpisodio(ep1);
    s.agregarEpisodio(ep2);
    EXPECT_NO_THROW(s.mostrarEpisodios());
    EXPECT_NO_THROW(s.mostrarEpisodiosConCalificacion(4.0));
}

// Prueba calificarEpisodio con episodio inexistente
TEST(SerieTest, CalificarEpisodioInexistente) {
    Serie s("S004", "Lost", 40.0, "Drama");
    EXPECT_NO_THROW(s.calificarEpisodio("NoExiste", 5));
}

// Prueba ServicioStreaming: cargarArchivo con archivo inexistente
TEST(ServicioStreamingTest, CargarArchivoInexistente) {
    ServicioStreaming servicio;
    EXPECT_NO_THROW(servicio.cargarArchivo("archivo_que_no_existe.txt"));
}

// Prueba calificarVideo para episodio dentro de una serie
TEST(ServicioStreamingTest, CalificarEpisodioEnSerie) {
    ServicioStreaming servicio;
    // Simulación manual (no hay método público para agregar videos, pero sí para cargar archivo)
    // Se recomienda exponer un método para pruebas si se requiere mayor coverage aquí.
    // Este test solo verifica que no truene.
    EXPECT_NO_THROW(servicio.calificarVideo("EpisodioX", 4));
}

// Prueba mostrarPeliculasConCalificacion sin películas cargadas
TEST(ServicioStreamingTest, MostrarPeliculasSinPeliculas) {
    ServicioStreaming servicio;
    EXPECT_NO_THROW(servicio.mostrarPeliculasConCalificacion(3.0));
}

// Prueba mostrarVideosPorCalificacionOGenero sin videos cargados
TEST(ServicioStreamingTest, MostrarVideosSinVideos) {
    ServicioStreaming servicio;
    EXPECT_NO_THROW(servicio.mostrarVideosPorCalificacionOGenero(3.0, "Acción"));
}

// Prueba mostrarEpisodiosDeSerieConCalificacion sin series cargadas
TEST(ServicioStreamingTest, MostrarEpisodiosSerieSinSeries) {
    ServicioStreaming servicio;
    EXPECT_NO_THROW(servicio.mostrarEpisodiosDeSerieConCalificacion("SerieX", 4.0));
}
