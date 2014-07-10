LIBS=-lfcgi -lfcgi++
OPT=

run: main.o Tsux.o
	g++ $^ -o $@ $(LIBS) $(OPT)

main.o: src/main.cpp
	g++ -c src/main.cpp

Tsux.o: src/Tsux.cpp
	g++ -c src/Tsux.cpp


