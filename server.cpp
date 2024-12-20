//server.cpp
#include "server.h"
#include "calc.h"
#include "error.h"
#include "log.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#define BUFFER_SIZE 1024

std::tuple<bool, int, sockaddr_in> server::create_socket(int port, logtxt* logger)
{
    try {
        sockaddr_in server_addr = {}; // Инициализация нулями
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);
        
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            throw criticalerr("Ошибка создания сокета");
        } else {
            std::cout << "Сокет создан" << std::endl;
        }
        
        return std::make_tuple(true, server_socket, server_addr);
    } catch (const criticalerr& e) {
        if (logger) logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        return std::make_tuple(false, -1, sockaddr_in());
    }
}

std::tuple<bool, sockaddr_in> server::bind_socket(int server_socket, sockaddr_in server_addr, logtxt* logger)
{
	try{  
        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(server_socket);
            throw criticalerr("Ошибка привязки сокета");
        } else {
            std::cout << "Сокет привязан" << std::endl;
        }
        return std::make_tuple(true, server_addr);
        
	} catch  (const criticalerr& e) {
        if (logger) logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        return std::make_tuple(false, sockaddr_in());
    } 
} 

std::tuple<bool, sockaddr_in> server::listen_socket(int port, int server_socket, sockaddr_in server_addr, logtxt* logger)
{ try{
        if (listen(server_socket, 2) < 0) {
            close(server_socket);
            throw criticalerr("Ошибка прослушивания");
        } else {
            std::cout << "Сокет прослушивается" << std::endl;
        }
        std::cout << "Сервер запущен на 127.0.0.1:" << port << "..." << std::endl;
        return std::make_tuple(true, server_addr);
	} catch  (const criticalerr& e) {
        if (logger) logger->writeerr("CRIT ERROR - " + std::string(e.what()));
        std::cerr << "CRIT ERROR - " << e.what() << std::endl;
        return std::make_tuple(false, sockaddr_in());
    }
}

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
        std::thread::id clientthreadid = std::this_thread::get_id();
        // Блокировка мьютекса перед выводом
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Поток [" << clientthreadid << "] начал работу: " << std::endl;
        }
        // Запуск таймера отключения
        std::thread disconnect_timer([client_socket, clientthreadid, this]() {
            int timeinsec = 30;
            std::this_thread::sleep_for(std::chrono::seconds(timeinsec));
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Поток [" << clientthreadid << "] был отключен спустя " << timeinsec << " секунд" << std::endl;
            }
            close(client_socket); // Закрывает сокет клиента
        });
        disconnect_timer.detach(); // Отделяет поток, чтобы он работал параллельно
        char buffer[BUFFER_SIZE];
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            throw noncriticalerr("Ошибка получения сообщения от клиента");
        } else {
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Поток [" << clientthreadid << "] | Сервер получил сообщение: ";
            }
        }
        buffer[bytes_received] = '\0';
        std::string message(buffer);
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << message << std::endl;
        }
        // Извлечение логина, соли и хэша
        if (message.length() < 80) {
            throw noncriticalerr("Неверная длина сообщения");
        } else {
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Поток [" << clientthreadid << "] | Длина сообщения правильная" << std::endl;
            }
        }
        std::string hash = message.substr(message.length() - 64);
        std::string salt = message.substr(message.length() - 80, 16);
        std::string login = message.substr(0, message.length() - 80);
        // Проверка логина в базе данных
        auto it = database.find(login);
        if (it == database.end()) {
            ssize_t recsen = send(client_socket, "ERR", 3, 0);
            if (recsen <= 0) {
                throw noncriticalerr("Ошибка отправки \"ERR\"");
            }
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Поток [" << clientthreadid << "] | Неизвестный логин: " << login << std::endl;
            }
            throw noncriticalerr("Неизвестный логин: " + login);
        } else {
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Поток [" << clientthreadid << "] | Пользователь " << login << " присутствует в базе данных" << std::endl;
            }
        }
        std::string password = it->second;
        std::string hashed_input = sha256(salt + password);
        if (hashed_input != hash) {
            ssize_t recsen = send(client_socket, "ERR", 3, 0);
            if (recsen <= 0) {
                throw noncriticalerr("Ошибка отправки \"ERR\"");
            }
            throw noncriticalerr("Ошибка авторизации для логина: " + login);
        } else {
            ssize_t recsen = send(client_socket, "OK", 2, 0);
            if (recsen <= 0) {
                throw noncriticalerr("Ошибка отправки \"OK\"");
            }
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Поток [" << clientthreadid << "] | \"OK\" отправлено успешно" << std::endl;
                std::cout << "Поток [" << clientthreadid << "] | Пользователь " << login << " успешно авторизовался" << std::endl;
            }
            // Обработка векторов
            uint32_t N;
            if (recv(client_socket, &N, sizeof(N), 0) <= 0) {
                throw noncriticalerr("Ошибка получения количества векторов");
            } else {
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "Поток [" << clientthreadid << "] | Количество векторов получено" << std::endl;
                }
            }
            calc calculator;
            for (uint32_t i = 0; i < N; ++i) {
                uint32_t counter = i + 1;
                uint32_t S;
                if (recv(client_socket, &S, sizeof(S), 0) <= 0) {
                    throw noncriticalerr("Ошибка получения длины " + std::to_string(counter) + "-го вектора");
                } else {
                    {
                        std::lock_guard<std::mutex> lock(cout_mutex);
                        std::cout << "Поток [" << clientthreadid << "] | Длина " << counter << "-го вектора получена" << std::endl;
                    }
                }
                S = ntohl(S); // Преобразование из сети в хост
                std::vector<float> vec(S);
                if (recv(client_socket, vec.data(), S * sizeof(float), 0) <= 0) {
                    throw noncriticalerr("Ошибка получения " + std::to_string(counter) + "-го вектора");
                } else {
                    {
                        std::lock_guard<std::mutex> lock(cout_mutex);
                        std::cout << "Поток [" << clientthreadid << "] | Вектор " << counter << " получен" << std::endl;
                    }
                }
                float result = calculator.countoper(vec);
                if (send(client_socket, &result, sizeof(float), 0) <= 0) {
                    throw noncriticalerr("Ошибка отправки " + std::to_string(counter) + "-го результата");
                } else {
                    {
                        std::lock_guard<std::mutex> lock(cout_mutex);
                        std::cout << "Поток [" << clientthreadid << "] | Результат " << counter << " отправлен" << std::endl;
                    }
                }
            }
        }
        close(client_socket);
    } catch (const noncriticalerr& e) {
        std::ostringstream oss;
        oss << "Поток [" << std::this_thread::get_id() << "] | " << "NONCRIT ERROR - " << e.what();
        logger->writeerr(oss.str());
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cerr << oss.str() << std::endl;
        }
        close(client_socket);
        return;
    } 
}

int server::connection(int port, std::map<std::string, std::string> database, logtxt* logger)
{
    try {
        bool fcs;
        int server_socket;
        sockaddr_in server_addr;
        std::tie(fcs, server_socket, server_addr) = create_socket(port, logger);
        if (fcs == false) {
            exit(1);
        }
        
        bool fbs;
        std::tie(fbs, server_addr) = bind_socket(server_socket, server_addr, logger);
        if (fbs == false) {
        	exit(1);
        }
        
        bool fls;
        std::tie(fls, server_addr) = listen_socket(port, server_socket, server_addr, logger);
        if (fls == false) {
        	exit(1);
        }
        
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
    }
    return 0;
}
