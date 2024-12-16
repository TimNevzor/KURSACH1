target =  client_float
sources = client_float.cpp
CC = g++
LDLIBS = -lssl -lcrypto -lboost_system -lboost_program_options -pthread
all:build
build:
	$(CC) $(sources) -o $(target) $(LDLIBS)
dbg:
	$(CC) -g $(sources) -o $(target)DBG
clean:
	rm -f $(target) $(DEBUG_EXEC)
