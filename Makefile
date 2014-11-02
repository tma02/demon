CFLAGS := -Wall
SRC_DIR := src/

all: emu

EMU_OBJS := $(SRC_DIR)emulate/dcpu.o
emu: $(EMU_OBJS)
	$(CC) -o emu $(EMU_OBJS)

$(SRC_DIR)emulate/dcpu.c: $(SRC_DIR)emulate/dcpu.h

clean:
	rm -f emu $(SRC_DIR)*.o
