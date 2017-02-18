ifndef CC
	CC: gcc
endif

LIB := -lm
OBJ :=  mod.o grid.o ray.o numc.o hetero.o

all:ray_2d ttime

ray_2d:%:%.o $(OBJ)
	$(CC) $^ $(LIB) -O3 -o $@

ttime:ttime.o $(OBJ)
	$(CC) $^ $(LIB) -O3 -o $@

%o.:%.c

clean:
	rm -v ray_2d *.o
