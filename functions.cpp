#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <MinimalSocket/udp/UdpSocket.h>
#include <thread> 
using namespace std;

// Include headers
#include "functions.h"
#include "estructuras.h"

// Functions

// Move command function
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient)
{
    struct Posicion
    {
        int x;
        int y;
    };

    vector<Posicion>
        posiciones = {{-50, 0},
                      {-40, -10},
                      {-35, -28},
                      {-40, 10},
                      {-35, 28},
                      {-25, 11},
                      {-8, 20},
                      {-25, -11},
                      {-5, 0},
                      {-15, 0},
                      {-8, -20}};

    Posicion myPos = posiciones[player.unum - 1];

    auto moveCommand = "(move " + to_string(myPos.x) + " " + to_string(myPos.y) + ")";
    udp_socket.sendTo(moveCommand, recipient);
    cout << "Move command sent" << "Posicion: " << moveCommand << endl;
}

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
    player.flags_seen = 0;

    player.see_ball = false;
    player.seeing_zone = false;

    // All the flags are not seen at the beginning
    field.flag_center = {999, 999};
    field.flag_center_top = {999, 999};
    field.flag_center_bottom = {999, 999};
    field.flag_left_top = {999, 999};
    field.flag_left_bottom = {999, 999};
    field.flag_right_top = {999, 999};
    field.flag_right_bottom = {999, 999};
    field.flag_penalty_left_top = {999, 999};
    field.flag_penalty_left_center = {999, 999};
    field.flag_penalty_left_bottom = {999, 999};
    field.flag_penalty_right_top = {999, 999};
    field.flag_penalty_right_center = {999, 999};
    field.flag_penalty_right_bottom = {999, 999};
    field.flag_goal_left_top = {999, 999};
    field.flag_goal_left_bottom = {999, 999};
    field.flag_goal_right_top = {999, 999};
    field.flag_goal_right_bottom = {999, 999};

    if (own_goal.side == "l")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }
    if (own_goal.side == "r")
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
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
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
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        // Search for the flags
        // Search for the center flag
        if (see_message[i].find("(f c)") != string::npos)
        {
            player.flags_seen++;
            vector<string> center_coords = separate_string_separator(see_message[i], " ");
            field.flag_center = {stof(center_coords[2]), stof(center_coords[3])};
            cout << "Center flag coordinates: " << field.flag_center[0] << " " << field.flag_center[1] << endl;
        }
        if (player.zone_name == "(f c)")
        {
            player.seeing_zone = true;
        }

        // Search for the center top flag
        if (see_message[i].find("(f c t)") != string::npos)
        {
            vector<string> center_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_top = {stof(center_top_coords[3]), stof(center_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f c t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the center bottom flag
        if (see_message[i].find("(f c b)") != string::npos)
        {
            vector<string> center_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_bottom = {stof(center_bottom_coords[3]), stof(center_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f c b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the left top flag
        if (see_message[i].find("(f l t)") != string::npos)
        {
            vector<string> left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_top = {stof(left_top_coords[3]), stof(left_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the left bottom flag
        if (see_message[i].find("(f l b)") != string::npos)
        {
            vector<string> left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_bottom = {stof(left_bottom_coords[3]), stof(left_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the right top flag
        if (see_message[i].find("(f r t)") != string::npos)
        {
            vector<string> right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_top = {stof(right_top_coords[3]), stof(right_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the right bottom flag
        if (see_message[i].find("(f r b)") != string::npos)
        {
            vector<string> right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_bottom = {stof(right_bottom_coords[3]), stof(right_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name == "(f r b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left top flag
        if (see_message[i].find("(f p l t)") != string::npos)
        {
            vector<string> penalty_left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_top = {stof(penalty_left_top_coords[4]), stof(penalty_left_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left center flag
        if (see_message[i].find("(f p l c)") != string::npos)
        {
            vector<string> penalty_left_center_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_center = {stof(penalty_left_center_coords[4]), stof(penalty_left_center_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p l c)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left bottom flag
        if (see_message[i].find("(f p l b)") != string::npos)
        {
            vector<string> penalty_left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_bottom = {stof(penalty_left_bottom_coords[4]), stof(penalty_left_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right top flag
        if (see_message[i].find("(f p r t)") != string::npos)
        {
            vector<string> penalty_right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_top = {stof(penalty_right_top_coords[4]), stof(penalty_right_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right center flag
        if (see_message[i].find("(f p r c)") != string::npos)
        {
            vector<string> penalty_right_center_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_center = {stof(penalty_right_center_coords[4]), stof(penalty_right_center_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p r c)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right bottom flag
        if (see_message[i].find("(f p r b)") != string::npos)
        {
            vector<string> penalty_right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_bottom = {stof(penalty_right_bottom_coords[4]), stof(penalty_right_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f p r b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal left top flag
        if (see_message[i].find("(f g l t)") != string::npos)
        {
            vector<string> goal_left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_left_top = {stof(goal_left_top_coords[4]), stof(goal_left_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal left bottom flag
        if (see_message[i].find("(f g l b)") != string::npos)
        {
            vector<string> goal_left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_left_bottom = {stof(goal_left_bottom_coords[4]), stof(goal_left_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal right top flag
        if (see_message[i].find("(f g r t)") != string::npos)
        {
            vector<string> goal_right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_right_top = {stof(goal_right_top_coords[4]), stof(goal_right_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal right bottom flag
        if (see_message[i].find("(f g r b)") != string::npos)
        {
            vector<string> goal_right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_right_bottom = {stof(goal_right_bottom_coords[4]), stof(goal_right_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name == "(f g r b)")
            {
                player.seeing_zone = true;
            }
        }
    }
}

// bool estasentusitio(const Field &field, const Player &player, const Goal &own_goal, const Goal &opponent_goal)
// {
//     if (player.side == "l")
//     {
//         switch (player.unum)
//         {
//         case 1:
//             if ((field.flag_left_top_distance > 27 || field.flag_left_top_distance <= 0.1) &&
//                 (field.flag_left_bottom_distance > 27 || field.flag_left_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 90 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }

//             break;
//         case 2:
//             if ((own_goal.distance > 7 || own_goal.distance <= 0.1) &&
//                 (field.flag_left_top_distance > 33 || field.flag_left_top_distance <= 0.1) &&
//                 (field.flag_left_bottom_distance > 14.5 || field.flag_left_bottom_distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 42.5 || field.flag_center_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 77.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 3:
//             if ((own_goal.distance > 7 || own_goal.distance <= 0.1) &&
//                 (field.flag_left_bottom_distance > 33 || field.flag_left_bottom_distance <= 0.1) &&
//                 (field.flag_left_top_distance > 14.5 || field.flag_left_top_distance <= 0.1) &&
//                 (field.flag_center_top_distance > 30 || field.flag_center_top_distance <= 0.1) &&
//                 (opponent_goal.distance > 77.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 4:
//             if ((own_goal.distance > 17.6 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 54 || field.flag_center_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 61.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 5:
//             if ((own_goal.distance > 17.6 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_top_distance > 54 || field.flag_center_top_distance <= 0.1) &&
//                 (opponent_goal.distance > 61.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 6:
//             if ((own_goal.distance > 8 || own_goal.distance <= 0.1) &&
//                 (field.flag_left_top_distance > 25 || field.flag_left_top_distance <= 0.1) &&
//                 (field.flag_center_top_distance > 41.5 || field.flag_center_top_distance <= 0.1) &&
//                 (opponent_goal.distance > 68.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 7:
//             if ((own_goal.distance > 8 || own_goal.distance <= 0.1) &&
//                 (field.flag_left_bottom_distance > 25 || field.flag_left_bottom_distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 20 || field.flag_center_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 60 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 8:
//             if ((own_goal.distance > 35.5 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_top_distance > 32 || field.flag_center_top_distance <= 0.1) &&
//                 (field.flag_right_top_distance > 25 || field.flag_right_top_distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 9:
//             if ((own_goal.distance > 35.5 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 32 || field.flag_center_bottom_distance <= 0.1) &&
//                 (field.flag_right_bottom_distance > 25 || field.flag_right_bottom_distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 10:
//             if ((own_goal.distance > 14 || own_goal.distance <= 0.1) &&
//                 (field.flag_left_top_distance > 35.5 || field.flag_left_top_distance <= 0.1) &&
//                 (field.flag_left_bottom_distance > 35.5 || field.flag_left_bottom_distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 11:
//             if ((own_goal.distance > 35.5 || own_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         }
//     }
//     else
//     {
//         switch (player.unum)
//         {
//         case 1:
//             if ((field.flag_right_top_distance > 27 || field.flag_right_top_distance <= 0.1) &&
//                 (field.flag_right_bottom_distance > 27 || field.flag_right_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 90 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 2:
//             if ((own_goal.distance > 7 || own_goal.distance <= 0.1) &&
//                 (field.flag_right_top_distance > 33 || field.flag_right_top_distance <= 0.1) &&
//                 (field.flag_right_bottom_distance > 14.5 || field.flag_right_bottom_distance <= 0.1) &&
//                 (field.flag_center_top_distance > 42.5 || field.flag_center_top_distance <= 0.1) &&
//                 (opponent_goal.distance > 77.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 3:
//             if ((own_goal.distance > 7 || own_goal.distance <= 0.1) &&
//                 (field.flag_right_bottom_distance > 20 || field.flag_right_bottom_distance <= 0.1) &&
//                 (field.flag_right_top_distance > 14.5 || field.flag_right_top_distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 30 || field.flag_center_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 77.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 4:
//             if ((own_goal.distance > 17.6 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_top_distance > 54 || field.flag_center_top_distance <= 0.1) &&
//                 (opponent_goal.distance > 61.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 5:
//             if ((own_goal.distance > 17.6 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 54 || field.flag_center_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 61.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 6:
//             if ((own_goal.distance > 8 || own_goal.distance <= 0.1) &&
//                 (field.flag_right_top_distance > 25 || field.flag_right_top_distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 41.5 || field.flag_center_bottom_distance <= 0.1) &&
//                 (opponent_goal.distance > 68.5 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 7:
//             if ((own_goal.distance > 8 || own_goal.distance <= 0.1) &&
//                 (field.flag_right_bottom_distance > 25 || field.flag_right_bottom_distance <= 0.1) &&
//                 (field.flag_center_top_distance > 20 || field.flag_center_top_distance <= 0.1) &&
//                 (opponent_goal.distance > 60 || opponent_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 8:
//             if ((own_goal.distance > 35.5 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 32 || field.flag_center_bottom_distance <= 0.1) &&
//                 (field.flag_left_top_distance > 25 || field.flag_left_top_distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 9:
//             if ((own_goal.distance > 35.5 || own_goal.distance <= 0.1) &&
//                 (field.flag_center_bottom_distance > 32 || field.flag_center_bottom_distance <= 0.1) &&
//                 (field.flag_left_bottom_distance > 25 || field.flag_left_bottom_distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 10:
//             if ((own_goal.distance > 14 || own_goal.distance <= 0.1) &&
//                 (field.flag_right_top_distance > 35.5 || field.flag_right_top_distance <= 0.1) &&
//                 (field.flag_right_bottom_distance > 35.5 || field.flag_right_bottom_distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         case 11:
//             if ((own_goal.distance > 35.5 || own_goal.distance <= 0.1))
//             {
//                 return true;
//             }
//             break;
//         }
//     }
//     return false;
// }

string dash(double power, double angle)
{
    std::string dash_command = "(dash " + to_string(power) + " " + to_string(angle) + ")";
    return dash_command;
}

void store_data_hear(string &hear_message, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp, Ball &ball)
{
    vector<string> aux_hear_message = separate_string(hear_message); // hear 0 referee kick_off_l
    vector<string> valores_mensaje_Hear;
    for (size_t i = 0; i < aux_hear_message.size(); i++)
    {
        if (aux_hear_message[i].find("hear") != string::npos)
        {
            valores_mensaje_Hear = separate_string_separator(aux_hear_message[i], " ");
            cout << "TIME: " << valores_mensaje_Hear[1] << endl;
            cout << "REFEREE: " << valores_mensaje_Hear[2] << endl;
            cout << "MODO: " << valores_mensaje_Hear[3] << endl;
            player.playmode_prev = player.playmode; //metemos en player el modo de juego anterior
             std::cout << "MODO ANTERIRO: " << player.playmode_prev << std::endl; 
            player.playmode = valores_mensaje_Hear[3]; //metemos en player el modo de juego
             std::cout << "MODO ACTUAL: " << player.playmode << std::endl; 

         if (valores_mensaje_Hear[3].find("goal") != string::npos)
        {
            // Paso 1: Encontrar la última posición del '_'
            size_t lastUnderscorePos_L = valores_mensaje_Hear[3].rfind('_');
             
            std::string numberStr_L = valores_mensaje_Hear[3].substr(lastUnderscorePos_L + 1); 
            // std::string numberStr_R = valores_mensaje_Hear[3].substr(lastUnderscorePos_R + 1); 
            player.jugadorMarcaGol = numberStr_L; 
            //player.jugadorMarcaGol = numberStr_R; 
            std::cout << "Número extraído: " << player.jugadorMarcaGol << std::endl; 
        }
           
            //funcion_modos_juego(player.playmode, player, udp_socket, server_udp, ball);
        }
    }
}

void chutarPorteria(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    if (ball.distance < 1)
    {
        float angle = opponent_goal.angle; // Ángulo hacia la portería contraria
        int power = 100;                   // Potencia del chute
        std::string kick_command = "(kick " + to_string(power) + " " + to_string(angle) + ")";
        udp_socket.sendTo(kick_command, server_udp); // Enviar comando de chute}
    }
    else
    {
        int i = 0;
        if (abs(ball.angle) >= 10)
        {
            int division = 1;
            if (ball.distance < 6)
            {
                division = 50;
            }
            else
            {
                division = 5;
            }
            // Rotate the player to the ball
            std::string rotate_command = "(turn " + to_string(ball.angle / division) + ")";
            udp_socket.sendTo(rotate_command, server_udp);
        }

        else
        {
            int power = 100;
            if (ball.distance < 3)
            {
                power = 60;
            }
            else if (ball.distance < 7)
            {
                power = 80;
            }
            // In this moment, the player should be looking to the ball
            // Create the dash command
            std::string dash_command = "(dash " + to_string(power) + " 0)";
            udp_socket.sendTo(dash_command, server_udp);
        }
    }
}

void pase(Player &player, Ball &ball,JugadorCercano &jugador, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    if (ball.distance < 1)
    {
        float angle = jugador.angle; // Ángulo hacia la portería contraria
        int power = 50;                   // Potencia del chute
        std::string kick_command = "(kick " + to_string(power) + " " + to_string(angle) + ")";
        udp_socket.sendTo(kick_command, server_udp); // Enviar comando de chute}
    }
    else
    {
        int i = 0;
        if (abs(ball.angle) >= 10)
        {
            int division = 1;
            if (ball.distance < 6)
            {
                division = 50;
            }
            else
            {
                division = 5;
            }
            // Rotate the player to the ball
            std::string rotate_command = "(turn " + to_string(ball.angle / division) + ")";
            udp_socket.sendTo(rotate_command, server_udp);
        }

        else
        {
            int power = 100;
            if (ball.distance < 3)
            {
                power = 60;
            }
            else if (ball.distance < 7)
            {
                power = 80;
            }
            // In this moment, the player should be looking to the ball
            // Create the dash command
            std::string dash_command = "(dash " + to_string(power) + " 0)";
            udp_socket.sendTo(dash_command, server_udp);
        }
    }
}


void configurePlayer(Player &player) // la  mitad de los jugadores de la derecha no estan en zona al iniciar
{
    vector<Posicion>
        posiciones = {{-50, 0},
                      {-40, -10},
                      {-35, -28},
                      {-40, 10},
                      {-35, 28},
                      {-25, 11},
                      {-8, 20},
                      {-25, -11},
                      {-5, 0},
                      {-15, 0},
                      {-8, -20}};

    const std::vector<Posicion> flags_config =
        {
            {0, 0},         // Center of the field
            {0, -33.5},     // Top center
            {0, 33.5},      // Bottom center
            {-52.5, -33.5}, // Corner top-left
            {-52.5, 33.5},  // Corner bottom-left
            {52.5, -33.5},  // Corner top-right
            {52.5, 33.5},   // Corner bottom-right
            {-36, -20},     // Penalty top-left 7
            {-36, 0},       // Penalty center-left
            {-36, 20},      // Penalty bottom-left 9
            {36, -20},      // Penalty top-right
            {36, 0},        // Penalty center-right
            {36, 20},       // Penalty bottom-right
            {-52.5, -7.32}, // Goal top-left
            {-52.5, 7.32},  // Goal bottom-left
            {52.5, -7.32},  // Goal top-right 15
            {52.5, 7.32}    // Goal bottom-right 16
        };
    if (player.unum == 1)
    {
        if (player.side == "r")
        {
            player.rol = "PORTERO";
            player.range = 10;
            player.zone = {50, 0};
            player.zone_name = "g r"; 
        }
        else
        {
            player.rol = "PORTERO";
            player.range = 10;
            player.zone = posiciones[player.unum - 1];
            player.zone_name = "g l"; 
        }
    }
    else if (player.unum == 3)
    {
        if (player.side == "r")
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {50, 20};
            player.zone_name = "(f " + player.side + " b)";
        }
        else
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {-50,-20};
            player.zone_name = "(f " + player.side + " t)";
        }
    }
    else if (player.unum == 5)
    {
        if (player.side == "r")
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {50, -20};
            player.zone_name = "(f " + player.side + " t)";
        }
        else
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {-30,30};
            player.zone_name = "(f " + player.side + " b)";
        }
    }
    else if (player.unum == 2)
    {
        if (player.side == "r")
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {50, 10};
            player.zone_name = "(f p" + player.side + " b)";
        }
        else
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {-50,-10};
            player.zone_name = "(f p " + player.side + " t)";
        }
    }
    else if (player.unum == 4)
    {
        if (player.side == "r")
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {50, -10};
            player.zone_name = "(f p " + player.side + " t)";
        }
        else
        {
            player.rol = "DEFENSA";
            player.range = 20;
            player.zone = {-35,20};
            player.zone_name = "(f p " + player.side + " b)";
        }
    }
    else if (player.unum == 8)
    {
        if (player.side == "r")
        {
            player.rol = "DEFENSA";
            player.range = 30;
            player.zone = {15, 25};
            player.zone_name = "(f p " + player.side + " b)";
        }
        else
        {
            player.rol = "DEFENSA";
            player.range = 30;
            player.zone = {-30, -20};
            player.zone_name = "(f p " + player.side + " t)";
        }
    }
    else if (player.unum == 6)
    {
        if (player.side == "r")
        {
            player.rol = "DEFENSA";
            player.range = 30;
            player.zone = {30, -10};
            player.zone_name = "(f p " + player.side + " t)";
        }
        else
        {
            player.rol = "DEFENSA";
            player.range = 30;
            player.zone = {-15,20};
            player.zone_name = "(f p " + player.side + " b)";
        }
    }
    else if (player.unum == 9)
    {
        if (player.side == "r")
        {
            player.rol = "DELANTERO";
            player.range = 20;
            player.zone = {10, 0};
            player.zone_name = "(f p r c)";
        }
        else
        {
            player.rol = "DELANTERO";
            player.range = 20;
            player.zone = {-10, 0};
            player.zone_name = "(f p l c)";
        }
    }
    else if (player.unum == 10)
    {
        if (player.side == "r")
        {
            player.rol = "DELANTERO";
            player.range = 15;
            player.zone = {20, 0};
            player.zone_name = "(f c)";
        }
        else
        {
            player.rol = "DELANTERO";
            player.range = 15;
            player.zone = posiciones[player.unum - 1];
            player.zone_name = "(f c)";
        }
    }
    else if (player.unum == 7)
    {
        if (player.side == "r")
        {
            player.rol = "DELANTERO";
            player.range = 30;
            player.zone = {10, -20};
            player.zone_name = "(f c t)";
        }
        else
        {
            player.rol = "DELANTERO";
            player.range = 30;
            player.zone = {-15,20};
            player.zone_name = "(f c b)";
        }
    }
    else if (player.unum == 11)
    {
        if (player.side == "r")
        {
            player.rol = "DELANTERO";
            player.range = 30;
            player.zone = {10, 25};
            player.zone_name = "(f c b)";
        }
        else
        {
            player.rol = "DELANTERO";
            player.range = 30;
            player.zone = {-10,-20};
            player.zone_name = "(f c t)";
        }
    }
    else
    {
        player.rol = "DELANTERO";
        player.range = 65;
        player.zone = flags_config[11];
        player.zone_name = "(f c)";
    }
}

string returnToZone(Player const &player)
{
    // cout << "Not in zone" << endl; esto es mentira
    if (player.seeing_zone == false)
    {
        std::string rotate_command = "(turn " + to_string(15) + ")";
        return rotate_command;
    }
    else
    {
        if (player.flags_seen <= 3)
        {
            std::string dash_command = "(turn 15)";
            return dash_command;
        }
        else
        {
            std::string dash_command = "(dash 100 0)";
            return dash_command;
        }
    }
    // el portero no sabe volver porque no tiene flag
}

void imInZone(Player &player)
{
    player.distancia_a_zona = sqrt(pow(player.x - player.zone.x, 2) + pow(player.y - player.zone.y, 2));
    if (player.distancia_a_zona<player.range&&player.flags_seen>=3)
    {
        player.in_zone = true;
    }
    else
    {
        player.in_zone = false;
    }
}

void store_data_senseBody(string &senseBody_message, Player &player) //(sense_body 313 (view_mode high normal) (stamina 7985 1 126795))
{
    vector<string> parse_message = separate_string(senseBody_message); // sense_body 313 (view_mode high normal) (stamina 7985 1 126795)
    vector<string> valores;
    if (parse_message[0].find("sense_body") != string::npos)
    {
        vector<string> senseBodu_message = separate_string(parse_message[0]);

        for (size_t i = 0; i < senseBodu_message.size(); i++)
        {
            if (senseBodu_message[1].find("stamina") != string::npos)
            {
                valores = separate_string_separator(senseBodu_message[1], " ");
                player.stamina = stof(valores[1]);
            }
        }
    }
}

void store_data_hear(string &hear_message, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    vector<string> aux_hear_message = separate_string(hear_message); // hear 0 referee kick_off_l
    vector<string> valores_mensaje_Hear;
    for (size_t i = 0; i < aux_hear_message.size(); i++)
    {
        if (aux_hear_message[i].find("hear") != string::npos)
        {
            valores_mensaje_Hear = separate_string_separator(aux_hear_message[i], " ");
            cout << "TIME: " << valores_mensaje_Hear[1] << endl;
            cout << "REFEREE: " << valores_mensaje_Hear[2] << endl;
            cout << "MODO: " << valores_mensaje_Hear[3] << endl;
            player.playmode_prev = player.playmode; //metemos en player el modo de juego anterior
             std::cout << "MODO ANTERIRO: " << player.playmode_prev << std::endl; 
            player.playmode = valores_mensaje_Hear[3]; //metemos en player el modo de juego
             std::cout << "MODO ACTUAL: " << player.playmode << std::endl; 

         if (valores_mensaje_Hear[3].find("goal") != string::npos)
        {
            // Paso 1: Encontrar la última posición del '_'
            size_t lastUnderscorePos_L = valores_mensaje_Hear[3].rfind('_');
             
            std::string numberStr_L = valores_mensaje_Hear[3].substr(lastUnderscorePos_L + 1); 
            // std::string numberStr_R = valores_mensaje_Hear[3].substr(lastUnderscorePos_R + 1); 
            player.jugadorMarcaGol = numberStr_L; 
            //player.jugadorMarcaGol = numberStr_R; 
            std::cout << "Número extraído: " << player.jugadorMarcaGol << std::endl; 
        }
           
            //funcion_modos_juego(player.playmode, player, udp_socket, server_udp, ball);
        }
    }
}
    

JugadorCercano procesarJugadoresVisibles(const vector<string> &see_message, const Player &player)
{
    JugadorCercano jugador_mas_cercano;
    float menor_distancia = std::numeric_limits<float>::max();

    for (auto &obj : see_message)
    {
        if (obj.find("(p") != string::npos && obj.find("(p)") == string::npos)
        {
            string obj_copy = obj; // Hacemos una copia local del string
            vector<string> player_info = separate_string_separator(obj_copy, " ");
            if (player_info.size() < 5) continue; // Asegurar que hay suficientes elementos

            JugadorCercano jugador;
            jugador.nombreEquipo = player_info[1];
            jugador.dorsal = player_info[2];
            jugador.distancia = player_info[3];
            jugador.angle = stof(player_info[4]); // Convertir el ángulo a float
            jugador.distance = stof(jugador.distancia); // Convertir la distancia a float

            // Eliminar el último carácter si es un paréntesis ')'
            if (!jugador.dorsal.empty() && jugador.dorsal.back() == ')')
            {
                jugador.dorsal.pop_back();

                // Comprobar si el jugador es del mismo equipo
                if (jugador.nombreEquipo.find(player.team_name) != string::npos)
                {
                    // Verificar si es el jugador más cercano
                    if (jugador.distance < menor_distancia)
                    {
                        menor_distancia = jugador.distance;
                        jugador_mas_cercano = jugador;
                    }
                }
            }
        }
    }

    return jugador_mas_cercano;
}

void mostrarJugadorMasCercano(const JugadorCercano &jugador_mas_cercano)
{
    if (jugador_mas_cercano.dorsal.empty()) {
        std::cout << "No se encontraron jugadores cercanos." << std::endl;
        return;
    }

    std::cout << " " << std::endl;
    std::cout << "Jugador más cercano:" << std::endl;
    std::cout << "Nombre Equipo: " << jugador_mas_cercano.nombreEquipo << std::endl;
    std::cout << "Dorsal: " << jugador_mas_cercano.dorsal << std::endl;
    std::cout << "Distancia: " << jugador_mas_cercano.distancia << std::endl;
    std::cout << "Ángulo: " << jugador_mas_cercano.angle << std::endl;
    std::cout << " " << std::endl;
}


void funcion_modos_juego(const string &modo, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp, Ball &ball)
{   
    if ((player.playmode == "goal_l_" + player.jugadorMarcaGol) || (player.playmode == "goal_r_" + player.jugadorMarcaGol) || player.playmode == "half_time") //movemos a los jugadores a su posicion inicial
    {   
        sendInitialMoveMessage(player, udp_socket, server_udp); 
        configurePlayer(player); 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if ((player.playmode == "corner_kick_l" && player.side == "l") || (player.playmode == "corner_kick_r"  && player.side == "r"))
    {
        if (player.unum==9){
            if (player.see_ball)
                {
                    int i = 0;
                    if (abs(ball.angle) >= 10)
                    {
                        int division = 1;
                        if (ball.distance < 6)
                        {
                            division = 20;
                        }
                        else
                        {
                            division = 5;
                        }
                        // Rotate the player to the ball
                        std::string rotate_command = "(turn " + to_string(ball.angle / division) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }

                    else
                    {
                        int power = 100;
                        if (ball.distance < 3)
                        {
                            power = 60;
                            std::string dash_command = "(dash " + to_string(power) + " 0)";
                            udp_socket.sendTo(dash_command, server_udp);
                        }
                        else if (ball.distance < 7)
                        {
                            power = 80;
                            std::string dash_command = "(dash " + to_string(power) + " 0)";
                            udp_socket.sendTo(dash_command, server_udp);
                        }
                        else if(ball.distance<=1){
                        power=10;
                        float angle=130;
                        std::string kick_command = "(kick " + to_string(power) + " " + to_string(angle) + ")";
                        udp_socket.sendTo(kick_command, server_udp); // Enviar comando de chute}

                        }


                    }
                }

                else

                {
                    //cout << "---------------rotating to find de ball-" << endl;
                    // Rotate to find the ball
                    if (player.y < 0)
                    {
                        std::string rotate_command = "(turn " + to_string(-80) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }
                    else
                    {
                        std::string rotate_command = "(turn " + to_string(80) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }
                }
        }
        else{
         udp_socket.sendTo(returnToZone(player), server_udp);
        }

    } 
}