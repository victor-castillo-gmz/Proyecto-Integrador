/**
 * @file serviciostreaming.cpp
 * @brief Implementación de la clase ServicioStreaming.
 * @author Tu Nombre
 * @date 2025-06-15
 */

#include "serviciostreaming.h"
#include "pelicula.h"
#include "serie.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

std::string ServicioStreaming::ToLower(std::string s) const {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

void ServicioStreaming::CargarArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    videos.clear();
    videosPorId.clear();
    episodiosPorTitulo.clear();

    std::string linea;
    while (std::getline(archivo, linea)) {
        ParseLine(linea);
    }

    archivo.close();
    IndexVideos(); // Crear índices para búsquedas rápidas
    std::cout << "Archivo '" << nombreArchivo << "' cargado exitosamente." << std::endl;
}

void ServicioStreaming::ParseLine(const std::string& linea) {
    std::stringstream ss(linea);
    char tipo;
    ss >> tipo;
    ss.ignore(); 

    switch (tipo) {
        case 'M':
            ParseMovie(ss);
            break;
        case 'S':
            ParseSeries(ss);
            break;
        case 'C':
            ParseRatings(ss);
            break;
        case 'E':
            ParseEpisodes(ss);
            break;
        default:
            std::cerr << "Advertencia: Tipo de linea desconocido '" << tipo << "'" << std::endl;
            break;
    }
}

void ServicioStreaming::ParseMovie(std::stringstream& ss) {
    std::string id, nombre, genero;
    double duracion;
    std::getline(ss, id, ',');
    std::getline(ss, nombre, ',');
    ss >> duracion;
    ss.ignore();
    std::getline(ss, genero);
    videos.push_back(std::make_unique<Pelicula>(id, nombre, duracion, genero));
}

void ServicioStreaming::ParseSeries(std::stringstream& ss) {
    std::string id, nombre, genero;
    double duracion_placeholder = 0; // Duración no es relevante para el objeto Serie en sí
    std::getline(ss, id, ',');
    std::getline(ss, nombre, ',');
    std::getline(ss, genero);
    videos.push_back(std::make_unique<Serie>(id, nombre, duracion_placeholder, genero));
}

void ServicioStreaming::ParseRatings(std::stringstream& ss) {
    std::string id;
    int calificacion;
    std::getline(ss, id, ',');

    // Reconstruir el mapa si es necesario (primera vez que se carga)
    if (videosPorId.empty()) {
        for (const auto& v : videos) {
            videosPorId[v->GetId()] = v.get();
        }
    }
    
    auto it = videosPorId.find(id);
    if (it != videosPorId.end()) {
        while (ss >> calificacion) {
            it->second->Calificar(calificacion);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
    } else {
        std::cerr << "Advertencia: ID de video no encontrado para calificacion: " << id << std::endl;
    }
}

void ServicioStreaming::ParseEpisodes(std::stringstream& ss) {
    std::string serieId, tituloEpisodio;
    int temporada;
    std::getline(ss, serieId, ',');
    std::getline(ss, tituloEpisodio, ',');
    ss >> temporada;
    
    auto it = videosPorId.find(serieId);
    if (it != videosPorId.end()) {
        Serie* serie = dynamic_cast<Serie*>(it->second);
        if (serie) {
            serie->AgregarEpisodio(Episodio(tituloEpisodio, temporada));
        } else {
            std::cerr << "Advertencia: ID '" << serieId << "' no corresponde a una serie." << std::endl;
        }
    } else {
        std::cerr << "Advertencia: ID de serie no encontrado para episodio: " << serieId << std::endl;
    }
}

void ServicioStreaming::IndexVideos() {
    videosPorId.clear();
    episodiosPorTitulo.clear();
    for (auto& video : videos) {
        videosPorId[video->GetId()] = video.get();
        Serie* serie = dynamic_cast<Serie*>(video.get());
        if (serie) {
            for (auto& episodio : serie->GetEpisodiosMutables()) {
                episodiosPorTitulo[ToLower(episodio.GetTitulo())] = &episodio;
            }
        }
    }
}

void ServicioStreaming::CalificarVideo(const std::string& titulo, int calificacion) {
    std::string tituloLower = ToLower(titulo);
    bool encontrado = false;

    // Buscar en episodios
    auto itEp = episodiosPorTitulo.find(tituloLower);
    if (itEp != episodiosPorTitulo.end()) {
        itEp->second->Calificar(calificacion);
        std::cout << "Episodio '" << itEp->second->GetTitulo() << "' calificado con " << calificacion << "." << std::endl;
        encontrado = true;
    }

    // Buscar en videos (películas y series) si no se encontró como episodio
    if (!encontrado) {
        for (const auto& video : videos) {
            if (ToLower(video->GetNombre()) == tituloLower) {
                video->Calificar(calificacion);
                std::cout << "Video '" << video->GetNombre() << "' calificado con " << calificacion << "." << std::endl;
                encontrado = true;
                break; 
            }
        }
    }

    if (!encontrado) {
        std::cout << "Video o episodio con titulo '" << titulo << "' no encontrado." << std::endl;
    }
}

void ServicioStreaming::MostrarVideosPorCalificacionOGenero(double calificacionMinima, const std::string& genero) {
    std::cout << "\n--- Mostrando videos con calificacion >= " << calificacionMinima;
    if (!genero.empty()) {
        std::cout << " y genero '" << genero << "'";
    }
    std::cout << " ---\n" << std::endl;

    bool encontrado = false;
    std::string generoLower = ToLower(genero);

    for (const auto& video : videos) {
        bool calificacionOk = video->GetCalificacionPromedio() >= calificacionMinima;
        bool generoOk = genero.empty() || ToLower(video->GetGenero()) == generoLower;

        if (calificacionOk && generoOk) {
            video->MostrarDatos();
            std::cout << "--------------------" << std::endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        std::cout << "No se encontraron videos que coincidan con los criterios." << std::endl;
    }
}

void ServicioStreaming::MostrarEpisodiosDeSerieConCalificacion(const std::string& tituloSerie, double calificacionMinima) {
    std::string tituloLower = ToLower(tituloSerie);
    bool serieEncontrada = false;

    for (const auto& video : videos) {
        Serie* serie = dynamic_cast<Serie*>(video.get());
        if (serie && ToLower(serie->GetNombre()) == tituloLower) {
            serie->MostrarEpisodiosConCalificacion(calificacionMinima);
            serieEncontrada = true;
            break;
        }
    }

    if (!serieEncontrada) {
        std::cout << "Serie con titulo '" << tituloSerie << "' no encontrada." << std::endl;
    }
}

void ServicioStreaming::MostrarPeliculasConCalificacion(double calificacionMinima) {
    std::cout << "\n--- Mostrando peliculas con calificacion >= " << calificacionMinima << " ---\n" << std::endl;
    bool encontrado = false;
    for (const auto& video : videos) {
        if (dynamic_cast<Pelicula*>(video.get()) && video->GetCalificacionPromedio() >= calificacionMinima) {
            video->MostrarDatos();
            std::cout << "--------------------" << std::endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "No se encontraron peliculas con esa calificacion." << std::endl;
    }
}