CC=gcc
CFLAGS=-g -fPIC
LDFLAGS=-shared -o

WIN_BIN=lib_envoy.dll
UNIX_BIN=libenvoy.so

OBJFILES=$(wildcard src/*.c)
TESTS = $(patsubst %.c, %, $(wildcard t/*.c))

all: unix

unix:
	$(CC) $(CFLAGS) $(OBJFILES) $(LDFLAGS) $(UNIX_BIN)

win:
	$(CC) $(CFLAGS) $(OBJFILES) $(LDFLAGS) $(WIN_BIN)

clean:
	rm $(TARGET)
