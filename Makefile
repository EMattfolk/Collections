CC=g++
CFLAGS= --std=c++17 -g -Wall

.SILENT: default run
.PHONY: default run

default:
	$(CC) $(CFLAGS) tests.cpp -o Tests

run:
	$(CC) $(CFLAGS) tests.cpp -o Tests && ./Tests
