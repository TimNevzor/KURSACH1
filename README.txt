Для сервера:

make
./main

Для клиента:

chmod u+x ./client_float
./client_float -H SHA256 -S c

Для запуска юнит-тестов:

g++ -std=c++17 -o testmain testmain.cpp interface.cpp log.cpp server.cpp calc.cpp -pthread -lboost_program_options -lcryptopp -lUnitTest++

Пути по умолчанию:

До лог-файла: = log.txt
До БД: base.txt
Порт по умолчанию: 33333

Пример запуска сервера с ключами:

./main -b [путь до БД] -l [путь до лог-файла] -p [порт]

./main -h      <-(вызов справки)

Запуск нескольких (пяти) клиентов:

./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c

Полная запись запуска сервера:

(g++ -o main main.cpp interface.cpp log.cpp server.cpp calc.cpp -pthread -lboost_program_options -lcryptopp)
