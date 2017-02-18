#include <string.h>

/* convert c string to fortran string */
void c2fstr(char *str,int size);

/* convert fortran string to c string */
void f2cstr(char *str,int size);

/* Get index of specific phase returned by ttimes */
int phase_id(char const *phase,char (*phcd)[8],int pn);

#define getttimes getttimes_
