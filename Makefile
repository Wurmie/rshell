#Variables
CXX = g++
CFLAGS = -ansi -pedantic -Wall -Werror
VPATH = src
FILEPATH = ./src/main.cpp
FILEPATH1 = ./src/ls.cpp
FILEPATH2 = ./src/cp.cpp
OUTPATH = ./bin/rshell
OUTPATH1 = ./bin/ls
OUTPATH2 = ./bin/cp
objects = main.o

#Targets
all: 
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH)
	$(CXX) $(CFLAGS) $(FILEPATH1) -o $(OUTPATH1)
	$(CXX) $(CFLAGS) $(FILEPATH2) -o $(OUTPATH2)

rshell: 
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH) -o $(OUTPATH) 
ls:
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH1) -o $(OUTPATH1) 

cp:
	mkdir -p ./bin
	$(CXX) $(CFLAGS) $(FILEPATH2) -o $(OUTPATH2)
clean:
	rm -rf ./bin
