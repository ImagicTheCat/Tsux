OPT=-Wall -shared -fPIC
OBJ=Tsux.o ParamSet.o FileSet.o Action.o Regex.o URI.o

lib/libtsux.so: $(OBJ)
	g++ $^ -o $@ $(OPT)

clean:
	rm $(OBJ)

Tsux.o: src/Tsux.cpp src/Tsux.hpp
	g++ -c src/Tsux.cpp $(OPT)

ParamSet.o: src/ParamSet.cpp src/ParamSet.hpp
	g++ -c src/ParamSet.cpp $(OPT)

FileSet.o: src/FileSet.cpp src/FileSet.hpp
	g++ -c src/FileSet.cpp $(OPT)

Action.o: src/Action.cpp src/Action.hpp
	g++ -c src/Action.cpp $(OPT)

Regex.o: src/Regex.cpp src/Regex.hpp
	g++ -c src/Regex.cpp $(OPT)

URI.o: src/URI.cpp src/URI.hpp
	g++ -c src/URI.cpp $(OPT)

