#define COL_MAJOR 1
#define ROW_MAJOR 0
/*define property structure of the interface*/
typedef struct interface{
	float a1;	/*P wave velocity of upper layer.*/
	float a2;	/*P wave velocity of lower layer.*/
	float p1;	/*density of upper payer*/
	float p2;	/*density of lower layer*/
	float b1;	/*S wave velocity of upper layer*/
	float b2;	/*S wave velocity of lower layer*/
}ITF;

typedef struct rtcoeff_p{
	float p1p2;
	float p1s1;
	float p1p1;
	float p1s2;
}RTF_P;
