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
    ~Serie() override = default; // Use override and default for clarity

    // Method to add an episode
    void AgregarEpisodio(const Episodio& episodio); // Renamed for SonarQube convention

    // Getters
    const std::vector<Episodio>& GetEpisodios() const; // Renamed for SonarQube convention

    // Override the virtual function from Video
    void mostrarDatos() const override;

    // New method to show episodes with a minimum rating
    void MostrarEpisodiosConCalificacion(double calificacionMinima) const; // Renamed for SonarQube convention
    void MostrarEpisodios() const; // New method for general listing
};

#endif // SERIE_H