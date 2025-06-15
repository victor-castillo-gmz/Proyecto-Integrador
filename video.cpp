#include "Video.h"

// Constructor implementation
Video::Video(std::string id, std::string nombre, double duracion, std::string genero)
    : id(id), nombre(nombre), duracion(duracion), genero(genero), calificacionPromedio(0.0) {}

// Method to add a rating
void Video::calificar(int valor) {
    if (valor >= 1 && valor <= 5) {
        calificaciones.push_back(valor);
        calcularPromedio(); // Recalculate average after adding a rating
    } else {
        std::cout << "Calificación inválida. Debe ser de 1 a 5." << std::endl;
    }
}

// Calculate and update the average rating
void Video::calcularPromedio() {
    if (!calificaciones.empty()) {
        double sum = std::accumulate(calificaciones.begin(), calificaciones.end(), 0.0);
        calificacionPromedio = sum / calificaciones.size();
    } else {
        calificacionPromedio = 0.0;
    }
}

// Helper to print common video info
void Video::imprimirInfoBase() const {
    std::cout << "ID: " << id << "\n";
    std::cout << "Nombre: " << nombre << "\n";
    std::cout << "Duración: " << duracion << " min\n";
    std::cout << "Género: " << genero << "\n";
    std::cout << "Calificación promedio: " << std::fixed << std::setprecision(1) << calificacionPromedio << "\n";
}