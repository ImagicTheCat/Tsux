LIB=-ltsux -lfcgi -lfcgi++
OPT=

run: main.o mMain.o mChat.o
	g++ $^ -o $@ $(LIB) $(OPT)

main.o: main.cpp
	g++ -c main.cpp

mMain.o: mMain.cpp mMain.hpp
	g++ -c mMain.cpp


mChat.o: mChat.cpp mChat.hpp
	g++ -c mChat.cpp
