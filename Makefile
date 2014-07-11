LIBS=-lfcgi -lfcgi++
OPT=

run: main.o Tsux.o ParamSet.o
	g++ $^ -o $@ $(LIBS) $(OPT)

main.o: src/main.cpp
	g++ -c src/main.cpp

Tsux.o: src/Tsux.cpp src/Tsux.hpp
	g++ -c src/Tsux.cpp

ParamSet.o: src/ParamSet.cpp src/ParamSet.hpp
	g++ -c src/ParamSet.cpp
