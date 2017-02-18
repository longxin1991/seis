#include <sys/time.h>
#include <stdlib.h>

double dclock()
{
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp,&tzp);
	return((double)tp.tv_sec+(double)tp.tv_usec*1.e-6);
}

double get_random()
{
	return ((double)random()/(double)RAND_MAX);
}



