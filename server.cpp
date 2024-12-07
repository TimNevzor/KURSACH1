// server.cpp
#include "server.h"
#include "calc.h"
#include "error.h"
#include "log.h"
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include <cstdint>
#include <stdint.h>
#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>

#define BUFFER_SIZE 1024

std::string server::sha256(const std::string& input)
{
    CryptoPP::SHA256 hash;
    std::string digest;

    CryptoPP::StringSource ss(input, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
    return digest;
}

void server::threadclient(int client_socket, std::map<std::string, std::string> database, logtxt* logger)
{
    try {
        char buffer[BUFFER_SIZE];
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            throw criticalerr("Ошибка получения данных от клиента");
        }

        buffer[bytes_received] = '\0';
        std::string message(buffer);

        // Извлечение логина, соли и хэша
        if (message.length() < 80) {
            throw criticalerr("Неверная длина сообщения");
        }

        std::string hash = message.substr(message.length() - 64);
        std::string salt = message.substr(message.length() - 80, 16);
        std::string login = message.substr(0, message.length() - 80);

        // Проверка логина в базе данных
        auto it = database.find(login);
        if (it == database.end()) {
            send(client_socket, "ERR", 3, 0);
            throw noncriticalerr("Неизвестный логин: " + login);
        }

        std::string password = it->second;
        std::string hashed_input = sha256(salt + password);

        if (hashed_input != hash) {
            send(client_socket, "ERR", 3, 0);
            throw noncriticalerr("Ошибка авторизации для логина: " + login);
        } else {
            send(client_socket, "OK", 2, 0);

            // Обработка векторов
            uint32_t N;
            if (recv(client_socket, &N, sizeof(N), 0) <= 0) {
                throw criticalerr("Ошибка получения количества векторов");
            }
            N = ntohl(N); // Преобразование из сети в хост

            calc calculator;

            for (uint32_t i = 0; i < sizeof(N); ++i) {
                uint32_t S;
                if (recv(client_socket, &S, sizeof(S), 0) <= 0) {
                    throw criticalerr("Ошибка получения длины вектора");
                }
                S = ntohl(S); // Преобразование из сети в хост

                std::vector<float> vec(S);
                if (recv(client_socket, vec.data(), S * sizeof(float), 0) <= 0) {
                    throw criticalerr("Ошибка получения вектора");
                }

                float result = calculator.countoper(vec);
                result = htonl(result); // Преобразование в сеть перед отправкой
                if (send(client_socket, &result, sizeof(result), 0) <= 0) {
                    throw criticalerr("Ошибка отправки результата");
                }
            }
        }

        close(client_socket);
    } catch (const noncriticalerr& e) {
        logger->writeerr("NONCRIT ERROR - " + std::string(e.what()));
        std::cerr << "NONCRIT ERROR - " << e.what() << std::endl;
        close(client_socket);
        return;
    } catch (const criticalerr& e) {
        logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        close(client_socket);
        return;
    }
}

int server::connection(int port, std::map<std::string, std::string> database, logtxt* logger)
{
    try {
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);

        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            throw criticalerr("Ошибка создания сокета");
        }

        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(server_socket);
            throw criticalerr("Ошибка привязки сокета");
        }

        if (listen(server_socket, 5) < 0) {
            close(server_socket);
            throw criticalerr("Ошибка прослушивания");
        }

        std::cout << "Сервер запущен на 127.0.0.1:" << port << "..." << std::endl;

        while (true) {
            int client_socket = accept(server_socket, nullptr, nullptr);
            if (client_socket < 0) {

                logger->writeerr("NONCRIT ERROR - Ошибка подключения клиента");
                continue;
            }

            std::thread client_thread(&server::threadclient, this, client_socket, database, logger);
            client_thread.detach();
        }

        close(server_socket);

    } catch (const noncriticalerr& e) {
        logger->writeerr("NONCRIT ERROR - " + std::string(e.what()));
        std::cerr << "NONCRIT ERROR - " << e.what() << std::endl;
        return -1;
    } catch (const criticalerr& e) {
        logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
