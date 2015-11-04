ifndef CC
	CC: gcc
endif

LIB := -lm
OBJ := numc.o tracing.o mod.o mem.o struct.o velocity.o

all:ray_1d

ray_1d:%:%.o $(OBJ)
	$(CC) $^ $(LIB) -o $@

test:%:%.o $(OBJ)
	$(CC) $^ $(LIB) -o $@

%o.:%.c

clean:
	rm ray_1d *.o
