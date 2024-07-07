#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <cmath>
#include"estructuras.h"
#include <MinimalSocket/udp/UdpSocket.h>
using namespace std;

void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient);

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);

void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field);

bool estasentusitio (const Field &field, const Player &player, const Goal &own_goal, const Goal &opponent_goal);

string dash(double power, double angle);

void store_data_hear(string &hear_message, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp, Ball &ball);

void chutarPorteria(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

int procesarJugadoresVisibles(vector<string> see_message,Player player);

void mostrarJugadoresVisibles(const vector<JugadorCercano> &jugadores_visibles);

void configurePlayer(Player &player);

string returnToZone(Player const &player);

void imInZone(Player &player);

void pasar(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

void store_data_senseBody(string &senseBody_message, Player &player);

// Función para procesar mensajes 'hear'
void store_data_hear(string &hear_message, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para manejar modos de juego
// void handle_game_mode(const string &modo, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// // Función para manejar saques de portería
// void execute_goal_kick(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// // Función para manejar saques de banda
// void execute_throw_in(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// // Función para manejar saques de esquina
// void execute_corner_kick(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// // Función para manejar faltas
// void execute_free_kick(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// // Función para orientar al jugador hacia el campo
// void orientarJugadorHaciaCampo(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

 void funcion_modos_juego(const string &modo, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp, Ball &ball);

#endif


