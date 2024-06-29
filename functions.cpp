#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;

// Include headers
#include "functions.h"
#include "estructuras.h"

// Functions

// Parse initial message function (init Side Unum PlayMode)
Player parseInitialMessage(std::string &message, Player &player)
{
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = message.find(delimiter)) != std::string::npos)
    {
        token = message.substr(0, pos);
        if (i == 1)
        {
            player.side = token;
        }
        else if (i == 2)
        {
            player.unum = std::stoi(token);
        }
        else if (i == 3)
        {
            player.playmode = token;
        }
        message.erase(0, pos + delimiter.length());
        i++;
    }
    cout << "Player: " << player.side << " " << player.unum << " " << player.playmode << endl;
    return player;
}

vector<string> separate_string_separator(string &s, string separator)
{
    vector<string> v;
    int pos = 0;
    while ((pos = s.find(separator)) != string::npos)
    {
        v.push_back(s.substr(0, pos));
        s.erase(0, pos + separator.length());
    }
    v.push_back(s);
    return v;
}

// Parse string function
vector<string> separate_string(string &s)
{
    // Separa por contenido entre parentesis, por nivel: "((hola) (soy) (dani)) (que tal) (estas)" -> {(hola) (soy) (dani), que tal, estas}
    vector<string> v;
    int pos{0};
    int level{0};
    string temp;

    // Recorro el string
    while (pos < s.size())
    {
        if (s[pos] == '(')
        {
            level++;
        }
        else if (s[pos] == ')')
        {
            level--;
        }

        // Dependiendo del nivel, se añade al vector o se reinicia el string temporal
        if (s[pos] == '(' && level == 1)
        {
            temp = "";
        }
        else if (s[pos] == ')' && level == 0)
        {
            v.push_back(temp);
        }
        else
        {
            temp += s[pos];
        }
        pos++;
    }

    // Compruebo que se han abierto y cerrado los mismos parentesis
    if (level != 0)
    {
        throw std::runtime_error("No se han abierto y cerrado los mismos parentesis");
    }
    else
    {
        return v;
    }
}

// Find data in see message function
void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field)
{

    vector<string> ball_coords;
    bool found_ball = false;
    player.flags_seen = 0;
    if (own_goal.side == "r")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }
    if (own_goal.side == "l")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }
    for (size_t i = 0; i < see_message.size(); i++)
    {
        // Search for the ball
        if (see_message[i].find("(b)") != string::npos)
        {
            cout << "The player sees the ball" << endl;
            player.see_ball = true;
            ball_coords = separate_string_separator(see_message[i], " ");
            ball.x = ball_coords[1];
            ball.y = ball_coords[2];
            cout << "Ball coordinates: " << ball.x << " " << ball.y << endl;
            // Calculate the distance to the ball
            double distance = sqrt(pow(stod(ball.x), 2) + pow(stod(ball.y), 2));
            ball.distance = distance;
            // Calculate the angle to the ball
            double angle = atan2(stod(ball.y), stod(ball.x));
            ball.angle = angle * 180 / M_PI;
            found_ball = true;
            // cout << "Angle to the ball: " << angle << endl;
        }

        // Search for the right goal
        if (see_message[i].find("(g r)") != string::npos)
        {
            cout << "The player sees the right goal" << endl;
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "r")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                own_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                own_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                opponent_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        // Search for the left goal
        if (see_message[i].find("(g l)") != string::npos)
        {
            cout << "The player sees the left goal" << endl;
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "l")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                own_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                own_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                opponent_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        if (see_message[i].find("(f c)") != string::npos)
        {
            vector<string> center_coords = separate_string_separator(see_message[i], " ");
            field.flag_center = {stof(center_coords[2]), stof(center_coords[3])};
            field.flag_center_distance = sqrt(pow(stof(center_coords[2]), 2) + pow(stof(center_coords[3]), 2));
            cout << "Center flag coordinates: " << field.flag_center[0] << " " << field.flag_center[1] << endl;
        }

        // Search for the center top flag
        if (see_message[i].find("(f c t)") != string::npos)
        {
            vector<string> center_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_top = {stof(center_top_coords[3]), stof(center_top_coords[4])};
            field.flag_center_top_distance = sqrt(pow(stof(center_top_coords[3]), 2) + pow(stof(center_top_coords[4]), 2));
            player.flags_seen++;
        }

        // Search for the center bottom flag
        if (see_message[i].find("(f c b)") != string::npos)
        {
            vector<string> center_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_bottom = {stof(center_bottom_coords[3]), stof(center_bottom_coords[4])};
            field.flag_center_bottom_distance = sqrt(pow(stof(center_bottom_coords[3]), 2) + pow(stof(center_bottom_coords[4]), 2));
            player.flags_seen++;
        }

        // Search for the left top flag
        if (see_message[i].find("(f l t)") != string::npos)
        {
            vector<string> left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_top = {stof(left_top_coords[3]), stof(left_top_coords[4])};
            field.flag_left_top_distance = sqrt(pow(stof(left_top_coords[3]), 2) + pow(stof(left_top_coords[4]), 2));
            player.flags_seen++;
        }

        // Search for the left bottom flag
        if (see_message[i].find("(f l b)") != string::npos)
        {
            vector<string> left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_bottom = {stof(left_bottom_coords[3]), stof(left_bottom_coords[4])};
            field.flag_left_bottom_distance = sqrt(pow(stof(left_bottom_coords[3]), 2) + pow(stof(left_bottom_coords[4]), 2));
            player.flags_seen++;
        }

        // Search for the right top flag
        if (see_message[i].find("(f r t)") != string::npos)
        {
            vector<string> right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_top = {stof(right_top_coords[3]), stof(right_top_coords[4])};
            field.flag_right_top_distance = sqrt(pow(stof(right_top_coords[3]), 2) + pow(stof(right_top_coords[4]), 2));
            player.flags_seen++;
        }

        // Search for the right bottom flag
        if (see_message[i].find("(f r b)") != string::npos)
        {
            vector<string> right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_bottom = {stof(right_bottom_coords[3]), stof(right_bottom_coords[4])};
            field.flag_right_bottom_distance = sqrt(pow(stof(right_bottom_coords[3]), 2) + pow(stof(right_bottom_coords[4]), 2));
            player.flags_seen++;
        }
    }
    if (found_ball == false)
    {
        player.see_ball = false;
    }
}

