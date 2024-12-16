#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <boost/program_options.hpp>

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

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 33333
#define BUFFER_SIZE 1024

namespace po = boost::program_options;

// Функция генерации соли
std::string generateRandomSalt(size_t length) {
    const char hex_chars[] = "0123456789ABCDEF";
    std::string salt;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    for (size_t i = 0; i < length; ++i) {
        salt += hex_chars[dist(mt)];
    }

    return salt;
}

// Функция хеширования пароля с использованием SHA-256
std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return oss.str();
}

void clientFunction(std::string hashType, std::string side) {

	std::cout << "Select hash " + hashType << std::endl;
	std::cout << "Тип данных: float" << std::endl;
	
	std::vector<float> v1 = {1, 2, 3, 4};
	std::vector<float> v2 = {80000, 100000, 120000, 120000};
	std::vector<float> v3 = {80000, 100000, 120000, 130000};
	std::vector<float> v4 = {-4.4, -5.5, -6.6, -7.7};
	
	std::cout << "Векторы (4x4)" << std::endl;
	
	std::cout << "         " << v1[0] << "         " << v1[1] << "         " << v1[2] << "         " << v1[3] << std::endl;
	std::cout << "     "     << v2[0] << "    "      << v2[1] << "    "      << v2[2] << "    "      << v2[3] << std::endl;
	std::cout << "     "     << v3[0] << "    "      << v3[1] << "    "      << v3[2] << "    "      << v3[3] << std::endl;	
	std::cout << "      "    << v4[0] << "      "    << v4[1] << "      "    << v4[2] << "      "    << v4[3] << std::endl;
	
    struct sockaddr_in self_addr;
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = 0; // Автоматический выбор порта
    self_addr.sin_addr.s_addr = INADDR_ANY; // Принять соединения от любого адреса
    
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
       
    int s = socket(AF_INET, SOCK_STREAM, 0); //создает сокет для TCP-соединения
    if (s == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;///////////////////////////////////////////////////
        return;
    } else {
    	std::cout << "socket success" << std::endl;
    }
    
    int e = bind(s, (struct sockaddr*)&self_addr, sizeof(self_addr)); //привязывает сокет к локальному адресу и порту
    if (e == -1) {
        std::cerr << "Error binding socket" << std::endl;//////////////////////////////////////////////
        close(s);
        return;
    } else {
    	std::cout << "bind success" << std::endl;
    }
    
    e = connect(s, (struct sockaddr*)&srv_addr, sizeof(srv_addr)); //подключает сокет к серверу по указанному адресу и порту
    if (e == -1) {
        std::cerr << "Connection refused Останов." << std::endl;
        close(s);
        return;
    } else {
    	std::cout << "connect success" << std::endl;
    }
    
    std::cout << "Фаза аутентификации:" << std::endl;
    
	std::string username = "user";
    std::string salt;
    salt = generateRandomSalt(16);
    std::cout << "generate salt: " << salt << std::endl;
    
    std::string password = "P@ssW0rd";
    std::string salted_password = salt + password;
    std::string hashed_password = sha256(salted_password);
    std::string message = username + salt + hashed_password;
    std::cout << "send message: " << message << std::endl;
	
	e = send(s, message.c_str(), message.size(), 0);
	
    if (e == -1) {
        std::cerr << "Connection reset by peer Останов." << std::endl;
        close(s);
        return;
    }

    char buffer[BUFFER_SIZE] = {0};
    
    ssize_t recv_size = recv(s, buffer, BUFFER_SIZE - 1, 0);
    if (recv_size < 0) {
        std::cerr << "Не \"OK\" после аутентификации. Останов." << std::endl;
        close(s);
        return;
    } else {
        buffer[recv_size] = '\0';
        std::cout << "receive " << recv_size <<  " bytes as answer: " << buffer << std::endl;
    }
    
    std::cout << "Фаза вычислений" << std::endl;
    
    uint32_t N = 4;
    
    e = send(s, &N, sizeof(N), 0);
    
    if (e == -1) {
    	close(s);
    	return;
    } else {
    	std::cout << "send numbers of vectors" << std::endl;
    }
    
    for(uint32_t i = 0; i < 4; i++) {
    	uint32_t counter = i + 1;
    	
    	uint32_t S = 4;
    	e = send(s, &S, sizeof(S), 0);
    
    	if (e == -1) {
    		close(s);
    		return;
    	} else {
    		std::cout << "send size of vector " << counter << std::endl;
    	}
    	
    	if (i == 0) {
    		e = send(s, v1.data(), v1.size() * sizeof(float), 0);
    	}
    	
    	if (i == 1) {
    		e = send(s, v2.data(), v2.size() * sizeof(float), 0);
    	}
    	
    	if (i == 2) {
    		e = send(s, v3.data(), v3.size() * sizeof(float), 0);
    	}
    	
    	if (i == 3) {
    		e = send(s, v4.data(), v4.size() * sizeof(float), 0);
    	}
    	
    	if (e == -1) {
    		std::cout << "send data of vector " << counter << std::endl;
    		std::cout << "Connection reset by peer Останов." << std::endl;
    		close(s);
    		return;
    	} else {
    		std::cout << "send data of vector " << counter << std::endl;
    	}
 	
    	ssize_t recv_size = recv(s, buffer, sizeof(float), 0);
    	if (recv_size < 0) {
        	std::cout << "receive " << recv_size << " bytes as result of calc vector " << counter << ": " << 0 << std::endl;
        	std::cerr << "Количество присланных байт не совпадает с размером типа float Останов." << std::endl;
        	close(s);
        	return;
    	} else if (recv_size != sizeof(float)) {
    		std::cout << "receive " << recv_size << " bytes as result of calc vector " << counter << ": " << 0 << std::endl;
        	std::cerr << "Количество присланных байт не совпадает с размером типа float Останов." << std::endl;
    		close(s);
    		return;
    	} else {
			float result;

    		memcpy(&result, buffer, sizeof(float));
    		std::cout << "receive " << recv_size << " bytes as result of calc vector " << counter << ": " << result << std::endl;
    	}
    }
    close(s);
}

int main(int argc, char* argv[]) {

    std::string hashType;
    std::string side;

    po::options_description desc("Допустимые опции");
    desc.add_options()
        ("help,h", "Вызов справки")
        ("hash,H", po::value<std::string>(&hashType)->default_value("SHA256"), "Тип хэширования (например, SHA256)")
        ("side,S", po::value<std::string>(&side)->default_value("c"), "Источник соли: 'c' для клиента");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: client -H MD5|SHA1|SHA224|SHA256 -S s[erver]|c[lient]" << "\n";
        return 1;
    }

    if (hashType != "SHA256") {
        std::cerr << "Поддерживается только SHA256" << std::endl;
        return 1;
    }
    
    if (side != "c") {
        std::cerr << "Поддерживается только сторона формирования соли со стороны клиента" << std::endl;
        return 1;
    }

    clientFunction(hashType, side);
    return 0;
}
