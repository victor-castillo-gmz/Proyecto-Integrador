#include "Episodio.h"
#include <numeric>

Episodio::Episodio(const std::string& titulo, int numero)
    : titulo(titulo), numero(numero) {}

void Episodio::calificar(int calificacion) {
    if (calificacion >= 0 && calificacion <= 5) {
        calificaciones.push_back(calificacion);
    }
}

double Episodio::getCalificacionPromedio() const {
    if (calificaciones.empty()) return 0.0;
    int suma = std::accumulate(calificaciones.begin(), calificaciones.end(), 0);
    return static_cast<double>(suma) / calificaciones.size();
}

const std::string& Episodio::getTitulo() const {
    return titulo;
}
