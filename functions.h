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