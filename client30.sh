#!/bin/bash
for i in {1..30}; do
    ./client_float -H SHA256 -S c &
done
# Ожидание завершения всех фоновых процессов
wait
