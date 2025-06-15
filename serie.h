#ifndef SERIE_H
#define SERIE_H

#include "video.h"   // Inherits from Video
#include "episodio.h" // Uses Episodio objects
#include <vector>

class Serie : public Video {
private:
    std::vector<Episodio> episodios;

public:
    // Constructor
    Serie(std::string id, std::string nombre, double duracion, std::string genero);

    // Add an episode to the series
    void agregarEpisodio(const Episodio& ep);

    // Override the virtual mostrarDatos() from Video
    void mostrarDatos() const override;

    // Display details for all episodes in the series
    void mostrarEpisodios() const;

    // Display episodes with a minimum rating
    void mostrarEpisodiosConCalificacion(double calificacionMinima) const;

    // Rate a specific episode by title
    void calificarEpisodio(const std::string& tituloEpisodio, int valor);

    // Getter for episodes (to allow external modification/access for data loading)
    std::vector<Episodio>& getEpisodios() { return episodios; }
};

#endif // SERIE_H