# specify the compiler
CC=g++

# specify compiler flags
CFLAGS=-std=c++11 -Wall

# specify the target
TARGET=gatorLibrary

all: $(TARGET)

$(TARGET): gatorlibrary.cpp
	$(CC) $(CFLAGS) -o $(TARGET) gatorlibrary.cpp

run: $(TARGET)
	./$(TARGET) $(FILE)

clean:
	rm $(TARGET)