all: bin
	g++ -g -w -Wall -o bin/main main.cpp -std=c++11

bin:
	mkdir bin
