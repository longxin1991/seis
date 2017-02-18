int layerxt(float p,float h,float utop,float ubot,float *dx,float *dt);
void flat_tf(float r,float v,float *zf,float *vf);
void depth_to_thickness(float *depth,float *d,int ln);
void cal_ic_time(float p,Model mod,float *t);
void layer_trace(Model mod,float p,float *t,float *x,int *m);
void ray_shoot(Model mod,float p1,float p2,float deg,float *t,float *p);
void devide_layer(float,float,float,int,float *);
float tstar_ic_l(float,Model,int);
void source_env(float,Model,int *,float *,float *);
