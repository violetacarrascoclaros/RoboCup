#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> 
#include "CosteVSRecompensa.h"
#include "functions.h"
#include "estructuras.h"


// Función para calcular el coste y la recompensa de ir a por la pelota
void calcularCosteYRecompensaIrAPorLaPelota(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    int jugadoresCerca = player.num_jug_cerca;
    // Coste base
    coste = 30;
    //Recompensa base
    recompensa=50;
    //Ajuste por distancia a zona
    if(player.distancia_a_zona>15){
        recompensa+=5;
    }
    else if(player.distancia_a_zona>30){
        recompensa+=10;
    }
    
    if(player.see_opponent_goal){
        recompensa+=20;
    }
    // Ajuste por estamina
    // if (player.stamina < 7500) {
    //     coste += 10; // Aumenta el coste si la estamina es baja
    // }
    
    // Ajuste por número de jugadores cerca
    if (jugadoresCerca >= 3) {
        coste +=10; // Aumenta el coste si hay muchos jugadores cerca
    }

    // Ajuste por rol del jugador
    if (player.rol == "DELANTERO") {
        recompensa += 20; // Mayor recompensa para delanteros
    } else if (player.rol == "DEFENSA") {
        coste += 5; // Mayor coste para defensas
    } else if (player.rol == "PORTERO") {
        coste += 15; // Mayor coste para porteros
    }
    //Ajuste por distancia a la pelota
    if (distPelota > 10) {
        coste += 10; // Aumenta el coste si está lejos de la pelota
    }
    else if(distPelota>20){
        coste+=20;
    }
    else if(distPelota>30){
        coste+=30;
    }
    else if(distPelota>40){
        coste+=40;
    }
    else if(distPelota>50){
        coste+=50;
    }

   //Ajuste si ya esta en la pelota
    if (distPelota < 1 ) {
        cout <<ball.distance<<endl;
        coste=100; 
        recompensa=0;
    }
}


// Función para calcular el coste y la recompensa de chutar
void calcularCosteYRecompensaPasar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    int jugadoresCerca = player.num_jug_cerca; 
    // Coste base
    coste = 50;
    // Recompensa base
    recompensa = 40; 
    // Ajuste por estamina
    if (player.stamina < 7000) {
        recompensa += 10; // Aumenta la recompensa si la estamina es baja
    }
    // Ajuste por número de jugadores cerca
    if (jugadoresCerca > 3) {
        recompensa += 10; // Aumenta la recompensa si hay muchos jugadores cerca
    }
    //Ajuste segun si ve la porteria o no 
    if(!player.see_opponent_goal){
        recompensa+=10;
    }
    //Ajuste segun distancia a porteria
    if(distPorteria>30){
        recompensa+=10;
    }
    // Ajuste por la distancia a la pelota
    if (distPelota > 1.3) {
        coste += 30; // Aumenta la el coste está lejos de la pelota
    }
    if(distPelota<1) {
        recompensa+=15;
    }
    if(player.see_own_goal){
        coste+=20;
    }
    if(jugadoresCerca==0){
        coste=100;
        recompensa=0;
    }
}


// Función para calcular el coste y la recompensa de chutar
void calcularCosteYRecompensaChutar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    int jugadoresCerca = player.num_jug_cerca;    // numJugadoresCerca(player);
    // Coste base
    coste = 50;
    // Recompensa base
    recompensa = 50; 
    // Ajuste por la distancia a la pelota
    if (distPelota > 5) {
        coste += 10; // Aumenta la el coste está lejos de la pelota
    }
    //Ajuste por distancia a porteria
    if(distPorteria>10){
        coste+=5;
    }
    else if(distPorteria>20){
        coste+=10;
    }
    else if(distPorteria>30){
        coste+=15;
    }
    else if(distPorteria>40){
        coste+=20;
    }
    if(distPorteria<10){
        recompensa+=20;
    }
    //Ajuste por tipo de jugador
    if (player.rol == "DELANTERO") {
        recompensa +=20; // Mayor recompensa para delanteros
    } else if (player.rol == "DEFENSA") {
        coste -= 10; // Mayor coste para defensas
    } else if (player.rol == "PORTERO") {
        coste += 20; // Mayor coste para porteros
    }
    // Ajuste por número de jugadores cerca
    if (jugadoresCerca < 3) {
        recompensa += 10; // Aumenta la recompensa si hay muchos pocos jugadores cerca
    }
    if(!player.see_ball){
        coste = 100; 
        recompensa=0;
    }
    if(player.see_own_goal){
        coste=100;
        recompensa=0;
    }

    if(distPelota<1.3&&player.see_opponent_goal&&goal.distance<25){
        recompensa=100;
        coste=0;
    }

}


