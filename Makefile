CC=g++
CFLAGS= --std=c++17 -g -Wall
BUILDDIR=bin
SRC=$(wildcard src/*)

.PHONY: clean run

run: $(BUILDDIR)/tests
	./$<

$(BUILDDIR)/tests: src/tests.cpp $(SRC) $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR):
	mkdir $@

clean:
	rm -rf bin

