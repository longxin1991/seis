#define MAXLEN 50000
#define VERBOSE 1

typedef struct raydata
{
	float *x;
	float *z;
	float t;
	float dis;
	int np;
	char phase;
}RayData;


/*Data type for constant horizontal layer model*/
typedef struct model{
	float *Vp;
	float *Vs;
	float *h;
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
