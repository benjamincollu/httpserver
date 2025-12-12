#include <stdio.h>
#include <iostream>
#include "tcpserver.hpp"

int main(int argc, char* argv[]) {
    http::TcpServer server = http::TcpServer();
    server.listenTCP();
    while (true) {
        server.acceptTCP();
    }
    return 0;
}