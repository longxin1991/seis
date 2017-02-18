typedef struct ray2dvar{
	double tr;
	double ttheta;
	double r;
	double theta;
}Ray2d;

typedef struct raysample{
	double *r;
	double *theta;
	int n;
}Raysamp;

void ray_tracer2d(Ray2d rvar0,int nth,int nr,float *h,double *sp,double *,double *,Raysamp *);
int tracer2d_core(Ray2d rvar0,Ray2d *rvar1,int *,int *,int *,double *,double *,double *,double *,double step,double *time,int nth,int nr,float *h,double *sp);

int ray_shoot(double delta1,double delta2,int nth,int nr,float *h,double *sp,double deg,double *t,double *i,Raysamp *rsamp);
void ray_init(double delta,Ray2d *rvar0);
