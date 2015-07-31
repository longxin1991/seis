#include <stdlib.h>
#include "dtype.h"

void free_path(RayData *Data){
	free(Data->x);
	free(Data->z);
}
