#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <errno.h>
#include <string>
#include "phone_book.h"

class Server
{
    int _socket;
    sockaddr_in _address;
    int _port;
    int _connections;
    bool _is_running = false;
    int* _sockets_array = nullptr;
    PhoneBook _phone_book;

    void _HandleAcceptions();
    void _FdSetRead(fd_set* read_fd_set);
    void _SetSocket(int new_socket);
    void _TryAcceptConnection();
    void _TryRecvFrom(int &socket);
    void _ProceedQuery(int socket, std::string query);
public:
    Server(int port, int connections);
    void Start();
    void Stop();
    ~Server();
};