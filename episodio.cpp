#include "Episodio.h"

// Constructor
Episodio::Episodio(std::string titulo, int temporada)
    : titulo(titulo), temporada(temporada), calificacionPromedio(0.0) {}

// Method to add a rating to the episode
void Episodio::calificar(int valor) {
    if (valor >= 1 && valor <= 5) {
        calificaciones.push_back(valor);
        calcularPromedio();
    } else {
        std::cout << "Calificación de episodio inválida. Debe ser de 1 a 5." << std::endl;
    }
}

// Calculate and update the average rating
void Episodio::calcularPromedio() {
    if (!calificaciones.empty()) {
        double sum = std::accumulate(calificaciones.begin(), calificaciones.end(), 0.0);
        calificacionPromedio = sum / calificaciones.size();
    } else {
        calificacionPromedio = 0.0;
    }
}

// Display episode details
void Episodio::mostrarDatos() const {
    std::cout << "    - Título: " << titulo << ", Temporada: " << temporada
              << ", Calificación: " << std::fixed << std::setprecision(1) << calificacionPromedio << std::endl;
}