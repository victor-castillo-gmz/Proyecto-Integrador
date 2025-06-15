#include <iostream>
#include "Pelicula.h"
#include "Serie.h"
#include "Episodio.h"
#include "ServicioStreaming.h"

int main() {
    ServicioStreaming servicio;

    // Crear y agregar una película
    Pelicula pelicula1("P001", "Inception", 120.0, "Ciencia Ficción");
    servicio.agregarPelicula(pelicula1);

    // Calificar la película
    servicio.calificarVideo("Inception", 5);
    servicio.calificarVideo("Inception", 4);

    std::cout << "Calificación promedio de 'Inception': "
              << servicio.obtenerCalificacion("Inception") << std::endl;

    // Crear una serie
    Serie serie1("S001", "Breaking Bad", 60.0, "Drama");
    Episodio ep1("Piloto", 1);
    Episodio ep2("Cat's in the Bag...", 2);

    // Calificar episodios individualmente
    ep1.calificar(4);
    ep2.calificar(5);

    serie1.agregarEpisodio(ep1);
    serie1.agregarEpisodio(ep2);

    servicio.agregarSerie(serie1);

    // Calificar un episodio a través del servicio
    servicio.calificarVideo("Piloto", 5);
