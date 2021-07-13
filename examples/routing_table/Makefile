CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c17
LDFLAGS=
OBJFILES=$(wildcard ./*.c)
TARGET=demo

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
	
clean:
	rm $(TARGET)
