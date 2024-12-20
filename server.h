/** @file
 * @author Невзоров Т.В.
 * @version 1.0
 * @date 20.12.2024
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля server
 */
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
/** @brief Создание соединения для работы с клиентом
 * @details Создает сокет для взимодействия с клиентом
 * В private секции находится объект cout_mutex типа mutex
 */
class server
{
public:
	/**
        * @brief Основной метод, порождающий другие методы 
        * @details Принимает параметры, которые будут передаваться другим методам,
        * порождает методы по созданию, привязке и прослушиванию сокета.
        * Запускает цикл, в котором принимает клиентов.
        * @param port Порт, на котором будет запускаться сервер
        * @param base Ассоциативный массив с логинами и паролями
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл        
        * @throw noncriticalerr при ошибке подкдючения клиента
        */
    int connection(int port, std::map<std::string, std::string> base, logtxt* logger);
    /**
        * @brief Создание сокета       
        * @details Создает сокет, который в дальнейшем будет использоваться для работы с клиентом 
        * @param port Порт, на котором будет запускаться сервер
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл        
        * @throw criticalerr при ошибке создания сокета
        */
    std::tuple<bool, int, sockaddr_in> create_socket(int port, logtxt* logger);
    /**
        * @brief Привязка сокета        
        * @details Привязывает сокет к адресу и порту, на которых он будет работать 
        * @param server_socket Дескриптор сокета, использующийся для прослушивания входящих соединений
        * @param server_addr Адрес сокета
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл        
        * @throw criticalerr при ошибке привязки сокета
        */
    std::tuple<bool, sockaddr_in> bind_socket(int server_socket, sockaddr_in server_addr, logtxt* logger);
    /**
        * @brief Прослушивание сокета        
        * @details Прослушивает сервер 
 		* @param port Порт, на котором запускается сервер
        * @param server_socket Дескриптор сокета, использующийся для прослушивания входящих соединений
        * @param server_addr Адрес сокета
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл
        * @throw criticalerr при ошибке прослушивания сокета
        */
    std::tuple<bool, sockaddr_in> listen_socket(int port, int server_socket, sockaddr_in server_addr, logtxt* logger);
    /**
        * @brief Метод, отвечающий за взаимодействие с клиентом       
        * @details Данный метод выполняет следующие действия:
        * принимает от клиента строку, содержащую логин, соль и хэш;
        * передает соль и пароль из базы данных в метод хэшироавния;
        * сравнивает захэшированный пароль с хэшем от клиента;
        * получает количество, длину и содержимое векторов;
        * порождает объект calculator класса calc и передает ему векторы;
        * отправляет клиенту посчитанный результат.
        * После 30 секунд метод закрывает сокет       
        * @param client_socket Дескриптор сокета, представляющий собой соединение с конкретным клиентом
        * @param base Ассоциативный массив с логинами и паролями
        * @param logger Указатель на объект класса logtxt, необходимый для записи ошибок в лог-файл        
        * @throw noncriticalerr при следующих ошибках:
        * Ошибка получения сообщения от клиента; 
        * Неверная длина сообщения; 
        * Ошибка отправки "ERR" при неизвестном логине;
        * Неизвестный логин; 
        * Ошибка отправки "ERR" при ошибке авторизации;
        * Ошибка авторизации для логина;
        * Ошибка отправки "OK"; 
        * Ошибка получения количества векторов;
        * Ошибка получения длины n-го вектора;
        * Ошибка получения n-го вектора; 
        * Ошибка отправки n-го результата
        */
    void threadclient(int client_socket, std::map<std::string, std::string> base, logtxt* logger);
    /**
        * @brief Хеширование пароля       
        * @details Хэширует строку, содержащую соль и пароль
        * @param message Константная ссылка на строку, содержащую сообщение из соли и пароля
        */
    std::string sha256(const std::string& message);
private:
    std::mutex cout_mutex; ///< Мьютекс для предотвращения состояния гонки при выводе сообщений на экран
};
