#ifndef EPISODIO_H
#define EPISODIO_H

#include <string>
#include <vector>

class Episodio {
private:
    std::string titulo;
    int numero;
    std::vector<int> calificaciones;

public:
    Episodio(const std::string& titulo, int numero);

    void calificar(int calificacion);
    double getCalificacionPromedio() const;
    const std::string& getTitulo() const;
};

#endif // EPISODIO_H
