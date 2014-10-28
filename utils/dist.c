#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
void cal_distaz(double sla,double slo,double ela,double elo,distaz *res) 
{
   double stalat, stalon, evtlat, evtlon;
   double delta, az, baz;
   double scolat, slon, ecolat, elon;
   double a,b,c,d,e,aa,bb,cc,dd,ee,g,gg,h,hh,k,kk;
   double rhs1,rhs2,sph,rad,del,daz,dbaz,pi,piby2;


   stalat = sla;
   stalon = slo;
   evtlat = ela;
   evtlon = elo;

   if ((stalat == evtlat)&&(stalon == evtlon)) {
	   res->dis=0;
	   res->az=0;
	   res->baz=0;
   }

   pi=3.141592654;
   piby2=pi/2.0;
   rad=2.*pi/360.0;

   sph=1.0/298.257;

   scolat=piby2 - atan((1.-sph)*(1.-sph)*tan(stalat*rad));
   ecolat=piby2 - atan((1.-sph)*(1.-sph)*tan(evtlat*rad));
   slon=stalon*rad;
   elon=evtlon*rad;

   a=sin(scolat)*cos(slon);
   b=sin(scolat)*sin(slon);
   c=cos(scolat);
   d=sin(slon);
   e=-cos(slon);
   g=-c*e;
   h=c*d;
   k=-sin(scolat);

   aa=sin(ecolat)*cos(elon);
   bb=sin(ecolat)*sin(elon);
   cc=cos(ecolat);
   dd=sin(elon);
   ee=-cos(elon);
   gg=-cc*ee;
   hh=cc*dd;
   kk=-sin(ecolat);

   del=acos(a*aa + b*bb + c*cc);
   delta=del/rad;
   rhs1=(aa-d)*(aa-d)+(bb-e)*(bb-e)+cc*cc - 2.;
   rhs2=(aa-g)*(aa-g)+(bb-h)*(bb-h)+(cc-k)*(cc-k) - 2.;
   dbaz=atan2(rhs1,rhs2);
   if (dbaz<0.0) {
      dbaz=dbaz+2*pi;
   } 
   baz=dbaz/rad;

   rhs1=(a-dd)*(a-dd)+(b-ee)*(b-ee)+c*c - 2.;
   rhs2=(a-gg)*(a-gg)+(b-hh)*(b-hh)+(c-kk)*(c-kk) - 2.;
   daz=atan2(rhs1,rhs2);
   if(daz<0.0) {
      daz=daz+2*pi;
   }
   az=daz/rad;

   if(abs(baz-360.) < .00001) baz=0.0;
   if(abs(az-360.) < .00001) az=0.0;
   
   res->dis=delta;
   res->baz=baz;
   res->az=az;
}

void cal_rela_cor(double clat,double clon,double slat,double slon,cor *res)
{
	distaz dtz;
	double gcarc;
	cal_distaz(clat,clon,slat,slon,&dtz);
	gcarc=dtz.dis*111.13;
	res->x_pos = gcarc*sin(dtz.baz*PI/180);
	res->y_pos = gcarc*cos(dtz.baz*PI/180);
}