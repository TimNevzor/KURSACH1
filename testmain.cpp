//testmain.cpp
#include <filesystem>
#include <UnitTest++/UnitTest++.h>
#include <fstream>
#include "interface.h"
#include "error.h"
#include "log.h"
#include "server.h"
#include "calc.h"
//тесты класса interface
SUITE(interface_tests) {
	//ввод без параметров
	TEST(check_args) {
        interface start;
        const char* argv[] = {"main"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "ok");
    }
	//ввод ключа -h
	TEST(check_args_h) {
        interface start;
        const char* argv[] = {"main", "-h"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "help");
    }
    //ввод ключа --help
    TEST(check_args_help) {
        interface start;
        const char* argv[] = {"main", "--help"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "help");
    }
	//ввод ключа -l с параметром [путь до лог-файла]
	TEST(check_args_l_logtxt) {
        interface start;
        const char* argv[] = {"main", "-l", "log.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "ok");
    }
	//ввод ключа -l без параметра
	TEST(check_args_l) {
        interface start;
        const char* argv[] = {"main", "-l"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "error");
    }
	//ввод ключа -b с параметром [путь до базы данных]
	TEST(check_args_b_basetxt) {
        interface start;
        const char* argv[] = {"main", "-b", "base.txt"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "ok");
    }
	//ввод ключа -b без параметра
	TEST(check_args_b) {
        interface start;
        const char* argv[] = {"main", "-b"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "error");
    }
	//ввод ключа -p с параметром [порт]
	TEST(check_args_p_33333) {
        interface start;
        const char* argv[] = {"main", "-p", "33333"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "ok");
    }
	//ввод ключа -p без параметра
	TEST(check_args_p) {
        interface start;
        const char* argv[] = {"main", "-p"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "error");
    }
    //ввод -l -b -p
	TEST(check_args_all_empty) {
        interface start;
        const char* argv[] = {"main", "-l", "-b", "-p"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "error");
    }
    //ввод -l log.txt -b base.txt -p 33333
	TEST(check_args_all) {
        interface start;
        const char* argv[] = {"main", "-l", "log.txt", "-b", "base.txt", "-p", "33333"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        std::string answer_of_check_args;
		std::string path_to_log;
		std::string path_to_base;
		std::string port_str;
		std::tie(answer_of_check_args, path_to_log, path_to_base, port_str) = start.check_args(argc, argv);
        CHECK_EQUAL(answer_of_check_args, "ok");
    }
    //хороший путь до лог-файла
    TEST(l_good_path) {
        interface start;
        std::string pathtologfile = "log.txt";
        CHECK_EQUAL(start.check_path_to_logfile(pathtologfile), true);
    }
    //несуществующий путь до лог-файла
    TEST(l_not_exist_path) {
        interface start;
        std::string pathtologfile = "log1.txt";
        CHECK_EQUAL(start.check_path_to_logfile(pathtologfile), false);
    }
    //хороший путь до базы данных
    TEST(b_good_path) {
        interface start;
        logtxt logger;
    	logger.setpath("log.txt");
        std::string pathtobase = "base.txt";
        bool flag;
        std::map<std::string, std::string> data_base;
        std::tie(flag, data_base) = start.check_path_to_database_and_get_database(pathtobase, &logger);
        CHECK_EQUAL(flag, true);
    }
    //несуществующий путь до базы данных
    TEST(b_not_exist_path) {
        interface start;
        logtxt logger;
    	logger.setpath("log.txt");
        std::string pathtobase = "base1.txt";
        bool flag;
        std::map<std::string, std::string> data_base;
        std::tie(flag, data_base) = start.check_path_to_database_and_get_database(pathtobase, &logger);
        CHECK_EQUAL(flag, false);
    }
    //некорректный формат строки
    TEST(b_bad_line) {
        interface start;
        logtxt logger;
    	logger.setpath("log.txt");
    	std::string filename = "output.txt";
    	std::ofstream outfile(filename);
    	outfile << "user P@ssW0rd\n";
    	outfile << "user:P@ssW0rd\n";
    	outfile << "user-P@ssW0rd\n";
    	outfile.close();
        bool flag;
        std::map<std::string, std::string> data_base;
        std::tie(flag, data_base) = start.check_path_to_database_and_get_database(filename, &logger);
        CHECK_EQUAL(flag, false);
        std::remove(filename.c_str());
    }
    //хороший порт
    TEST(p_good) {
        interface start;
        logtxt logger;
    	logger.setpath("log.txt");
    	std::string portstr = "33333";
        bool flag;
        int port;
        std::tie(flag, port) = start.check_port(portstr, &logger);
        CHECK_EQUAL(flag, true);
    }
    //плохой пустой порт
    TEST(p_bad_empty) {
        interface start;
        logtxt logger;
    	logger.setpath("log.txt");
    	std::string portstr = "";
        bool flag;
        int port;
        std::tie(flag, port) = start.check_port(portstr, &logger);
        CHECK_EQUAL(flag, false);
    }
    //плохой порт
    TEST(p_bad) {
        interface start;
        logtxt logger;
    	logger.setpath("log.txt");
    	std::string portstr = "-1";
        bool flag;
        int port;
        std::tie(flag, port) = start.check_port(portstr, &logger);
        CHECK_EQUAL(flag, false);
    }
}
//тесты класса server
SUITE(server_tests) {
	//успешное создание сокета
    TEST(create_socket_good) {
    	server startconnect;
    	int port = 33333;
    	std::map<std::string, std::string> data_base = {{"user", "P@ssW0rd"}};
    	logtxt logger;
    	logger.setpath("log.txt");
    	bool flag;
        int server_socket;
        sockaddr_in server_addr;
        std::tie(flag, server_socket, server_addr) = startconnect.create_socket(port, &logger);
        CHECK_EQUAL(flag, true);
    }
    //успешная привязка сокета
    TEST(bind_socket_good) {
    	server startconnect;
    	int port = 33333;
    	std::map<std::string, std::string> data_base = {{"user", "P@ssW0rd"}};
    	logtxt logger;
    	logger.setpath("log.txt");
    	bool flag;
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        
        std::tie(flag, server_addr) = startconnect.bind_socket(server_socket, server_addr, &logger);       
        CHECK_EQUAL(flag, true);
    }
    //успешное прослушивание сокета
    TEST(listen_socket_good) {
    	server startconnect;
    	int port = 33333;
    	std::map<std::string, std::string> data_base = {{"user", "P@ssW0rd"}};
    	logtxt logger;
    	logger.setpath("log.txt");
    	bool flag;
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
        std::tie(flag, server_addr) = startconnect.listen_socket(port, server_socket, server_addr, &logger);      
        CHECK_EQUAL(flag, true);
    }
}
int main(int argc, char* argv[]) {
    return UnitTest::RunAllTests();
}
