#include <iostream>
#include "client_server.h"

int main() {
    Server server(8080, 5);
    server.Start();

    return 0;
}