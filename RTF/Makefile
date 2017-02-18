ifndef CC
	CC: gcc
endif

LAPACK := ~/lapack-3.5.0
INCLUDE := $(LAPACK)/lapacke/include/
LIB := -lm 
OBJ := io.o numc.o
CFLAGS := -O3

all:crtf

crtf:%:%.o $(OBJ)
	gfortran $^ $(LAPACK)/liblapacke.a $(LAPACK)/liblapack.a $(LAPACK)/librefblas.a -o $@

#%o.:%.c
#	$(CC) -c -I $(INCLUDE)
.c.o:
	$(CC) -c $(CFLAGS) -I $(INCLUDE) -o $@ $<

clean:
	rm crtf *.o
