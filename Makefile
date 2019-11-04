CC=g++
CFLAGS= --std=c++17 -g

.SILENT:

make:
	$(CC) $(CFLAGS) main.cpp -o Main

run:
	$(CC) $(CFLAGS) main.cpp -o Main && ./Main
