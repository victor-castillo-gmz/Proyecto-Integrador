#include "serviciostreaming.h"
#include <iostream> // For std::cout, std::cerr
#include <limits> // For std::numeric_limits
#include <algorithm> // For std::transform, std::tolower

// Helper to convert string to lowercase for case-insensitive comparisons
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

// Destructor implementation
ServicioStreaming::~ServicioStreaming() {
    for (Video* video : videos) {
        delete video; // Free dynamically allocated Video objects
    }
    videos.clear();
}

// Helper to find a video by title (case-insensitive)
Video* ServicioStreaming::buscarVideoPorTitulo(const std::string& titulo) const {
    std::string searchTitleLower = toLower(titulo);
    for (Video* video : videos) {
        if (toLower(video->getNombre()) == searchTitleLower) {
            return video;
        }
    }
    // Also check episodes within series for rating
    for (Video* video : videos) {
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie) {
            for (Episodio& ep : serie->getEpisodios()) {
                if (toLower(ep.getTitulo()) == searchTitleLower) {
                    // Return the series if an episode matches, as rating an episode requires access to the series
                    return serie;
                }
            }
        }
    }
    return nullptr;
}


// Load data from file
void ServicioStreaming::cargarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    // Clear existing videos to avoid duplicates if loading multiple times
    for (Video* video : videos) {
        delete video;
    }
    videos.clear();

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string tipo, id, nombre, genero;
        double duracion;
        int temporadas;

        std::getline(ss, tipo, ',');
        std::getline(ss, id, ',');
        std::getline(ss, nombre, ',');
        ss >> duracion; ss.ignore(std::numeric_limits<std::streamsize>::max(), ',');
        std::getline(ss, genero, ',');

        // Trim whitespace from strings
        tipo.erase(0, tipo.find_first_not_of(" \t\n\r\f\v"));
        tipo.erase(tipo.find_last_not_of(" \t\n\r\f\v") + 1);
        id.erase(0, id.find_first_not_of(" \t\n\r\f\v"));
        id.erase(id.find_last_not_of(" \t\n\r\f\v") + 1);
        nombre.erase(0, nombre.find_first_not_of(" \t\n\r\f\v"));
        nombre.erase(nombre.find_last_not_of(" \t\n\r\f\v") + 1);
        genero.erase(0, genero.find_first_not_of(" \t\n\r\f\v"));
        genero.erase(genero.find_last_not_of(" \t\n\r\f\v") + 1);


        if (tipo == "Pelicula") {
            videos.push_back(new Pelicula(id, nombre, duracion, genero));
            // Add any initial ratings if format includes them
        } else if (tipo == "Serie") {
            ss >> temporadas; // Read number of seasons for a series
            ss.ignore(std::numeric_limits<std::streamsize>::max(), ','); // Consume the comma after temporadas

            Serie* nuevaSerie = new Serie(id, nombre, duracion, genero);

            std::string episodioData;
            if (std::getline(ss, episodioData)) { // Read the rest of the line for episode data
                std::stringstream ssEpisodios(episodioData);
                std::string epTitulo;
                int epTemporada;
                // Episodes are expected in the format "TituloEpisodio1:Temporada1;TituloEpisodio2:Temporada2;..."
                std::string segment;
                while (std::getline(ssEpisodios, segment, ';')) {
                    size_t colonPos = segment.find(':');
                    if (colonPos != std::string::npos) {
                        epTitulo = segment.substr(0, colonPos);
                        epTemporada = std::stoi(segment.substr(colonPos + 1));
                        nuevaSerie->agregarEpisodio(Episodio(epTitulo, epTemporada));
                    }
                }
            }
            videos.push_back(nuevaSerie);
        } else {
            std::cerr << "Advertencia: Tipo de video desconocido en la línea: " << linea << std::endl;
        }
    }
    archivo.close();
    std::cout << "Datos cargados exitosamente desde " << nombreArchivo << ". Total de videos: " << videos.size() << std::endl;
}


