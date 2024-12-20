#компилятор
CXX = g++
#флаги и линковка
LDLIBS = -lssl -lcrypto -lboost_system -lboost_program_options -pthread
#исходные файлы
sources = client_float.cpp
#исполняемые файлы
target = client_float
#правила:
all: $(target)
	$(CXX) -o $(target) $(sources) $(LDLIBS)
clean:
	rm -f $(target)
.PHONY: all clean
