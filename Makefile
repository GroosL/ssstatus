CC = cc
CFLAGS = -Wall -O2
TARGET = ssstatus
SRC = main.c assets/cpu.c assets/memory.c assets/customCommand.c

all: $(TARGET)

$(TARGET): $(SRC) assets/cpu.h assets/memory.h assets/customCommand.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

debug: CFLAGS = -Wall -g -O0
debug: $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean debug
