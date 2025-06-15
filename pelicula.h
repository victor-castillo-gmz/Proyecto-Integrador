#ifndef PELICULA_H
#define PELICULA_H

#include <string>
#include <vector>

class Pelicula {
private:
    std::string id;
    std::string titulo;
    double duracion;
    std::string genero;
    std::vector<int> calificaciones;

public:
    Pelicula(const std::string& id, const std::string& titulo, double duracion, const std::string& genero);

    void calificar(int calificacion);
    double getCalificacionPromedio() const;
    const std::vector<int>& getCalificaciones() const;

    const std::string& getTitulo() const;
};

#endif // PELICULA_H
