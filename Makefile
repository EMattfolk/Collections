CC=g++
CFLAGS= --std=c++17 -g -Wall

.SILENT: default run
.PHONY: default run

default:
	$(CC) $(CFLAGS) main.cpp -o Main

run:
	$(CC) $(CFLAGS) main.cpp -o Main && ./Main
