// server.h
#pragma once
#include <string>
#include <map>
#include <mutex>
#include "log.h"

class server
{
public:
    int connection(int port, std::map<std::string, std::string> base, logtxt* logger);
    void threadclient(int client_socket, std::map<std::string, std::string> base, logtxt* logger);
    std::string sha256(const std::string& message);
private:
	std::mutex cout_mutex;
};
