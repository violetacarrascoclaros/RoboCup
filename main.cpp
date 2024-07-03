#include <iostream>

using namespace std;

#include <MinimalSocket/udp/UdpSocket.h>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <cmath>

// Include headers
#include "functions.h"
#include "estructuras.h"
#include "CosteVSRecompensa.h"


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
    Goal own_goal{"0", "0", "init", 0};
    Goal opponent_goal{"0", "0", "init", 0};
    Field field;

    // parse the initial message
    player = parseInitialMessage(received_message_content, player);

    cout << player.unum << endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);

    // Configure the goals
    if (player.side == "r")
    {
        opponent_goal.side = "l";
        own_goal.side = "r";
    }
    else
    {
        opponent_goal.side = "r";
        own_goal.side = "l";
    }
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        vector<string> parsed_message = separate_string(received_message_content);

        // Search for see message
        if (parsed_message[0].find("see") <= 5)
        {
            vector<string> see_message = separate_string(parsed_message[0]);
            store_data_see(see_message, player, ball, own_goal, opponent_goal, field);
            bool in_position = estasentusitio(field, player, own_goal, opponent_goal);
        // Logic of the player
           
        int mejorAccion = obtenerMejorAccion(player,ball,opponent_goal);
        switch (mejorAccion)
        {
            case 0:
                // Perform action 0
                break;
            case 1:
                // Perform action 1
                break;
            case 2:
                // Perform action 2
                break;
            // Add more cases for other actions
            default:
                // Perform default action
                break;
        }



        //end logic of the player
        }
    }
    return 0;
}