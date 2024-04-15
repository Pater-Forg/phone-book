#include "client_server.h"
#include <thread>
#include <chrono>
#include <sstream>

Server::Server(int port, int connections)
    : _port(port), _connections(connections), _phone_book("data.csv", "conf") {
        _sockets_array = new int[connections];
        for (int i = 0; i < _connections; i++) {
            _sockets_array[i] = -1;
        }   
    }

Server::~Server() {
    delete[] _sockets_array;
}

void Server::Start() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        std::cout << "[Error] Can't get server socket" << std::endl;
        return;
    }

    _address.sin_family = AF_INET;
    _address.sin_port = htons(_port);
    _address.sin_addr.s_addr = INADDR_ANY;

    bind(_socket, (struct sockaddr*)&_address, sizeof(_address));
    if (listen(_socket, _connections) == 0) {
        _is_running = true;
        std::cout << "[Info] Server is listening on port " << _port << "..." << std::endl;
        _sockets_array[0] = _socket;
        _HandleAcceptions();
    }
    else {
        std::cout << "[Error] Can't start listening" << std::endl;
    }    
}

void Server::_HandleAcceptions() {
    fd_set read_fd_set;
    int incoming_connections;
    while (_is_running) {
        FD_ZERO(&read_fd_set);
        _FdSetRead(&read_fd_set);

        incoming_connections = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
        if (incoming_connections >= 0) {
            if (FD_ISSET(_socket, &read_fd_set)) {
                _TryAcceptConnection();
                incoming_connections--;
                if (!incoming_connections) continue;
            }

            for (int i = 1; i < _connections; i++) {
                if (_sockets_array[i] > 0 && FD_ISSET(_sockets_array[i], &read_fd_set)) {
                    _TryRecvFrom(_sockets_array[i]);
                }
            }
        }
    }
}

void Server::_FdSetRead(fd_set* read_fd_set) {
    for (int i = 0; i < _connections; i++) {
        if (_sockets_array[i] >= 0) {
            FD_SET(_sockets_array[i], read_fd_set);
        }
    }
}

void Server::_TryAcceptConnection() {
    sockaddr_in new_address;
    socklen_t addrlen = sizeof(sockaddr_in);
    int new_socket = accept(_socket, (struct sockaddr*)&new_address, &addrlen);
    if (new_socket >= 0) {
        std::cout << "[Info] accepted connection with sd: " << new_socket << std::endl;
        _SetSocket(new_socket);
    }
    else {
        std::cout << "[Error] acception failed: " << strerror(errno) << std::endl;
    }
}

void Server::_TryRecvFrom(int &socket) {
    char buffer[1024] = {0};
    int recv_res = recv(socket, buffer, 1024, 0);
    if (recv_res > 0) {
        std::cout << "[Recv:" << socket << "] " << buffer << std::endl;
        std::string query(buffer);
        std::thread([this, socket, query](){
            this->_ProceedQuery(socket, query);
        }).detach();
    }
    else if (recv_res == 0) {
        std::cout << "[Info] Close connection for: " << socket << std::endl;
        close(socket);
        socket = -1;
    }
    else if (recv_res < 0) {
        std::cout << "[Error] Failed to recive message from: " << socket << std::endl;
    }
}

void Server::_SetSocket(int new_socket) {
    for (int i = 0; i < _connections; i++) {
        if (_sockets_array[i] < 0) {
            _sockets_array[i] = new_socket;
            break;
        }
    }
}

void Server::Stop() {
    _is_running = false;
    for (int i = 0; i < _connections; i++) {
        close(_sockets_array[i]);
    }
    std::cout << "[Info] Server stopped" << std::endl;
}

void Server::_ProceedQuery(int socket, std::string query) {
    std::stringstream s_stream(query.c_str());
    std::string command;
    std::string response;
    char delim = _phone_book.GetDelim();
    std::getline(s_stream, command, ' ');
    std::cout << "[Cmd] '" << command << "'" << std::endl;
    if (command == "add") {
        Entry new_entry;
        std::getline(s_stream, new_entry.FirstName, delim);
        std::getline(s_stream, new_entry.MiddleName, delim);
        std::getline(s_stream, new_entry.LastName, delim);
        std::getline(s_stream, new_entry.PhoneNumber, delim);
        std::getline(s_stream, new_entry.Note, delim);
        _phone_book.Add(new_entry);
        response = "Success operation: Add";
    }
    else if (command == "delete") {
        int id;
        s_stream >> id;
        if (_phone_book.Remove(id)) {
            response = "Success operation: Delete";
        }
        else {
            response = "Failed operation: Delete (id not found)";
        }
    }
    else if (command == "find") {
        std::string value;
        s_stream >> value;
        Entry entry = _phone_book.Find(value);
        if (entry.Id > 0) {
            response = entry.ToString(delim);
        }
        else {
            response = "Not found";
        }
    }
    _phone_book.SaveData();
    send(socket, response.c_str(), response.length()+1, 0);
}