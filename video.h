#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>
#include <numeric> // For std::accumulate
#include <iostream> // For std::cout
#include <iomanip>  // For std::fixed, std::setprecision

class Video {
protected:
    std::string id;
    std::string nombre;
    double duracion;
    std::string genero;
    std::vector<int> calificaciones;

public:
    Video(const std::string& id, const std::string& nombre, double duracion, const std::string& genero);
    virtual ~Video() = default; // Virtual destructor for proper polymorphic cleanup

    // Getters
    std::string getId() const;
    std::string getNombre() const;
    double getDuracion() const;
    std::string getGenero() const;
    double getCalificacionPromedio() const;

    void calificar(int calificacion);

    // Virtual function to display details, to be overridden by derived classes
    virtual void mostrarDatos() const;

protected:
    // Helper to print common video info
    void imprimirInfoBase() const;
};

#endif // VIDEO_H