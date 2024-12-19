//server.h
#pragma once
#include <string>
#include <map>
#include <mutex>
#include <tuple>
#include <sstream>
#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include "log.h"

class server
{
public:
    int connection(int port, std::map<std::string, std::string> base, logtxt* logger);
    std::tuple<bool, int, sockaddr_in> create_socket(int port, logtxt* logger);
    std::tuple<bool, int, sockaddr_in> bind_socket(int server_socket, sockaddr_in server_addr, logtxt* logger);
    std::tuple<bool, int, sockaddr_in> listen_socket(int port, int b, int server_socket, sockaddr_in server_addr, logtxt* logger);
    void threadclient(int client_socket, std::map<std::string, std::string> base, logtxt* logger);
    std::string sha256(const std::string& message);
private:
    std::mutex cout_mutex;
};
