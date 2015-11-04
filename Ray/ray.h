#include "mem.h"

float *phase_parse(const char phase,Model mod);
void ray_tracing_m(struct raycor R,Model *mod, RayData *Data);
void ray_tracing_sp(struct raycor_sphere R,Model *mod,RayDataSP *Data);
void ray_tracing_ml(struct raycor R,Model *mod, RayData *Data);
RayData *ray_shoot(struct raycor R1,struct raycor R2,Model *mod,float dis,char *phase);
RayDataSP *ray_shoot_sp(struct raycor_sphere R1,struct raycor_sphere R2,Model *mod,float delta,char *phase);
int point_layer(float *h,int ln,float z0);
void depth_to_thickness(float *depth,float *d,int ln);
