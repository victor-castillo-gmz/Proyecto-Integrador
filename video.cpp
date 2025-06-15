#include "Video.h"

Video::Video(const std::string& titulo) : titulo(titulo) {}

const std::string& Video::getTitulo() const {
    return titulo;
}

void Video::calificar(int calificacion) {
    if (calificacion >= 1 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    }
    // Si la calificación no es válida, no hace nada
}

double Video::getCalificacionPromedio() const {
    if (calificaciones.empty()) return 0.0;

    int suma = 0;
    for (int c : calificaciones) {
        suma += c;
    }
    return static_cast<double>(suma) / calificaciones.size();
}

const std::vector<int>& Video::getCalificaciones() const {
    return calificaciones;
}
