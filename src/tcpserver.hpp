#ifndef HTTP_TCPSERVER
#define HTTP_TCPSERVER
#include <winsock2.h>

namespace http {
    class TcpServer {
        public:
            TcpServer();
            ~TcpServer();
            int startTCP();
            int listenTCP();
            int acceptTCP();
            int sendTCP(SOCKET client);
            void closeTCP(SOCKET client);
            void closeServer();
        private:
            SOCKET m_socket;
            WSADATA m_wsaData;
            int m_wsaError;
    };
}

#endif