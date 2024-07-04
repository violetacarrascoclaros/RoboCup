#ifndef COSTEVSRECOMPENSA_H
#define COSTEVSRECOMPENSA_H

#include <iostream>
#include <vector>
#include <cmath>
#include "functions.h"
#include "estructuras.h"


// Función para calcular el coste y la recompensa de ir a por la pelota
void calcularCosteYRecompensaIrAPorLaPelota(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message);
void calcularCosteYRecompensaPasar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message);
void calcularCosteYRecompensaChutar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message);
void calcularCosteYRecompensaVolverAZona(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message);
void MirarPelota(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message);

int obtenerMejorAccion(Player& player, const Ball& ball, const Goal& goal,vector<string> see_message);

#endif // COSTEVSRECOMPENSA_H