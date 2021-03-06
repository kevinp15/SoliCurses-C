CC=gcc
CFLAGS=-c -g -pedantic -std=gnu11 -Wall
LDFLAGS=-lncurses
EXECUTABLE=./SoliCurses-C.out
SRCDIR=./src
BINDIR=./bin

SOURCES=$(wildcard ${SRCDIR}/*.c)

OBJECTS=$(SOURCES:${SRCDIR}/%.c=$(BINDIR)/%.o)

all : $(EXECUTABLE)

$(EXECUTABLE) : ${OBJECTS}
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean : 
	rm -rf $(BINDIR)/*.o $(EXECUTABLE)

run : all
	$(EXECUTABLE)