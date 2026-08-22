CC = cc
CFLAGS = -Wall -O2

TARGET = ssstatus

SRC = main.c $(wildcard assets/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

config.h: config.def.h
	cp config.def.h config.h

$(TARGET): config.h $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS = -Wall -g -O0
debug: clean $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all debug clean
