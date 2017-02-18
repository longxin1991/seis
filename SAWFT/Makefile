CC=gcc
LIB := -lm  -lgsl -lgslcblas
CFLAGS := -O3 -Wall
OMP := true
OMPLIB := -fopenmp


OBJ := io.o sacio.o utils.o numc.o transform.o waveform.o attenuation.o sawft.o

all:sawft

sawft.o:sawft.c
ifeq ($(OMP),true)
	@echo "enable openmp."
	${CC} -c $^ ${OMPLIB}
else
ifeq ($(OMP),false)
		@echo "openmp is not enabled."
		${CC} -c $^
endif
endif

sawft:${OBJ}
ifeq ($(OMP),true)
	${CC} $^ ${LIB} ${OMPLIB} ${CFLAGS} -o $@
else
	${CC} $^ ${LIB} ${CFLAGS} -o $@
endif

clean:
	rm ${OBJ} sawft
