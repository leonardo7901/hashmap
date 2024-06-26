DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -g3
else
    CFLAGS = -Ofast
endif
CC = cc
TARGET = list
OBJ = main.o map.o
.PHONY = clean re

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LINK) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET) 2>/dev/null

re:
	make clean
	make -j