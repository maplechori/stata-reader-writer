CC=g++
CFLAGS=
LDFLAGS=-lboost_system
SOURCES=main.cpp StataBase.cpp StataHeader.cpp StataMap.cpp StataVariables.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	@echo 'Building target: $@. First dep: $<'
	${CC} -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS) 

.c.o:
		@echo 'Building target: $@. First dep: $<'
		$(CC) $(CFLAGS) $< -o $@


clean:
		rm -rf *.o 
		rm -rf main
