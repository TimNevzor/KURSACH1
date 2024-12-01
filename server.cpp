// server.cpp
#include "server.h"
#include "calc.h"
#include "error.h"   // Для criticalerr и noncriticalerr
#include "log.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <vector>
#include <exception> // Для std::exception

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Для хеширования
#include <sstream>
#include <cryptopp/base64.h>



#define BUFFER_SIZE 1024

std::string server::sha256(const std::string& input)
{
    CryptoPP::SHA256 hash;
    std::string digest;

    CryptoPP::StringSource ss(input, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
    return digest;
}

int server::connection(int port, std::map<std::string, std::string> database, logtxt* sansara)
{
    try {
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            throw criticalerr("Ошибка создания сокета");
        }

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Устанавливаем IP-адрес на 127.0.0.1
        server_addr.sin_port = htons(port);

        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(server_socket);
            throw criticalerr("Ошибка привязки сокета");
        }

        if (listen(server_socket, 5) < 0) {
            close(server_socket);
            throw criticalerr("Ошибка прослушивания");
        }

        std::cout << "Сервер запущен на 127.0.0.1:" << port << ", ожидает подключений..." << std::endl;

        while (true) {
            int client_socket = accept(server_socket, nullptr, nullptr);
            if (client_socket < 0) {
                sansara->writeerr("Ошибка подключения клиента");
                continue;
            }

            char buffer[BUFFER_SIZE];
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received <= 0) {
                sansara->writeerr("Ошибка получения данных от клиента");
                close(client_socket);
                continue;
            }

            buffer[bytes_received] = '\0';
            std::string message(buffer);

            // Извлечение логина, соли и хэша
            if (message.length() < 80) {
                sansara->writeerr("Неверная длина сообщения.");
                close(client_socket);
                continue;
            }

            std::string hash = message.substr(message.length() - 64);
            std::string salt = message.substr(message.length() - 80, 16);
            std::string login = message.substr(0, message.length() - 80);

            // Проверка логина в базе данных
            auto it = database.find(login);
            if (it == database.end()) {
                sansara->writeerr("Неизвестный логин: " + login);
                send(client_socket, "ERR", 3, 0); // Отправляем "ERR" при неудачной аутентификации
                close(client_socket);
                continue;
            }

            std::string password = it->second;
            std::string hashed_input = sha256(salt + password);

            if (hashed_input != hash) {
                sansara->writeerr("Ошибка авторизации для логина: " + login);
                send(client_socket, "ERR", 3, 0); // Отправляем "ERR" при неудачной аутентификации
                close(client_socket);
                continue;
            } else {
                std::cout << "Успешная авторизация для логина: " << login << std::endl;
                send(client_socket, "OK", 2, 0); // Отправляем "OK" при успешной аутентификации

                // Обработка векторов
                uint32_t N;
                if (recv(client_socket, &N, sizeof(N), 0) <= 0) {
                    sansara->writeerr("Ошибка получения количества векторов");
                    close(client_socket);
                    continue;
                }
                N = ntohl(N); // Преобразование из сети в хост

                calc calculator;

                for (uint32_t i = 0; i < N; ++i) {
                    uint32_t S;
                    if (recv(client_socket, &S, sizeof(S), 0) <= 0) {
                        sansara->writeerr("Ошибка получения длины вектора");
                        close(client_socket);
                        break;
                    }
                    S = ntohl(S); // Преобразование из сети в хост

                    std::vector<float> vec(S);
                    if (recv(client_socket, vec.data(), S * sizeof(float), 0) <= 0) {
                        sansara->writeerr("Ошибка получения вектора");
                        close(client_socket);
                        break;
                    }

                    float result = calculator.process_vector(vec);
                    result = htonl(result); // Преобразование в сеть перед отправкой
                    if (send(client_socket, &result, sizeof(result), 0) <= 0) {
                        sansara->writeerr("Ошибка отправки результата");
                        close(client_socket);
                        break;
                    }
                }
            }

            close(client_socket);
        }

        close(server_socket);
    } catch (const criticalerr& e) {
        sansara->writeerr(e.what());
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return -1; // Завершение работы сервера при критической ошибке
    } catch (const std::exception& e) {
        sansara->writeerr("Общая ошибка: " + std::string(e.what()));
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return -1; // Завершение работы сервера при общей ошибке
    } catch (...) {
        sansara->writeerr("Необработанное исключение");
        std::cerr << "Необработанное исключение" << std::endl;
        return -1; // Завершение работы сервера при необработанном исключении
    }

    return 0;
}
