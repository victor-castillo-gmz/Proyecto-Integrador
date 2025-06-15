#include "Pelicula.h"
#include <numeric>

Pelicula::Pelicula(const std::string& id, const std::string& titulo, double duracion, const std::string& genero)
    : id(id), titulo(titulo), duracion(duracion), genero(genero) {}

void Pelicula::calificar(int calificacion) {
    if (calificacion >= 0 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    }
}

double Pelicula::getCalificacionPromedio() const {
    if (calificaciones.empty()) return 0.0;
    int suma = std::accumulate(calificaciones.begin(), calificaciones.end(), 0);
    return static_cast<double>(suma) / calificaciones.size();
}

const std::vector<int>& Pelicula::getCalificaciones() const {
    return calificaciones;
}

const std::string& Pelicula::getTitulo() const {
    return titulo;
}
