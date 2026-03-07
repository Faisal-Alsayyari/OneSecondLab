CC = gcc
CFLAGS = -I Core -Wall -Wextra -std=c11 -g

TARGET = test
SRC = test.c Core/timing.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
