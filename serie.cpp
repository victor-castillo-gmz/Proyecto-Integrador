/**
 * @file serie.cpp
 * @brief Implementación de la clase Serie.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "serie.h"
#include <iostream>
#include <iomanip>

Serie::Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero)
    : Video(id, nombre, duracion, genero) {}

void Serie::AgregarEpisodio(const Episodio& episodio) {
    episodios.push_back(episodio);
}

const std::vector<Episodio>& Serie::GetEpisodios() const {
    return episodios;
}

std::vector<Episodio>& Serie::GetEpisodiosMutables() {
    return episodios;
}

void Serie::MostrarDatos() const {
    std::cout << "Tipo: Serie" << std::endl;
    ImprimirInfoBase();
    MostrarEpisodios();
}

void Serie::MostrarEpisodios() const {
    if (episodios.empty()) {
        std::cout << "  Esta serie no tiene episodios cargados." << std::endl;
        return;
    }
    std::cout << "  Episodios:" << std::endl;
    for (const auto& ep : episodios) {
        ep.MostrarDatos();
    }
}

void Serie::MostrarEpisodiosConCalificacion(double calificacionMinima) const {
    std::cout << "Episodios de la serie '" << GetNombre() << "' con calificacion >= "
              << std::fixed << std::setprecision(1) << calificacionMinima << ":" << std::endl;
    
    bool found_episode = false;
    for (const auto& ep : episodios) {
        if (ep.GetCalificacionPromedio() >= calificacionMinima) {
            ep.MostrarDatos();
            found_episode = true;
        }
    }
    if (!found_episode) {
        std::cout << "  No se encontraron episodios con esa calificacion." << std::endl;
    }
}