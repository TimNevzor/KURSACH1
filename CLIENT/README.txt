Для самописного клиента:

chmod u+x ./client_float
./client_float

или

make
./client_float -H SHA256 -S c

Запуск нескольких (пяти) клиентов:

./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c & ./client_float -H SHA256 -S c

*Сервер поддерживает только тип хэширования "SHA256" и сторону формирования соли "c" (клиент)
*Сервер работает только на порту 33333
*При аутентификации сервер поддерживает ТОЛЬКО ОДНОГО пользователя с логином "user" и паролем "P@ssW0rd"
