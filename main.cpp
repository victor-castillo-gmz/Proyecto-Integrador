/**
 * @file main.cpp
 * @brief Punto de entrada principal para la aplicación de consola del servicio de streaming.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include <iostream>
#include <limits>
#include <string>
#include "serviciostreaming.h"

// Prototipos de funciones auxiliares
void ClearInputBuffer();
std::string GetStringInput(const std::string& prompt);
double GetDoubleInput(const std::string& prompt);
int GetIntInput(const std::string& prompt);
void DisplayMenu();

int main() {
    ServicioStreaming servicio;
    int option;

    do {
        DisplayMenu();
        option = GetIntInput("Seleccione una opcion: ");

        switch (option) {
            case 1: {
                std::string filename = GetStringInput("Ingrese el nombre del archivo de datos (ej. datos.txt): ");
                servicio.CargarArchivo(filename);
                break;
            }
            case 2: {
                double minRating = GetDoubleInput("Ingrese la calificacion minima (0-5, 0 para no filtrar): ");
                std::string genreFilter = GetStringInput("Ingrese el genero a filtrar (deje vacio para todos): ");
                servicio.MostrarVideosPorCalificacionOGenero(minRating, genreFilter);
                break;
            }
            case 3: {
                std::string serieName = GetStringInput("Ingrese el titulo de la serie: ");
                double minEpisodeRating = GetDoubleInput("Ingrese la calificacion minima para los episodios (0-5, 0 para no filtrar): ");
                servicio.MostrarEpisodiosDeSerieConCalificacion(serieName, minEpisodeRating);
                break;
            }
            case 4: {
                double minMovieRating = GetDoubleInput("Ingrese la calificacion minima para las peliculas (0-5, 0 para no filtrar): ");
                servicio.MostrarPeliculasConCalificacion(minMovieRating);
                break;
            }
            case 5: {
                std::string titleToRate = GetStringInput("Ingrese el titulo del video o episodio a calificar: ");
                int ratingValue;
                while (true) {
                    ratingValue = GetIntInput("Ingrese la calificacion (1-5): ");
                    if (ratingValue >= 1 && ratingValue <= 5) {
                        break;
                    } else {
                        std::cout << "Calificacion invalida. Debe ser un numero entre 1 y 5.\n";
                    }
                }
                servicio.CalificarVideo(titleToRate, ratingValue);
                break;
            }
            case 0: {
                std::cout << "Saliendo del programa. ¡Hasta luego!\n";
                break;
            }
            default: {
                std::cout << "Opcion invalida. Por favor, intente de nuevo.\n";
                break;
            }
        }
    } while (option != 0);

    return 0;
}

// --- Implementación de funciones auxiliares ---

void ClearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string GetStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

double GetDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < 0 || value > 5) {
            std::cout << "Entrada invalida. Por favor, ingrese un numero entre 0 y 5.\n";
            std::cin.clear();
            ClearInputBuffer();
        } else {
            ClearInputBuffer();
            return value;
        }
    }
}

int GetIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Entrada invalida. Por favor, ingrese un numero entero.\n";
            std::cin.clear();
            ClearInputBuffer();
        } else {
            ClearInputBuffer();
            return value;
        }
    }
}

void DisplayMenu() {
    std::cout << "\n--- MENU DE SERVICIO DE STREAMING ---\n";
    std::cout << "1. Cargar archivo de datos\n";
    std::cout << "2. Mostrar videos en general (filtrar por calificacion o genero)\n";
    std::cout << "3. Mostrar episodios de una serie con calificacion especifica\n";
    std::cout << "4. Mostrar peliculas con calificacion especifica\n";
    std::cout << "5. Calificar un video o episodio\n";
    std::cout << "0. Salir\n";
    std::cout << "-------------------------------------\n";
}