#компилятор
CXX = g++
#флаги и линковка
LDLIBS = -pthread -lboost_program_options -lcryptopp -std=c++17
U_LDLIBS = -std=c++17 -pthread -lboost_program_options -lcryptopp -lUnitTest++
#исходные файлы
sources = main.cpp interface.cpp log.cpp server.cpp calc.cpp
u_sources = testmain.cpp interface.cpp log.cpp server.cpp calc.cpp
#исполняемые файлы
target = main
u_target = testmain
#правила:
all: $(target)
	$(CXX) -o $(target) $(sources) $(LDLIBS)

test: $(u_target)
	$(CXX) -o $(u_target) $(u_sources) $(U_LDLIBS)

clean:
	rm -f $(target) $(u_target)
.PHONY: all test clean