// Función para calcular el coste y la recompensa de volver a la zona
void calcularCosteYRecompensaVolverAZona(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    double distancia_a_zona=player.distancia_a_zona;
    int jugadoresCerca = player.num_jug_cerca;    // numJugadoresCerca(player);
    // Coste base
    coste = 30;
    // Recompensa base
    recompensa=41;
    // Ajuste por cercanía a portería
    if (distPorteria < 15) {
        coste += 25; // Aumenta el coste si está cerca de la portería
    } 
    else if(distPorteria<10){
        coste+=30;
    }
    else if(distPorteria<5){
        coste+=40;
    }
    //Ajuste por cercania a zona
    if(distancia_a_zona>30){
        recompensa+=10;
    }  
    //Ajuste por numero de jugadores cerca
    if (jugadoresCerca>=2){
        recompensa +=10;
    }
    // Ajuste por la distancia a la pelota
    if (distPelota < 20) {
        coste+=15; // Recompensa baja si ya está en la pelota
    }
    else if(distPelota<5){
        coste+=35;
    }
    if(distPelota>30){
        recompensa+=20;
    }
    //Ajuste por rol
    if (player.rol == "DELANTERO") {
        coste += 15; // Mayor coste para delanteros
    } else if (player.rol == "DEFENSA") {
        recompensa += 0; // Mayor recompensa para defensas
    } else if (player.rol == "PORTERO") {
        recompensa += 20; // Mayor recompensa para porteros
    }
    //Ajuste por fuera de zona
    if (player.in_zone) {
        coste = 100; // Recompensa baja si ya está en la zona
        recompensa=0;
    }
}



void calcularCosteYRecompensaIrconPelota(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message){
    
}

int obtenerMejorAccion(Player& player, const Ball& ball, const Goal& goal,vector<string> see_message) {
    double costeIrAPorPelota = 0.0, recompensaIrAPorPelota = 0.0;
    double costePasar = 0.0, recompensaPasar = 0.0;
    double costeChutar = 0.0, recompensaChutar = 0.0;
    double costeVolverAZona = 0.0, recompensaVolverAZona = 0.0;
    double resultadoIrAPorPelota = 0.0;
    double resultadoPasar = 0.0;
    double resultadoChutar = 0.0;
    double resultadoVolverAZona = 0.0;

    calcularCosteYRecompensaIrAPorLaPelota(player, ball, goal, costeIrAPorPelota, recompensaIrAPorPelota,see_message);
    calcularCosteYRecompensaPasar(player, ball, goal, costePasar, recompensaPasar,see_message);
    calcularCosteYRecompensaChutar(player, ball, goal, costeChutar, recompensaChutar,see_message);
    calcularCosteYRecompensaVolverAZona(player, ball, goal, costeVolverAZona, recompensaVolverAZona,see_message);

    resultadoIrAPorPelota = recompensaIrAPorPelota - costeIrAPorPelota;
    resultadoPasar = recompensaPasar - costePasar;
    resultadoChutar = recompensaChutar - costeChutar;
    resultadoVolverAZona = recompensaVolverAZona - costeVolverAZona;
    
    std::vector<std::pair<int, double>> acciones = {
        {0, resultadoIrAPorPelota},
        {1, resultadoPasar},
        {2, resultadoChutar},
        {3, resultadoVolverAZona}
    };
    
    
    cout << "Coste y recompensa de ir a por la pelota: " <<costeIrAPorPelota<<" "<<recompensaIrAPorPelota<<" "<< resultadoIrAPorPelota << endl;
    cout << "Coste y recompensa de pasar: " <<costePasar<<" "<<recompensaPasar<<" "<< resultadoPasar << endl;
    cout << "Coste y recompensa de chutar: "<<costeChutar<<" "<<recompensaChutar<<" " << resultadoChutar << endl;
    cout << "Coste y recompensa de volver a la zona: " <<costeVolverAZona<<" "<<recompensaVolverAZona<<" "<< resultadoVolverAZona << endl;
    cout << "----------------------------------------" << endl;
    cout <<"numer jugadores cerca: "<<player.num_jug_cerca<<endl;
    if (resultadoChutar<=-5&&resultadoPasar<=-5&&resultadoIrAPorPelota<=-5&&resultadoVolverAZona<=-5){
        return 8;
    }
    else{
    auto mejorAccion = max_element(acciones.begin(), acciones.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    return mejorAccion->first;
    }

    
}
