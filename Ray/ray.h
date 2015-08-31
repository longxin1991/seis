#include "mem.h"

float *phase_parse(const char phase,Model mod);
void ray_tracing(float i0,float x0,float z0,float v, RayData *Data);
void ray_tracing_m(struct raycor R,Model *mod, RayData *Data);
RayData *ray_shoot(struct raycor R1,struct raycor R2,Model *mod,float dis,char phase);
int point_layer(float *h,int ln,float z0);
void depth_to_thickness(float *depth,float *d,int ln);
