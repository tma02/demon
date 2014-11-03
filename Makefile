CFLAGS := -Wall
SRC_DIR := src/
EMU_OBJ := $(SRC_DIR)emulate/dcpu.o

all: emu

emu: $(EMU_OBJ)
	$(CC) -o emu $(EMU_OBJS)

$(EMU_OBJ): $(SRC_DIR)emulate/dcpu.c $(SRC_DIR)emulate/dcpu.h

.PHONY: clean
clean:
	rm -f emu $(SRC_DIR)*.o
