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
    int jugadoresCerca = 3;   // numJugadoresCerca(player);
    // Coste base
    coste = 50;
    
    // Ajuste por estamina
    if (player.stamina < 7000) {
        coste *= 1.5; // Aumenta el coste si la estamina es baja
    }
    
    // Ajuste por número de jugadores cerca
    if (jugadoresCerca > 2) {
        coste *= 1.2; // Aumenta el coste si hay muchos jugadores cerca
    }

    // Recompensa base
    recompensa = 100 - distPelota; // Mayor recompensa cuanto más cerca de la pelota

    // Ajuste si el jugador está fuera del campo
    // if (player.OutOfField) {
    //     recompensa = 9999; // Recompensa muy alto si está fuera del campo
    // }

    // Ajuste por rol del jugador
    if (player.rol == "DELANTERO") {
        recompensa *= 1.5; // Mayor recompensa para delanteros
    } else if (player.rol == "DEFENSA") {
        coste *= 1.2; // Mayor coste para defensas
    } else if (player.rol == "PORTERO") {
        coste *= 2; // Mayor coste para porteros
    }

    // Ajuste si el jugador no ve la pelota
    if (!player.see_ball) {
        coste *= 2.0; // Aumenta el coste si no ve la pelota
    }

    // Ajuste si el jugador no ve la portería
    if (!player.see_opponent_goal && !player.OutOfField) {
        recompensa *= 0.8; // Disminuye la recompensa si no ve la portería
    }

   //Ajuste si ya esta en la pelota
    if (distPelota < 1.5) {
        recompensa = 1; // Recompensa baja si ya está en la pelota
    }
}


void calcularCosteYRecompensaPasar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    int jugadoresCerca = 3;    // numJugadoresCerca(player);
    // Coste base
    coste = 50;
    // Recompensa base
    recompensa = 50 + distPorteria; // Mayor recompensa cuanto más lejos de la porteria
    // Ajuste por estamina
    if (player.stamina < 7000) {
        recompensa *= 1.5; // Aumenta la recompensa si la estamina es baja
    }
    // Ajuste por número de jugadores cerca
    if (jugadoresCerca > 3) {
        recompensa *= 1.2; // Aumenta la recompensa si hay muchos jugadores cerca
    }
    // Ajuste por la distancia a la pelota
    if (distPelota > 1.5) {
        coste *= 3; // Aumenta la el coste está lejos de la pelota
    }
}

// Función para calcular el coste y la recompensa de chutar
void calcularCosteYRecompensaChutar(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    int jugadoresCerca = 3;    // numJugadoresCerca(player);
    // Coste base
    coste = 50;
    // Recompensa base
    recompensa = 100 - distPorteria; // Mayor recompensa cuanto más cerca de la porteria
    // Ajuste por la distancia a la pelota
    if (distPelota > 5) {
        coste *= 1000; // Aumenta la el coste está lejos de la pelota
    }
    //Ajuste por tipo de jugador
    if (player.rol == "DELANTERO") {
        recompensa *= 1.5; // Mayor recompensa para delanteros
    } else if (player.rol == "DEFENSA") {
        coste *= 1.2; // Mayor coste para defensas
    } else if (player.rol == "PORTERO") {
        coste *= 1.5; // Mayor coste para porteros
    }
    // Ajuste por número de jugadores cerca
    if (jugadoresCerca < 3) {
        recompensa *= 1.2; // Aumenta la recompensa si hay muchos pocos jugadores cerca
    }
    if(!player.see_ball){
        coste *= 100.0; // Aumenta el coste si no ve la pelota
    }

}

// Función para calcular el coste y la recompensa de volver a la zona
void calcularCosteYRecompensaVolverAZona(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message) {
    double distPelota = ball.distance;
    double distPorteria = goal.distance;
    double distancia_a_zona=player.distancia_a_zona;
    int jugadoresCerca = 3;    // numJugadoresCerca(player);
    // Coste base
    coste = 50;
    // Recompensa base
    recompensa=100 + (2*distancia_a_zona); // Disminuye cuanto más lejos de zona está
    // Ajuste por cercanía a portería
    if (distPorteria < 15) {
        coste *= 3; // Aumenta el coste si está cerca de la portería
    } 
    //Ajuste por numero de jugadores cerca
    if (jugadoresCerca>2){
        recompensa *=2;
    }
    // Ajuste por la distancia a la pelota
    if (distPelota < 10) {
        recompensa *=0.5; // Recompensa baja si ya está en la pelota
    }
    //Ajuste por fuera de zona
    if (player.in_zone) {
        coste *= 500; // Recompensa baja si ya está en la zona
    }
    //Ajuste por rol
    if (player.rol == "DELANTERO") {
        coste *= 1.5; // Mayor coste para delanteros
    } else if (player.rol == "DEFENSA") {
        recompensa *= 1.2; // Mayor recompensa para defensas
    } else if (player.rol == "PORTERO") {
        recompensa *= 1.5; // Mayor recompensa para porteros
    }
   //Ajuste por numero de flags que ve
}
void MirarPelota(Player& player, const Ball& ball, const Goal& goal, double& coste, double& recompensa,vector<string> see_message){
    // double distPelota = ball.distance;
    // double distPorteria = goal.distance;
    // double distancia_a_zona=player.distancia_a_zona;
    // int jugadoresCerca = procesarJugadoresVisibles(see_message);    // numJugadoresCerca(player);
    // // Coste base
    // coste = 50;
    // // Recompensa base
    // recompensa=100 - distPelota; // Mayor recompensa cuanto más cerca de la pelota
    // //Ajuste por estar en zona
    // if (player.in_zone) {
    //     recompensa *= 1.5; // Mayor recompensa si ya está en la zona
    // }
    // if(player.see_ball){
    //     recompensa*=; // Aumenta el coste si no ve la pelota
    // }

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
    
    
    cout << "Coste y recompensa de ir a por la pelota: " << resultadoIrAPorPelota << endl;
    cout << "Coste y recompensa de pasar: " << resultadoPasar << endl;
    cout << "Coste y recompensa de chutar: " << resultadoChutar << endl;
    cout << "Coste y recompensa de volver a la zona: " << resultadoVolverAZona << endl;
    cout << "----------------------------------------" << endl;
    cout <<"numer jugadores cerca: "<<"3"<<endl;
    if (resultadoChutar<=-60&&resultadoPasar<=-60&&resultadoIrAPorPelota<=-60&&resultadoVolverAZona<=-60){
        return 8;
    }
    else{
    auto mejorAccion = max_element(acciones.begin(), acciones.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    return mejorAccion->first;
    }

    
}
