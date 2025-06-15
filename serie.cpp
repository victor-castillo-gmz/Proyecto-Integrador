#include "Serie.h"
#include <vector> // This was already there, good.
#include <string> // This was already there, good.

// Add these two lines:
#include <algorithm> // For std::transform
#include <cctype>    // For ::tolower


// Constructor
Serie::Serie(std::string id, std::string nombre, double duracion, std::string genero)
    : Video(id, nombre, duracion, genero) {} // Call base class constructor

// Add an episode to the series
void Serie::agregarEpisodio(const Episodio& ep) {
    episodios.push_back(ep);
}

// Override the virtual mostrarDatos() from Video
void Serie::mostrarDatos() const {
    std::cout << "--- Serie ---\n";
    imprimirInfoBase(); // Use the helper from Video
    std::cout << "Número de episodios: " << episodios.size() << "\n";
    std::cout << "Episodios:\n";
    if (episodios.empty()) {
        std::cout << "    (No hay episodios)\n";
    } else {
        for (const auto& ep : episodios) { // Use const reference here
            ep.mostrarDatos();
        }
    }
    std::cout << "----------------\n";
}

// Display details for all episodes in the series
void Serie::mostrarEpisodios() const {
    if (episodios.empty()) {
        std::cout << "No hay episodios en esta serie.\n";
        return;
    }
    std::cout << "Episodios de '" << nombre << "':\n";
    for (const auto& ep : episodios) {
        ep.mostrarDatos();
    }
}

// Display episodes with a minimum rating
void Serie::mostrarEpisodiosConCalificacion(double calificacionMinima) const {
    bool found = false;
    std::cout << "Episodios de la serie '" << nombre << "' con calificación >= "
              << std::fixed << std::setprecision(1) << calificacionMinima << ":\n";
    for (const auto& ep : episodios) {
        if (ep.getCalificacionPromedio() >= calificacionMinima) {
            ep.mostrarDatos();
            found = true;
        }
    }
    if (!found) {
        std::cout << "    No se encontraron episodios con esa calificación.\n";
    }
}

// Rate a specific episode by title
void Serie::calificarEpisodio(const std::string& tituloEpisodio, int valor) {
    bool found = false;
    for (auto& ep : episodios) { // Needs non-const reference to modify episode
        std::string epiTitulo = ep.getTitulo();
        std::transform(epiTitulo.begin(), epiTitulo.end(), epiTitulo.begin(), ::tolower); // Case-insensitive
        std::string searchTitulo = tituloEpisodio;
        std::transform(searchTitulo.begin(), searchTitulo.end(), searchTitulo.begin(), ::tolower);

        if (epiTitulo == searchTitulo) {
            ep.calificar(valor);
            std::cout << "Episodio '" << ep.getTitulo() << "' calificado. Nueva calificación promedio: "
                      << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Episodio '" << tituloEpisodio << "' no encontrado en esta serie.\n";
    }
}