void ServicioStreaming::mostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& generoFiltro) {
    std::cout << "\n--- Videos filtrados por Calificación (" << std::fixed << std::setprecision(1) << calificacionMinima << "+) o Género ('" << (generoFiltro.empty() ? "Todos" : generoFiltro) << "') ---\n";
    bool found = false;
    std::string generoFiltroLower = toLower(generoFiltro);

    for (const auto& video : videos) {
        bool matchesRating = video->getCalificacionPromedio() >= calificacionMinima;
        bool matchesGenre = generoFiltro.empty() || toLower(video->getGenero()) == generoFiltroLower;

        // For series, also check if any episode meets the rating
        bool hasEpisodeMeetingRating = false;
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie) {
            for (const auto& ep : serie->getEpisodios()) {
                if (ep.getCalificacionPromedio() >= calificacionMinima) {
                    hasEpisodeMeetingRating = true;
                    break;
                }
            }
        }

        if ((matchesRating || hasEpisodeMeetingRating) && matchesGenre) {
            video->mostrarDatos();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No se encontraron videos con los criterios especificados.\n";
    }
    std::cout << "------------------------------------------------------------------\n";
}

void ServicioStreaming::mostrarEpisodiosDeSerieConCalificacion(const std::string& nombreSerie, double calificacionMinima) {
    std::string searchSerieNameLower = toLower(nombreSerie);
    bool serieFound = false;
    for (const auto& video : videos) {
        Serie* serie = dynamic_cast<Serie*>(video);
        if (serie && toLower(serie->getNombre()) == searchSerieNameLower) {
            serieFound = true;
            serie->mostrarEpisodiosConCalificacion(calificacionMinima);
            break;
        }
    }
    if (!serieFound) {
        std::cout << "Serie '" << nombreSerie << "' no encontrada.\n";
    }
}

void ServicioStreaming::mostrarPeliculasConCalificacion(double calificacionMinima) {
    std::cout << "\n--- Películas con calificación >= " << std::fixed << std::setprecision(1) << calificacionMinima << " ---\n";
    bool found = false;
    for (const auto& video : videos) {
        Pelicula* pelicula = dynamic_cast<Pelicula*>(video);
        if (pelicula && pelicula->getCalificacionPromedio() >= calificacionMinima) {
            pelicula->mostrarDatos();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No se encontraron películas con calificación >= " << std::fixed << std::setprecision(1) << calificacionMinima << ".\n";
    }
    std::cout << "---------------------------------------------------------\n";
}

void ServicioStreaming::calificarVideo(const std::string& tituloVideo, int calificacion) {
    std::string titleToRateLower = toLower(tituloVideo);
    bool found = false;

    // First, try to find a direct Video match (Movie or Series itself)
    for (Video* video : videos) {
        if (toLower(video->getNombre()) == titleToRateLower) {
            video->calificar(calificacion);
            std::cout << "Video '" << video->getNombre() << "' calificado. Nueva calificación promedio: "
                      << std::fixed << std::setprecision(1) << video->getCalificacionPromedio() << std::endl;
            found = true;
            break;
        }
    }

    // If not found as a direct video, check if it's an episode within a series
    if (!found) {
        for (Video* video : videos) {
            Serie* serie = dynamic_cast<Serie*>(video);
            if (serie) {
                // The calificarEpisodio method handles finding the episode by title
                // and rating it, and will print messages directly.
                // We're essentially just calling it if it's a series.
                std::vector<Episodio>& episodios = serie->getEpisodios(); // Get mutable list of episodes
                bool episodeFoundInSerie = false;
                for (Episodio& ep : episodios) {
                    if (toLower(ep.getTitulo()) == titleToRateLower) {
                        ep.calificar(calificacion);
                        std::cout << "Episodio '" << ep.getTitulo() << "' de la serie '" << serie->getNombre() << "' calificado. Nueva calificación promedio: "
                                  << std::fixed << std::setprecision(1) << ep.getCalificacionPromedio() << std::endl;
                        episodeFoundInSerie = true;
                        found = true; // Mark overall rating as found
                        break;
                    }
                }
                if (episodeFoundInSerie) {
                    break; // Stop searching once episode is rated
                }
            }
        }
    }

    if (!found) {
        std::cout << "Video o episodio '" << tituloVideo << "' no encontrado.\n";
    }
}