#include <stdio.h>
#include <iostream>
#include "tcpserver.hpp"

bool running = true;
http::TcpServer server;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            server.closeServer();
            running = false;
            exit(0);
            return TRUE;
        default:
            return FALSE;
    }
}

int main(int argc, char* argv[]) {
    SetConsoleCtrlHandler(CtrlHandler, TRUE);
    server.listenTCP();
    while (running) {
        int status = server.acceptTCP();

        if (status == 1) {
            running = false;
        }
    }
    server.closeServer();
    return 0;
}