CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = server

all: $(TARGET)

$(TARGET): server.c
	$(CC) $(CFLAGS) server.c -o $(TARGET)

clean:
	rm -f $(TARGET)