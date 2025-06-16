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
    servicio.calificarVideo("Matrix", 4); // No debe hacer nada (aún no está cargado)

    // Simular carga manual
    servicio.calificarVideo("Matrix", 4); // Aún no sirve

    delete p; // limpiar
}

// Test de validación de calificación inválida
TEST(VideoTest, CalificacionInvalida) {
    Pelicula p("P003", "Test", 90.0, "Drama");
    p.calificar(6); // no válida
    EXPECT_EQ(p.getCalificaciones().size(), 0);
}