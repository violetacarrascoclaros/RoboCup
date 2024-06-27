#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <cmath>
#include"estructuras.h"
using namespace std;

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);

void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field);

vector<float> trilateration(vector<float> &P1, vector<float> &P2, vector<float> &P3);

bool estasentusitio (const Field &field, const Player &player, const Goal &own_goal, const Goal &opponent_goal);