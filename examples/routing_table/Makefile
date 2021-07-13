CC=gcc
OBJFILES=$(wildcard ./*.c)
INTERMEDIARY=main
TARGET=demo

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -I ../../src $(OBJFILES) -o $(INTERMEDIARY) -lpthread -L../../ -lenvoy

clean:
	rm $(TARGET)
