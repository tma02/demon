CFLAGS := -Wall
SRC_DIR := src/

all: emu

EMU_OBJS := $(SRC_DIR)emulate.o
emu: $(EMU_OBJS)
	$(CC) -o emu $(EMU_OBJS)

$(SRC_DIR)emulate.c: $(SRC_DIR)emulate.h

clean:
	rm -f emu $(SRC_DIR)*.o
