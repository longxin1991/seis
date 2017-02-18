FC=gfortran
OS=$(shell uname)
ifeq ($(OS),Darwin)
	CC=gcc-5
else
	CC=gcc
endif
LIBS = -lm -lgfortran

COBJ = ioapi.o mod.c numc.o layer.c discorr.o
FOBJ = ttimesapi.o libtau.o libsun.o

discorr: ${COBJ} ${FOBJ}
	${CC} $^ ${LIBS} -o $@ 
	
%o.:%.f
	${FC} -c 

clean:
	rm -rvf *.o discorr
