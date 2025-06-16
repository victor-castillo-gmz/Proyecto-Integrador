#include "serie.h"
#include <iostream>
#include <iomanip> // For std::fixed, std::setprecision

Serie::Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : Video(id, nombre, duracion, genero) {}

void Serie::AgregarEpisodio(const Episodio& episodio) {
    episodios.push_back(episodio);
}

const std::vector<Episodio>& Serie::GetEpisodios() const {
    return episodios;
}

void Serie::mostrarDatos() const {
    std::cout << "Tipo: Serie" << std::endl;
    imprimirInfoBase(); // Reuses the base class's info printing
    MostrarEpisodios(); // Now calls a new method to show all episodes
}

void Serie::MostrarEpisodiosConCalificacion(double calificacionMinima) const {
    std::cout << "Episodios de la serie '" << getNombre() << "' con calificación >= "
              << std::fixed << std::setprecision(1) << calificacionMinima << ":" << std::endl;
    bool found_episode = false;
    for (const auto& ep : episodios) {
        if (ep.getCalificacionPromedio() >= calificacionMinima) {
            ep.mostrarDatos();
            found_episode = true;
        }
    }
    if (!found_episode) {
        std::cout << "  No se encontraron episodios con esa calificación." << std::endl;
    }
}

void Serie::MostrarEpisodios() const {
    if (episodios.empty()) {
        std::cout << "  Esta serie no tiene episodios cargados." << std::endl;
        return;
    }
    std::cout << "  Episodios:" << std::endl;
    for (const auto& ep : episodios) {
        ep.mostrarDatos();
    }
}