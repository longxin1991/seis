typedef struct grid_index{
	int x;		
	int y;
}Ind;

void get_index(double r,double theta,int nth,int nr,float *h,Ind ind[2]);

double interp_sl(double uls,double lrs,double ur,double lr,double r,double *k);

double bilinear(double uls,double urs,double lls,double lrs,double ur,double lr,double lt,double rt,double r,double theta,double *kr,double *kt);

int incell(double ur,double lr,double lt,double rt,double r,double theta);

int pass_boundary(double rt,double theta,double hit_miss,double lmiss);
