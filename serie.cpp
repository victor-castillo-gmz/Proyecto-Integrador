#include "serie.h"
#include <iostream>
#include <iomanip> // Necesario para std::fixed y std::setprecision

// Constructor
Serie::Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : Video(id, nombre, duracion, genero) {
    // Los episodios se agregan después
}

// Destructor
Serie::~Serie() {
    // No es necesario liberar memoria de episodios aquí si Episodio no usa punteros
    // y los episodios se manejan por valor en el vector.
}

// Método para agregar un episodio
void Serie::agregarEpisodio(const Episodio& episodio) {
    episodios.push_back(episodio);
}

// Método para calificar un episodio específico por su título
void Serie::calificarEpisodio(const std::string& tituloEpisodio, int calificacion) {
    bool found = false;
    for (Episodio& ep : episodios) {
        if (ep.getTitulo() == tituloEpisodio) {
            ep.calificar(calificacion);
            std::cout << "Episodio '" << tituloEpisodio << "' calificado. Nueva calificación promedio: "
                      << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Episodio '" << tituloEpisodio << "' no encontrado en esta serie." << std::endl;
    }
}

// Método para mostrar datos de la serie, incluyendo episodios
void Serie::mostrarDatos() const {
    std::cout << "--- Serie ---" << std::endl;
    imprimirInfoBase(); // Reutiliza el método de la clase base Video
    mostrarEpisodios(); // Llama a mostrarEpisodios para listar los episodios
}

// Método para mostrar todos los episodios de la serie
void Serie::mostrarEpisodios() const {
    if (episodios.empty()) {
        std::cout << "No hay episodios en esta serie." << std::endl;
        return;
    }
    std::cout << "Episodios:" << std::endl;
    for (const Episodio& ep : episodios) {
        std::cout << "  - Título: " << ep.getTitulo()
                  << ", Temporada: " << ep.getTemporada()
                  << ", Calificación: " << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
    }
}

// Método para mostrar episodios con una calificación mínima
void Serie::mostrarEpisodiosConCalificacion(double calificacionMinima) const {
    bool found_matching_episode = false; // Flag to track if any episode meets criteria
    std::cout << "Episodios de '" << getNombre() << "' con calificación >= "
              << std::fixed << std::setprecision(1) << calificacionMinima << ":" << std::endl;

    for (const Episodio& ep : episodios) {
        if (ep.getCalificacionPromedio() >= calificacionMinima) {
            std::cout << "  - Título: " << ep.getTitulo()
                      << ", Temporada: " << ep.getTemporada()
                      << ", Calificación: " << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
            found_matching_episode = true;
        }
    }

    // MODIFICACION APLICADA: Asegurarse que esta cadena coincida EXACTAMENTE con el test.
    if (!found_matching_episode) {
        std::cout << "No se encontraron episodios con esa calificación." << std::endl;
    }
}