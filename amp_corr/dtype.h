#define MAXLEN 400000
#define VERBOSE 0
#define Re 6371.0

typedef struct raydata
{
	float *x;
	float *z;
	float t;
	float dis;
	int np;
	char *phase;
}RayData;

typedef struct raydata_sphere
{
	float *r;
	float *phi;
	float t;
	float delta;
	int np;
	char *phase;
}RayDataSP;

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

/*Data type for linear horiziontal layer model*/
typedef struct model_l{
	float *Vp;
	float *Vs;
	float *z;
	int n;
}Model_L;


struct raycor{
	float rx;
	float rz;
	float ri;
};

struct raycor_sphere{
	float r;
	float phi;
	float theta;
};

typedef struct moment_tensor{
	float mrr;
	float mtt;
	float mff;
	float mrt;
	float mrf;
	float mtf;
}CMT;
