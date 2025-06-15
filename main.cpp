#include <iostream>
#include <limits> // For numeric_limits
#include <string>
#include "servicioStreaming.h" // Include the streaming service header

// Helper function to clear input buffer
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Helper function for validated string input
std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}
//
// Helper function for validated double input (for ratings)
double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < 0 || value > 5) { // Assuming 0-5 for general filters
            std::cout << "Entrada inválida. Por favor, ingrese un número entre 0 y 5.\n";
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

// Helper function for validated integer input (for menu options and specific ratings)
int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Entrada inválida. Por favor, ingrese un número entero.\n";
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

// Function to display the main menu
void displayMenu() {
    std::cout << "\n--- MENÚ DE SERVICIO DE STREAMING ---\n";
    std::cout << "1. Cargar archivo de datos\n";
    std::cout << "2. Mostrar videos en general (filtrar por calificación o género)\n";
    std::cout << "3. Mostrar episodios de una serie con calificación específica\n";
    std::cout << "4. Mostrar películas con calificación específica\n";
    std::cout << "5. Calificar un video o episodio\n";
    std::cout << "0. Salir\n";
    std::cout << "-------------------------------------\n";
}

int main() {
    ServicioStreaming servicio;
    int option;

    do {
        displayMenu();
        option = getIntInput("Seleccione una opción: ");

        switch (option) {
            case 1: {
                std::string filename = getStringInput("Ingrese el nombre del archivo de datos (ej. datos.txt): ");
                servicio.cargarArchivo(filename);
                break;
            }
            case 2: {
                double minRating = getDoubleInput("Ingrese la calificación mínima (0-5, 0 para no filtrar): ");
                std::string genreFilter = getStringInput("Ingrese el género a filtrar (deje vacío para todos): ");
                servicio.mostrarVideosPorCalificacionOGenero(minRating, genreFilter);
                break;
            }
            case 3: {
                std::string serieName = getStringInput("Ingrese el título de la serie: ");
                double minEpisodeRating = getDoubleInput("Ingrese la calificación mínima para los episodios (0-5, 0 para no filtrar): ");
                servicio.mostrarEpisodiosDeSerieConCalificacion(serieName, minEpisodeRating);
                break;
            }
            case 4: {
                double minMovieRating = getDoubleInput("Ingrese la calificación mínima para las películas (0-5, 0 para no filtrar): ");
                servicio.mostrarPeliculasConCalificacion(minMovieRating);
                break;
            }
            case 5: {
                std::string titleToRate = getStringInput("Ingrese el título del video o episodio a calificar: ");
                int ratingValue;
                while (true) {
                    ratingValue = getIntInput("Ingrese la calificación (1-5): ");
                    if (ratingValue >= 1 && ratingValue <= 5) {
                        break;
                    } else {
                        std::cout << "Calificación inválida. Debe ser un número entre 1 y 5.\n";
                    }
                }
                servicio.calificarVideo(titleToRate, ratingValue);
                break;
            }
            case 0: {
                std::cout << "Saliendo del programa. ¡Hasta luego!\n";
                break;
            }
            default: {
                std::cout << "Opción inválida. Por favor, intente de nuevo.\n";
                break;
            }
        }
    } while (option != 0);

    return 0;
}