# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
# Executable "rest" depends on the files rest.o and run.o.
rest: bin/Table.o bin/Action.o bin/Customer.o bin/Restaurant.o bin/Dish.o bin/main.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/rest  bin/Table.o bin/Action.o bin/Customer.o bin/Restaurant.o bin/Dish.o bin/main.o $(LFLAGS)
	@echo 'Finished building target: rest'
	@echo ' '

bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp

bin/Table.o: src/Table.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp

bin/Restaurant.o: src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

bin/Customer.o: src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Dish.o: src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp

bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
#Clean the build directory
clean: 
	rm -f bin/*
