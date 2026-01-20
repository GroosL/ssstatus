CC = cc
CFLAGS = -Wall -O2
TARGET = ssstatus
SRC = main.c assets/*.c

all: $(TARGET)

$(TARGET): $(SRC) assets/*.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

debug: CFLAGS = -Wall -g -O0
debug: $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean debug
