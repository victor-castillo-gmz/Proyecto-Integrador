#include "ServicioStreaming.h"

void ServicioStreaming::agregarPelicula(const Pelicula& pelicula) {
    peliculas.push_back(pelicula);
}

void ServicioStreaming::agregarSerie(const Serie& serie) {
    series.push_back(serie);
}

void ServicioStreaming::calificarVideo(const std::string& titulo, int calificacion) {
    // Buscar en películas
    for (auto& p : peliculas) {
        if (p.getTitulo() == titulo) {
            p.calificar(calificacion);
            return;
        }
    }
    // Buscar en episodios de series
    for (auto& s : series) {
        // No califica la serie completa, sino episodios
        s.calificarEpisodio(titulo, calificacion);
    }
}

double ServicioStreaming::obtenerCalificacion(const std::string& titulo) const {
    // Buscar en peliculas
    for (const auto& p : peliculas) {
        if (p.getTitulo() == titulo) {
            return p.getCalificacionPromedio();
        }
    }
    // Buscar en episodios de series
    for (const auto& s : series) {
        for (const auto& ep : s.getEpisodios()) {
            if (ep.getTitulo() == titulo) {
                return ep.getCalificacionPromedio();
            }
        }
    }
    return 0.0; // No encontrado
}
