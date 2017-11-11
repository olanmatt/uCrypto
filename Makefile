TARGET = hashlib_test
LIBS =
CC = g++
CFLAGS = --std=c++11 -g -I . -Wall -Wextra -Wpedantic # -Werror

.PHONY: default all clean test

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.c, %.o, $(wildcard test/*.c))
HEADERS = $(wildcard *.h) test/catch.hpp

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	    $(CC) $(OBJECTS) -Wall $(LIBS) -o $@

test: $(TARGET)
	./$(TARGET)

test/catch.hpp:
	wget -P ./test https://github.com/philsquared/Catch/releases/download/v1.9.6/catch.hpp

clean:
	rm test/catch.hpp
	rm -f $(OBJECTS)
	rm -f $(TARGET)
