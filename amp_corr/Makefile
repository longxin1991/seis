FC=gfortran
OS=$(shell uname)
ifeq ($(OS),Darwin)
	CC=gcc-5
else
	CC=gcc
endif
LAPACK := ~/lapack-3.5.0
INCLUDE := $(LAPACK)/lapacke/include/
LIBS = -lm -lgfortran
STATIC_LIB = $(LAPACK)/liblapacke.a $(LAPACK)/liblapack.a $(LAPACK)/librefblas.a

COBJ = mod.o numc.o layer.o rtf.o ioapi.o corr.o amp_corr.o
FOBJ = ttimesapi.o libtau.o libsun.o

all:amp_corr

rtf.o:rtf.c
	${CC} -c $^ -I ${INCLUDE} ${LIBS}

amp_corr: ${COBJ} ${FOBJ}
	${CC} $^ ${STATIC_LIB} ${LIBS} -I ${INCLUDE}  -o $@ 
	
%o.:%.f
	${FC} -c 

clean:
	rm -rvf *.o amp_corr
