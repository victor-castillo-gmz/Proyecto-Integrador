#ifndef SERIE_H
#define SERIE_H

#include "video.h" // Inherits from Video  <-- ENSURE ONLY A SINGLE SPACE HERE
#include "episodio.h" // Uses Episodio objects
#include <vector>
#include <string> // Agregado: Es buena práctica incluir <string> cuando se usa std::string

class Serie : public Video {
private:
    std::vector<Episodio> episodios;

public:
    // Constructor
    Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);

    virtual ~Serie();

    void agregarEpisodio(const Episodio& ep);

    void mostrarDatos() const override;

    void mostrarEpisodios() const;

    void mostrarEpisodiosConCalificacion(double calificacionMinima) const;

    void calificarEpisodio(const std::string& tituloEpisodio, int calificacion);

    const std::vector<Episodio>& getEpisodios() const { return episodios; }
};

#endif // SERIE_H