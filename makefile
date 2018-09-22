CC = gcc
CFLAGS = -Iinclude -lpthread -lwiringPi


TOP_PATH = $(shell pwd)
SRC_PATH = $(TOP_PATH)/src
BIN_PATH = $(TOP_PATH)/bin
LIB_PATH = $(TOP_PATH)/lib
INC_PATH = $(TOP_PATH)/include

TARGET = $(BIN_PATH)/fan

$(TARGET): $(LIB_PATH)/fan.o $(SRC_PATH)/main.c
	$(CC) $(CFLAGS) $(SRC_PATH)/main.c $(LIB_PATH)/fan.o -o $(TARGET)

$(LIB_PATH)/fan.o: $(SRC_PATH)/fan.c
	$(CC) $(CFLAGS) -o lib/fan.o -c src/fan.c


clean:
	rm -rf lib/*.o bin/*
	
