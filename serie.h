#ifndef SERIE_H
#define SERIE_H

#include "video.h"
#include "episodio.h"
#include <vector>
#include <string>

class Serie : public Video {
private:
    std::vector<Episodio> episodios;

public:
    Serie(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);
    virtual ~Serie();

    void agregarEpisodio(const Episodio& ep);
    void mostrarDatos() const override;
    void mostrarEpisodios() const;
    void mostrarEpisodiosConCalificacion(double calificacionMinima) const;
    void calificarEpisodio(const std::string& tituloEpisodio, int calificacion);

    // MODIFICACION: Añadimos un getter no-const para permitir la modificación
    std::vector<Episodio>& getEpisodios() { return episodios; } // Non-const version

    // Ya tenías esta versión const, que es buena para acceso de solo lectura
    const std::vector<Episodio>& getEpisodios() const { return episodios; }
};

#endif // SERIE_H