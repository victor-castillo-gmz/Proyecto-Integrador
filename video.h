#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>

class Video {
protected:
    std::string titulo;
    std::vector<int> calificaciones;

public:
    explicit Video(const std::string& titulo);

    virtual ~Video() = default;

    const std::string& getTitulo() const;

    // Añade una calificación válida (entre 1 y 5)
    virtual void calificar(int calificacion);

    // Retorna la calificación promedio, o 0 si no hay calificaciones
    double getCalificacionPromedio() const;

    const std::vector<int>& getCalificaciones() const;
};

#endif // VIDEO_H
