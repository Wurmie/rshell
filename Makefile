#Variables
CXX = g++
CFLAGS = -ansi -pedantic -Wall -Werror
VPATH = src
FILEPATH = ./src/main.cpp
OUTPATH = ./bin/rshell
objects = main.o

#Targets
all: 
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)

rshell: 
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)  
clean:
	rm -rf ./bin
