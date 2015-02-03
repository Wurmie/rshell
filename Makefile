#Variables
CXX = g++
CFLAGS = -ansi -pedantic -Wall -Werror
VPATH = src
FILEPATH = ./src/main.cpp
FILEPATH1 = ./src/ls.cpp
OUTPATH = ./bin/rshell
OUTPATH1 = ./bin/ls
objects = main.o

#Targets
all: 
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
	$(CXX) $(CFLAGS) $(FILEPATH1) -o $(OUTPATH1)

rshell: 
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH) 
ls:
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH1) -o $(OUTPATH1) 
clean:
	rm -rf ./bin
