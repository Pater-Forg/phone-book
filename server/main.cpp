#include <iostream>
#include <thread>
#include "client_server.h"

int main() {
    Server server(8080, 5);
    
    std::thread t([&server](){
        while (1) {
            std::string input;
            std::cin >> input;
            if (input == "close") {
                server.Stop();
                exit(0);
            }
        }
    });
    server.Start();
    return 0;
}