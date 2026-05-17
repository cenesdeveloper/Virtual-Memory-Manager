CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = vmm
SOURCES = main.c tlb.c memory.c

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
