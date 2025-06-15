#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
#include <string>
#include <vector>
#include <numeric>  // For std::accumulate
#include <iomanip>  // For std::setprecision

// Base class for all videos
class Video {
protected:
    std::string id;
    std::string nombre;
    double duracion; // Using double for flexibility, though int is fine too
    std::string genero;
    std::vector<int> calificaciones;
    double calificacionPromedio; // Storing pre-calculated average for efficiency

public:
    // Constructor
    Video(std::string id, std::string nombre, double duracion, std::string genero);

    // Virtual destructor for proper memory cleanup with polymorphism
    virtual ~Video() = default;

    // Pure virtual function - must be implemented by derived classes
    virtual void mostrarDatos() const = 0;

    // Method to add a rating
    void calificar(int valor);

    // Calculate and update the average rating
    void calcularPromedio();

    // Getters marked const as they don't modify object state
    std::string getID() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getGenero() const { return genero; }
    double getCalificacionPromedio() const { return calificacionPromedio; }
    const std::vector<int>& getCalificaciones() const { return calificaciones; } // For saving/loading ratings

protected: // Helper for derived classes to print common video info
    void imprimirInfoBase() const;
};

#endif // VIDEO_H