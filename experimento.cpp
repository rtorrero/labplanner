#include "experimento.h"
#include <fstream>
#include <sstream>
#include <iostream>

//--------------------------------------------------------
// Cargar experimentos desde CSV
//--------------------------------------------------------
bool cargarExperimentosDeCSV(const std::string &filename,
                             std::vector<Experimento> &experimentos)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo CSV: " << filename << std::endl;
        return false;
    }

    experimentos.clear();
    std::string line;
    bool esPrimeraLinea = true;

    while (std::getline(file, line)) {
        // Ignorar líneas vacías
        if (line.empty()) continue;

        // Podríamos ignorar una posible cabecera si la primera línea no es numérica
        if (esPrimeraLinea) {
            // Comprobar si es cabecera
            bool probableCabecera = false;
            for (char c : line) {
                if (!isdigit(c) && c != ',' && c != '.' && c != '-' && !isspace(c)) {
                    probableCabecera = true;
                    break;
                }
            }
            if (probableCabecera) {
                // Saltamos la cabecera
                esPrimeraLinea = false;
                continue;
            }
            esPrimeraLinea = false;
        }

        std::stringstream ss(line);
        std::string token;

        Experimento exp{};

        // Formato: ID,BarleyMalt,DAP,Time
        // Ejemplo: 1,50.5,2.5,6.5

        // ID
        if (!std::getline(ss, token, ',')) continue;
        exp.id = std::stoi(token);

        // BarleyMalt
        if (!std::getline(ss, token, ',')) continue;
        exp.barleyMalt = std::stod(token);

        // DAP
        if (!std::getline(ss, token, ',')) continue;
        exp.DAP = std::stod(token);

        // Time
        if (!std::getline(ss, token, ',')) continue;
        exp.time = std::stod(token);

        experimentos.push_back(exp);
    }

    file.close();
    return true;
}

//--------------------------------------------------------
// Generar 2 variantes (Sólido/Líquido) × 3 réplicas
//--------------------------------------------------------
std::vector<Preparacion> generarPreparaciones(const std::vector<Experimento> &experimentos)
{
    std::vector<Preparacion> lista;

    for (auto &exp : experimentos) {
        for (int rep = 1; rep <= 3; ++rep) {
            // Variante sólida
            Preparacion pS;
            pS.idExperimento = exp.id;
            pS.variante = "Sólido";
            pS.replica = rep;
            pS.barleyMaltFinal = exp.barleyMalt;
            pS.DAPFinal = exp.DAP;
            pS.time = exp.time;
            lista.push_back(pS);

            // Variante líquida
            Preparacion pL;
            pL.idExperimento = exp.id;
            pL.variante = "Líquido";
            pL.replica = rep;
            pL.barleyMaltFinal = exp.barleyMalt; // Podrías aplicar factor
            pL.DAPFinal = exp.DAP;
            pL.time = exp.time;
            lista.push_back(pL);
        }
    }

    // Podrías ordenarlos aquí, por ejemplo, según barleyMaltFinal y luego DAPFinal
    // (Descomentar si quieres ordenar)
    /*
    std::sort(lista.begin(), lista.end(), [](const Preparacion &a, const Preparacion &b){
        if (a.barleyMaltFinal == b.barleyMaltFinal) {
            return a.DAPFinal < b.DAPFinal;
        }
        return a.barleyMaltFinal < b.barleyMaltFinal;
    });
    */

    return lista;
}
