target =  m
sources = main.cpp interface.cpp server.cpp
CC = g++
LDLIBS = -pthread -lboost_program_options -lcryptopp
all:build
build:
	$(CC) $(sources) -o $(target) $(LDLIBS)
dbg:
	$(CC) -g $(sources) -o $(target)DBG
clean:
	rm -f $(target) $(DEBUG_EXEC)
