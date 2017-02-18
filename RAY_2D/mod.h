#define MAXLAYER 100
#define VERBOSE  0

/*Data type for constant horizontal layer model*/
typedef struct model{
	float *Vp;
	float *Vs;
	float *h;
	float *rho;
	float *Qp;
	float *Qs;
	int n;
}Model;

/* Define the model parameters at discontinuities. */
typedef struct discont{
	float radius;
	float vp1;
	float vp2;
	float vs1;
	float vs2;
}Disc;

void read_mod(char *model_file,Model *mod);

float prem_iso(float r,char *param,int idom);
void prem_disc(Disc **discont,int *ndisc);

void generate_rgrid(Disc *discont,int ndisc,float **h,float **vp,int *nr);

void mk_1d_prem(Model *mod);
