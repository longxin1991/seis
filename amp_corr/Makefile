FC=gfortran
CC=gcc
LAPACK := ~/lapack-3.5.0
INCLUDE := $(LAPACK)/lapacke/include/
LIBS = -lm -lgfortran
STATIC_LIB = $(LAPACK)/liblapacke.a $(LAPACK)/liblapack.a $(LAPACK)/librefblas.a

COBJ = mod.o numc.o layer.o rtf.o ioapi.o corr.o amp_corr.o
FOBJ = ttimesapi.o libtau.o libsun.o

all:amp_corr

rtf.o:rtf.c
	gcc -c $^ -I ${INCLUDE} ${LIBS}

amp_corr: ${COBJ} ${FOBJ}
	gcc $^ ${STATIC_LIB} ${LIBS} -I ${INCLUDE}  -o $@ 
	
test:test.o ${FOBJ} 
	${FC} $^ ${CFLAGS} -o $@
test.o:test.c
	${CC} -c $^

%o.:%.f
	${FC} -c 

clean:
	rm -rvf *.o amp_corr
