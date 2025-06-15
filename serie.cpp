#include "Serie.h"

Serie::Serie(const std::string& id, const std::string& titulo, double duracion, const std::string& genero)
    : id(id), titulo(titulo), duracion(duracion), genero(genero) {}

void Serie::agregarEpisodio(const Episodio& episodio) {
    episodios.push_back(episodio);
}

void Serie::calificarEpisodio(const std::string& tituloEpisodio, int calificacion) {
    for (auto& ep : episodios) {
        if (ep.getTitulo() == tituloEpisodio) {
            ep.calificar(calificacion);
            break;
        }
    }
}

const std::vector<Episodio>& Serie::getEpisodios() const {
    return episodios;
}

const std::string& Serie::getTitulo() const {
    return titulo;
}
