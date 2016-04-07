CC=gcc
FLAGS=
CFLAGS=-c $(FLAGS)
LDFLAGS=$(FLAGS) -lpthread
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=onibus

all: $(SOURCES) $(EXECUTABLE)
	
run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

debug: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS) -g -DDEBUG

test: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS) -g -DDEBUG -DTEST
	./$(EXECUTABLE)

sublimegdb: $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS) -g -DDEBUG -DSGDB -DTEST

.c.o:
	$(CC) $< -o $@ $(CFLAGS)

clear:
	rm *.o &
	rm *.trace &
	rm $(EXECUTABLE)