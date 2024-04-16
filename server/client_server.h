#pragma once

#include <iostream>
#include <cstring> // strerror
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "phone_book.h"

#define BUFSIZE 32000

class Server
{
    int _socket;                    // socket's descriptor of server
    sockaddr_in _address;           // socket address of server
    int _port;                      // port for listening
    int _connections;               // max number of allowed connections
    bool _is_running = false;       // 'is server running' flag
    int* _sockets_array = nullptr;  // array that holds descriptors of clients
    PhoneBook _phone_book;          // Phone book

    // Waits for connections using 'select'
    void _HandleAcceptions();
    // Adds all descriptors to read_fd_set
    void _FdSetRead(fd_set* read_fd_set);
    // Change first descriptor with value -1 to value of new_socket
    void _SetSocket(int new_socket);
    // Calls 'accept' for new descriptor
    void _TryAcceptConnection();
    // Calls 'recv' on given descriptor
    void _TryRecvFrom(int &socket);
    // Reads command and calls corresponding function of Phone Book, then sends response
    void _ProceedQuery(int socket, std::string query);
public:
    Server(int port, int connections);
    // Opens connection
    void Start();
    // Closes connection
    void Stop();
    // Clears _sockets_array
    ~Server();
};