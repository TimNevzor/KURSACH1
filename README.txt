Для сервера:

make
./main

Для клиента:

chmod u+x ./client_float
./client_float -H SHA256 -S c

Для запуска юнит-тестов:

make test
./testmain

Значения по умолчанию:

Путь до лог-файла: = log.txt
Путь до БД: base.txt
Порт по умолчанию: 33333

Пример запуска сервера с ключами:

./main -b [путь до БД] -l [путь до лог-файла] -p [порт]

./main -h      <-(вызов справки)

Запуск нескольких (пяти) клиентов:

for i in {1..5}; do ./client_float -H SHA256 -S c & done

./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c
