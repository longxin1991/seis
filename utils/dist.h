#define PI 3.14159265357
typedef struct{
	double dis;
	double baz;
	double az;
}distaz;

typedef struct{
	double x_pos;
	double y_pos;
}cor;

void cal_distaz(double sla,double slo,double ela,double elo,distaz *res);
void cal_rela_cor(double clat,double clon,double slat,double slon,cor *res);
