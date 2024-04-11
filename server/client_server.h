#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>

class Client
{
    int _socket;
    sockaddr_in _address;
public:
    Client(int socket, sockaddr_in address);
    ~Client();
    int GetSocket();
    sockaddr_in GetAddress();
    void Disconnect();
};

class Server
{
    int _socket;
    sockaddr_in _address;
    int _port;
    int _connections;
    std::unique_ptr<Client> _client;
    bool _is_running = false;

    void _handle_client(std::unique_ptr<Client>& client);
    void _handle_acceptions();
public:
    Server(int port, int connections);
    ~Server();
    void Start();
    void Stop();
};