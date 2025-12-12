#include "tcpserver.hpp"
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>

namespace http {
    // made with guidance from
    // https://beej.us/guide/bgnet/html/

    TcpServer::TcpServer() : m_socket(), m_wsaData() {
        m_wsaError = WSAStartup(MAKEWORD(2, 2), &m_wsaData);

        if (m_wsaError != 0) {
            std::cout << "An error occured: " << m_wsaData.szSystemStatus << "\n";
        }

        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (m_socket == INVALID_SOCKET) {
            std::cout << "An error occured when creating a socket: " << WSAGetLastError() << "\n";
            WSACleanup();
            exit(0);
        } else {
            std::cout << "TCP socket created.\n";
        }

        sockaddr_in service;
        // presentation to network
        // -1 error
        //  0 address invalid
        if (inet_pton(AF_INET, "127.0.0.1", &service.sin_addr) < 1) {
            std::cout << "An error occured when converting address: " << WSAGetLastError() << "\n";
            WSACleanup();
            exit(0);
        } else {
            std::cout << "TCP socket address set to: " << inet_ntoa(service.sin_addr) << "\n";
        }
        // host to network short
        service.sin_port = htons(8080);
        service.sin_family = AF_INET;
        memset(service.sin_zero, 0, sizeof(service.sin_zero)); // sin-zero is 8 bytes of padding
        // memset(service.sin_zero, 0, 8); would also work

        /*
            struct sockaddr {
                unsigned short    sa_family;    // address family, AF_xxx
                char              sa_data[14];  // 14 bytes of protocol address
            };
            sa_data contains a destination address and port number.
            This is rather unwieldy since you don't want to tediously pack the address
            in the sa_data by hand...
            To deal with struct sockaddr, programmers created a parallel structure: struct sockaddr_in.

            A pointer to a struct sockaddr_in can be cast to a pointer to a struct sockaddr and vice-versa
        */
        if (bind(m_socket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
            std::cout << "Failed to bind socket: " << WSAGetLastError() << "\n";
            closesocket(m_socket);
            WSACleanup();
            exit(0);
        } else {
            std::cout << "TCP socket bound.\n";
        }
    }

    TcpServer::~TcpServer() {
        
    }

    int TcpServer::listenTCP() {
        if (listen(m_socket, 5) == SOCKET_ERROR) {
            std::cout << "Error listening to socket: " << WSAGetLastError() << "\n";
        } else {
            std::cout << "Listening to TCP socket..." << "\n";
        }
        return 0;
    }

    int TcpServer::acceptTCP() {
        struct sockaddr_storage client_address;
        socklen_t addr_size = sizeof(client_address);
        
        SOCKET client_socket = accept(m_socket, (struct sockaddr*)&client_address, &addr_size);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << "\n";
            return -1;
        }

        if (this->sendTCP(client_socket)) {
            std::cout << "Sent message to client.\n";
        }
        return 0;
    }

    int TcpServer::sendTCP(SOCKET client) {
        char *msg = "<h1>Hello from TCP Server</h1>";
        int len, bytes_sentMSG, bytes_sentHEADER;
        len = strlen(msg);
        char response_header[256];
        snprintf(response_header, sizeof(response_header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n"
                 "\r\n", len);
        bytes_sentHEADER = send(client, response_header, strlen(response_header), 0);
        bytes_sentMSG = send(client, msg, len, 0);
        return 1;
    }

    void TcpServer::closeTCP() {
        closesocket(m_socket);
        WSACleanup();
        std::cout << "TCP socket closed.\n";
    }
}