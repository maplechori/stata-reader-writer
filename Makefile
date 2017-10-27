CC=g++
CFLAGS=
DEBUG=-g
LDFLAGS=-lboost_system -lboost_unit_test_framework
SOURCES=main.cpp StataBase.cpp StataHeader.cpp StataMap.cpp StataVariables.cpp StataRead.cpp StataValueLabel.cpp State.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

SOURCES_TEST = $(filter-out main.cpp, $(SOURCES))
SOURCES_TEST += StataTest.cpp
OBJECTS_TEST=$(SOURCES_TEST:.c=.o)
EXECUTABLE_TEST = main_test

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	@echo 'Building target: $@. First dep: $<'
	${CC} -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS) $(DEBUG)

.c.o:
		@echo 'Building target: $@. First dep: $<'
		$(CC) $(CFLAGS) $< -o $@

test: $(SOURCES_TEST) $(EXECUTABLE_TEST)

$(EXECUTABLE_TEST): $(OBJECTS_TEST)
	@echo 'Building target: $@. First dep: $<'
	${CC} -o $(EXECUTABLE_TEST) $(OBJECTS_TEST) $(LDFLAGS) $(DEBUG)
	

clean:
		rm -rf *.o 
		rm -rf main main_test

