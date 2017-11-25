TARGETS=stata_readwrite stata_readwrite_test
CC=g++
CFLAGS=
DEBUG=-g
LDFLAGS=-lboost_system -lboost_program_options -lboost_unit_test_framework -lsqlite3
SOURCES=main.cpp StataHeader.cpp StataMap.cpp StataVariables.cpp StataValueLabel.cpp State.cpp StateBitops.cpp SQLite.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=stata_readwrite

SOURCES_TEST = $(filter-out main.cpp, $(SOURCES))
SOURCES_TEST += StataTest.cpp
OBJECTS_TEST=$(SOURCES_TEST:.c=.o)
EXECUTABLE_TEST = stata_readwrite_test

all: $(TARGETS)

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
		rm -rf stata_readwrite stata_readwrite_test

