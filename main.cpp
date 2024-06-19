#include <iostream>
using namespace std;
#include <MinimalSocket/udp/UdpSocket.h>
//#include "stringutils.h"
//#include "types.h"
//#include "parsemessages.h"
//#include "tictoc.h"
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <cmath>
#include "functions.h"
#include "estructuras.h"


// main with two args
int main(int argc, char *argv[])
{
    // check if the number of arguments is correct
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <team-name> <this-port>" << endl;
        return 1;
    }

    // get the team name and the port
    string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);

    cout << "Creating a UDP socket" << endl;

    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);

    cout << "Socket created" << endl;

    bool success = udp_socket.open();

    if (!success)
    {
        cout << "Error opening socket" << endl;
        return 1;
    }

    MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};
    cout << "(init " + team_name + "(version 15))";

    udp_socket.sendTo("(init " + team_name + "(version 15))", other_recipient_udp);
    cout << "Init Message sent" << endl;

    std::size_t message_max_size = 1000000;
    cout << "Waiting for a message" << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    // update upd port to provided by the other udp
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    // Create objects
    Player player{team_name, "", "", false, 0, 0, 0};
    Ball ball{"0", "0", "0", "0"};
    
    // parse the initial message
    player = parseInitialMessage(received_message_content, player);
    
    cout << player.unum << endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);




    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        //cout << "Received message: " << received_message_content << endl;

        vector<string> parsed_message = separate_string(received_message_content);
        //cout << "Parsed message: " << parsed_message[0] << endl;

        // Search for see message
        if (parsed_message[0].find("see") <= 5)
        {
            //cout << "See message received" << endl;
            cout << "Parsed message: " << parsed_message[0] << endl;

            vector<string> see_message = separate_string(parsed_message[0]);


            // Find the "(b)" and save the position of the ball
            size_t ball_pos = 0;
            player.see_ball = false;
            for (size_t i = 0; i < see_message.size(); i++)
            {
                // Search for the ball
                if (see_message[i].find("(b)") <= 5)
                {
                    ball_pos = i;
                    player.see_ball = true;
                    break;
                }
            }
            size_t goal_pos = 0;
            player.see_goal = false;
            // for (size_t i = 0; i < see_message.size(); i++)
            // {
            //     // Search for the goal
            //     if (see_message[i].find("(g r)")<= 7)
            //     {
            //         goal_pos=i;
            //         player.see_goal=true;
            //         break;
            //         cout << "Goal right found" << endl;
                
            //     }

            // }

            if (player.see_ball)
            {
                vector<string> ball_coords = separate_string_separator(see_message[ball_pos], " ");
                ball.x = ball_coords[1];
                ball.y = ball_coords[2];

                cout << "Ball position: " << ball.x << " " << ball.y << endl;

                // Calculate the distance to the ball
                double distance = sqrt(pow(stod(ball.x), 2) + pow(stod(ball.y), 2));
                cout << "Distance to the ball: " << distance << endl;

                // Calculate the angle to the ball
                double angle = atan2(stod(ball.y), stod(ball.x));
                angle = angle * 180 / M_PI;
                cout << "Angle to the ball: " << angle << endl;

                if (distance < 1.5)
                {
                    // Kick the ball
                    int power = 100;
                    std::string kick_command = "(kick " + to_string(power) + " 0)";
                    udp_socket.sendTo(kick_command, server_udp);
                    cout << "Kick command sent: " << kick_command << endl << endl << endl << endl << endl << endl << endl;
                }
                else
                {
                    int i = 0;
                    if (abs(angle) >= 10)
                    {
                        int division = 1;
                        if (distance < 6)
                        {
                            division = 20;
                        }
                        else
                        {
                            division = 5;
                        }
                        // Rotate the player to the ball
                        std::string rotate_command = "(turn " + to_string(angle/division) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }

                    else
                    {
                        int power = 100;
                        if (distance < 3)
                        {
                            power = 60;
                        }
                        else if (distance < 7)
                        {
                            power = 80;
                        }
                        // In this moment, the player should be looking to the ball
                        // Create the dash command
                        std::string dash_command = "(dash " + to_string(power) + " 0)";
                        udp_socket.sendTo(dash_command, server_udp);
                        //cout << "Dash command sent: " << dash_command << endl;
                    }
                }

            }
            else
            {
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
 
    }

return 0;
}