#include "client_server.h"

Client::Client(int socket, sockaddr_in address)
    : _socket(socket), _address(address) {}

Client::~Client() {
    close(_socket);
    std::cout << "[Info] client disconnected (destructor)" << std::endl;
}

int Client::GetSocket() {
    return _socket;
}

sockaddr_in Client::GetAddress() {
    return _address;
}

void Client::Disconnect() {
    close(_socket);
    std::cout << "[Info] client disconnected (disconnect)" << std::endl;
}

Server::Server(int port, int connections)
    : _port(port), _connections(connections) {}

Server::~Server() {
    close(_socket);
    _client->Disconnect();
}

void Server::_handle_acceptions() {
    socklen_t addrlen = sizeof(sockaddr_in);
    while (_is_running){
        sockaddr_in client_address;
        int client_socket = accept(_socket, (struct sockaddr*)&client_address, &addrlen);
        if (client_socket == -1) {
            std::cout << "[Error] can't accept client" << std::endl;
            continue;
        }
        std::unique_ptr<Client> client(new Client(client_socket, client_address));
        std::cout << "[Info] client socket: " << client->GetSocket() << std::endl;
        _client = std::move(client);
        _handle_client(_client);
    }
}

void Server::_handle_client(std::unique_ptr<Client>& client) {
    char buffer[1024] = {0};
    while (client) {
        int result = read(client->GetSocket(), buffer, sizeof(buffer));
        if (result == 0) break;
        std::cout << "[Recv] " << buffer << std::endl;
    }
    client->Disconnect();
}

void Server::Start() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    _address.sin_family = AF_INET;
    _address.sin_port = htons(_port);
    _address.sin_addr.s_addr = INADDR_ANY;

    bind(_socket, (struct sockaddr *)&_address, sizeof(_address));
    if (listen(_socket, _connections) == 0) {
        _is_running = true;
        std::cout << "[Info] Server is listening on port " << _port << "..." << std::endl;
        _handle_acceptions();
    }
    else {
        std::cout << "[Error] Can't start listening" << std::endl;
    }    
}

void Server::Stop() {
    _is_running = false;
    close(_socket);
    _client->Disconnect();
    std::cout << "[Info] Server stopped" << std::endl;
}