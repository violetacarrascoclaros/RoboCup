#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


#include <string>
#include<iostream>
#include <vector>
using namespace std;

struct Posicion
{
    double x;
    double y;
};
struct Player
{
    std::string team_name;
    std::string playmode;
    std::string side;
    bool see_ball;
    int flags_seen;
    int unum;
    double x;
    double y;
    double orientation;
    Posicion zone;
    string zone_name;
    bool seeing_zone;
    double range;
    bool see_own_goal;
    bool see_opponent_goal;
    string rol;
    bool see_left_top;
    float stamina;
    bool in_zone;
    bool OutOfField;
    double distancia_a_zona;
    double angle;
    string playmode_prev;
    string jugadorMarcaGol;

};

struct Ball
{
    std::string x;
    std::string y;
    std::string vx;
    std::string vy;
    double distance;
    double angle;
};

struct Goal
{
    std::string x;
    std::string y;
    std::string side;
    float distance;
    double angle;
};

struct JugadorCercano
{
    std::string x;
    std::string y;
    std::string distancia;
    std::string unum;
    std::string nombreEquipo;
    std::string dorsal;
    float distance;
    float angle;
};

struct JugadoresAlrededor
{
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> distancia;
    std::vector<int> unum;
    float distance;
};

struct Field
{
    vector<double> flag_center;
    vector<double> flag_center_abs = {0, 0};
    vector<double> flag_center_top;
    vector<double> flag_center_top_abs = {0.0, -33.5};
    vector<double> flag_center_bottom;
    vector<double> flag_center_bottom_abs = {0, 33.5};
    vector<double> flag_left_top;
    vector<double> flag_left_top_abs = {-52.2, -33.5};
    vector<double> flag_left_bottom;
    vector<double> flag_left_bottom_abs = {-52.2, 33.5};
    vector<double> flag_right_top;
    vector<double> flag_right_top_abs = {52.5, -33.5};
    vector<double> flag_right_bottom;
    vector<double> flag_right_bottom_abs = {52.5, 33.5};

    vector<double> flag_penalty_left_top;
    vector<double> flag_penalty_left_top_abs = {-36, -20};
    vector<double> flag_penalty_left_center;
    vector<double> flag_penalty_left_center_abs = {-36, 0};
    vector<double> flag_penalty_left_bottom;
    vector<double> flag_penalty_left_bottom_abs = {-36, 20};
    vector<double> flag_penalty_right_top;
    vector<double> flag_penalty_right_top_abs = {36, -20};
    vector<double> flag_penalty_right_center;
    vector<double> flag_penalty_right_center_abs = {36, 0};
    vector<double> flag_penalty_right_bottom;
    vector<double> flag_penalty_right_bottom_abs = {36, 20};

    vector<double> flag_goal_left_top;
    vector<double> flag_goal_left_top_abs = {-52.5, -7.32};
    vector<double> flag_goal_left_bottom;
    vector<double> flag_goal_left_bottom_abs = {-52.5, 7.32};
    vector<double> flag_goal_right_top;
    vector<double> flag_goal_right_top_abs = {52.5, -7.32};
    vector<double> flag_goal_right_bottom;
    vector<double> flag_goal_right_bottom_abs = {52.5, 7.32};
};



#endif // ESTRUCTURAS_H