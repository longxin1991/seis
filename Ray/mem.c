#include <stdlib.h>
#include "dtype.h"

void free_path(RayData *Data){
	free(Data->x);
	free(Data->z);
}

void free_path_sp(RayDataSP *Data){
	free(Data->r);
	free(Data->phi);
}
