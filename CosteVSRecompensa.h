#ifndef COSTEVSRECOMPENSA_H
#define COSTEVSRECOMPENSA_H

#include <iostream>
#include <vector>
#include <cmath>
#include "functions.h"
#include "estructuras.h"


// Función para calcular el coste y la recompensa de ir a por la pelota
void calcularCosteYRecompensaIrAPorLaPelota(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa);
void calcularCosteYRecompensaPasar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa);
void calcularCosteYRecompensaChutar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa);
void calcularCosteYRecompensaVolverAZona(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa);

int obtenerMejorAccion(Player& player, const Ball& ball, const Goal& goal);

#endif // COSTEVSRECOMPENSA_H