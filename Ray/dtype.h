#define MAXLEN 100000

typedef struct raydata
{
	float *x;
	float *z;
	float t;
	float dis;
	int np;
	char phase;
}RayData;

typedef struct model{
	float *Vp;
	float *Vs;
	float *h;
	int n;
}Model;

struct raycor{
	float rx;
	float rz;
	float ri;
};
