// server.h
#pragma once
#include <string>
#include <map>
#include "log.h"

class server
{
public:
    int connection(int port, std::map<std::string, std::string> base, logtxt* sansara);
    void handle_client(int client_socket, std::map<std::string, std::string> base, logtxt* sansara);
    std::string sha256(const std::string& message);
};
