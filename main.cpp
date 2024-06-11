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

// Define Player struct

struct Player
{
    std::string team_name;
    std::string playmode;
    std::string side;
    int unum;
    double x;
    double y;
};


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




// Move command function
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient)
{
    struct Posicion
    {
        int x;
        int y;
    };

    vector<Posicion>
        posiciones = {{50, 0},
                      {35, -20},
                      {35, 20},
                      {20, -25},
                      {18, -9},
                      {18, 5},
                      {20, 20},
                      {2, -18},
                      {28, -18},
                      {35, 11},
                      {5, 0}};
  
    Posicion myPos = posiciones[player.unum - 1];

    auto moveCommand = "(move " + to_string(myPos.x) + " " + to_string(myPos.y) + ")";
    udp_socket.sendTo(moveCommand, recipient);
}


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
    cout << "(init " + team_name + "(version 19))";

    udp_socket.sendTo("(init " + team_name + "(version 19))", other_recipient_udp);
    cout << "Init Message sent" << endl;

    std::size_t message_max_size = 1000;
    cout << "Waiting for a message" << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    // update upd port to provided by the other udp
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    Player player{team_name, "", "", 0, 0, 0};
    // parse the initial message
    player = parseInitialMessage(received_message_content, player);
    
    cout << player.unum << endl;
    //sendInitialMoveMessage(player, udp_socket, server_udp);

return 0;
}