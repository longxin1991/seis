#define DEG2RAD (180/PI)
#define PI 3.14159265358979311599796346854419
#include <math.h>


void linspace(double begin,double end,int n,double p[]);
int sign(double x);

int hv(double x);
void der(double y[],int n,double dt);
void expspace(double max,double min,int n,double p[]);