bool estasentusitio(const Field &field, const Player &player, const Goal &own_goal, const Goal &opponent_goal)
{
    if (player.side == "l")
    {
        switch (player.unum)
        {
        case 1:
            if ((field.flag_left_top_distance > 27 || field.flag_left_top_distance <=0.1) &&
                (field.flag_left_bottom_distance > 27 || field.flag_left_bottom_distance <=0.1) &&
                (opponent_goal.distance > 90 || opponent_goal.distance <=0.1))
            {
                return true;
            }

            break;
        case 2:
            if ((own_goal.distance > 7 || own_goal.distance <=0.1) &&
                (field.flag_left_top_distance > 33 || field.flag_left_top_distance <=0.1) &&
                (field.flag_left_bottom_distance > 14.5 || field.flag_left_bottom_distance <=0.1) &&
                (field.flag_center_bottom_distance > 42.5 || field.flag_center_bottom_distance <=0.1) &&
                (opponent_goal.distance > 77.5 || opponent_goal.distance <=0.1))
            {
                return true;
            }
            break;
        case 3:
            if ((own_goal.distance > 7 || own_goal.distance <=0.1) &&
                (field.flag_left_bottom_distance > 33 || field.flag_left_bottom_distance <=0.1) &&
                (field.flag_left_top_distance > 14.5 || field.flag_left_top_distance <=0.1) &&
                (field.flag_center_top_distance > 30 || field.flag_center_top_distance <=0.1) &&
                (opponent_goal.distance > 77.5 || opponent_goal.distance <=0.1))
            {
                return true;
            }
            break;
        case 4:
            if ((own_goal.distance > 17.6 || own_goal.distance <=0.1) &&
                (field.flag_center_bottom_distance > 54 || field.flag_center_bottom_distance <=0.1) &&
                (opponent_goal.distance > 61.5 || opponent_goal.distance <=0.1))
            {
                return true;
            }
            break;
        case 5:
            if ((own_goal.distance > 17.6 || own_goal.distance <=0.1) &&
                (field.flag_center_top_distance > 54 || field.flag_center_top_distance <=0.1) &&
                (opponent_goal.distance > 61.5 || opponent_goal.distance <=0.1))
            {
                return true;
            }
            break;
        case 6:
            if ((own_goal.distance > 8 || own_goal.distance <=0.1) &&
                (field.flag_left_top_distance > 25 || field.flag_left_top_distance <=0.1) &&
                (field.flag_center_top_distance > 41.5 || field.flag_center_top_distance <=0.1) &&
                (opponent_goal.distance > 68.5 || opponent_goal.distance <=0.1))
            {
                return true;
            }
            break;
        case 7:
            // if ((own_goal.distance > 8 || own_goal.distance <=0.1) &&
            //     (field.flag_left_bottom_distance > 25 || field.flag_left_bottom_distance <=0.1) &&
            //     (field.flag_center_bottom_distance > 20 || field.flag_center_bottom_distance <=0.1) &&
            //     (opponent_goal.distance > 60 || opponent_goal.distance <=0.1))
            //{
                return true;
            //}
            break;
        case 8:
            if ((own_goal.distance > 35.5 || own_goal.distance <=0.1) &&
                (field.flag_center_top_distance > 32 || field.flag_center_top_distance <=0.1) &&
                (field.flag_right_top_distance > 25 || field.flag_right_top_distance <=0.1))
            {
                return true;
            }
            break;
        case 9:
            // if ((own_goal.distance > 35.5 || own_goal.distance <=0.1) &&
            //     (field.flag_center_bottom_distance > 32 || field.flag_center_bottom_distance <=0.1) &&
            //     (field.flag_right_bottom_distance > 25 || field.flag_right_bottom_distance <=0.1))
            //{
                return true;
            //}
            break;
        case 10:
            // if ((own_goal.distance > 14 || own_goal.distance <=0.1) &&
            //     (field.flag_left_top_distance > 35.5 || field.flag_left_top_distance <=0.1) &&
            //     (field.flag_left_bottom_distance > 35.5 || field.flag_left_bottom_distance <=0.1))
            //{
                return true;
            //}
            break;
        case 11:
            //if ((own_goal.distance > 35.5 || own_goal.distance <=0.1))
            //{
                return true;
            //}
            break;
        }
    }
    return false;
}