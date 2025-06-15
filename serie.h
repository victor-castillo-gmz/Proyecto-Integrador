#ifndef SERIE_H
#define SERIE_H

#include <string>
#include <vector>
#include "Episodio.h"

class Serie {
private:
    std::string id;
    std::string titulo;
    double duracion; // duración promedio por episodio
    std::string genero;
    std::vector<Episodio> episodios;

public:
    Serie(const std::string& id, const std::string& titulo, double duracion, const std::string& genero);

    void agregarEpisodio(const Episodio& episodio);
    void calificarEpisodio(const std::string& tituloEpisodio, int calificacion);

    const std::vector<Episodio>& getEpisodios() const;
    const std::string& getTitulo() const;
};

#endif // SERIE_H
