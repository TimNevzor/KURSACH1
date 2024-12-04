Для сервера:

g++ -o m main.cpp interface.cpp server.cpp -pthread -lboost_program_options -lcryptopp
./m

Для клиента:

chmod u+x ./client_float
./client_float -H SHA256 -S c

Пути по умолчанию:

До БД: /home/stud/Desktop/KURSACH/base.txt
До лог-файла: = /home/stud/Desktop/KURSACH/log.txt
Порт по умолчанию: 33333

Пример запуска сервера с ключами:

./m -b [путь до БД] -l [путь до лог-файла] -p [порт]

./m -h      <-(вызов справки)
