CC=g++
CFLAGS=
DEBUG=-g
LDFLAGS=-lboost_system -lboost_unit_test_framework
SOURCES=main.cpp StataBase.cpp StataHeader.cpp StataMap.cpp StataVariables.cpp StataRead.cpp State.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	@echo 'Building target: $@. First dep: $<'
	${CC} -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS) $(DEBUG)

.c.o:
		@echo 'Building target: $@. First dep: $<'
		$(CC) $(CFLAGS) $< -o $@


clean:
		rm -rf *.o 
		rm -rf main

