#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


#include <string>
#include<iostream>
#include <vector>


struct Player
{
    std::string team_name;
    std::string playmode;
    std::string side;
    bool see_ball;
    bool see_own_goal;
    bool see_opponent_goal;
    bool see_left_top;
    int flags_seen;
    int unum;
    double x;
    double y;
    float stamina;
    bool in_zone;
    bool OutOfField;
    std::string rol;
    double distancia_a_zona;
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
    float distance;
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
    std::vector<float> flag_center;
    float flag_center_distance;
    std::vector<float> flag_center_top;
    float flag_center_top_distance;
    std::vector<float> flag_center_bottom;
    float flag_center_bottom_distance;
    std::vector<float> flag_left_top;
    float flag_left_top_distance;
    float flag_left_top_angle;
    std::vector<float> flag_left_bottom;
    float flag_left_bottom_distance;
    std::vector<float> flag_right_top;
    float flag_right_top_distance;
    std::vector<float> flag_right_bottom;
    float flag_right_bottom_distance;
};



#endif // ESTRUCTURAS_H