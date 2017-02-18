#include "ray.h"
#include "grid.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "numc.h"

//extern int turn,disc,cstep;
//extern float sigma,rdis,s1,s2;
int hetero,sampray;

int tracer2d_core(Ray2d rvar0,Ray2d *rvar1,int *turn0,int *disc0,int *cstep0,double *sigma0,double *rdis0,double *s10,double *s20,double step,double *time,int nth,int nr,float *h,double *sp)
{
	int stat=1,ulx,uly,urx,ury,llx,lly,lrx,lry;
	double r0,theta0,tr0,ttheta0,r,theta,tr,ttheta;
	double K1,K2,K3,K4,L1,L2,L3,L4,M1,M2,M3,M4,N1,N2,N3,N4,O1,O2,O3,O4;
	Ind id[2];
	double uls,urs,lls,lrs,s,s0,ur,lr,k,t=0,tmp,kr,kt,lt,rt,s3,miss,lmiss;
	double Nr,A,B,xi;
	int i=0,j=0;
	double hit_miss=0.0005,receiver_r=6371.0,dth,arclen,lt_arc,rt_arc,ddeg;
	int turn,disc,cstep,in,pass;
	double sigma,rdis,s1,s2,deg;

	turn = *turn0;
	disc = *disc0;
	cstep = *cstep0;
	rdis = *rdis0;
	s1 = *s10;
	s2 = *s20;
	sigma = *sigma0;

	r0 = rvar0.r;
	theta0 = rvar0.theta;
	tr0 = rvar0.tr;
	ttheta0 = rvar0.ttheta;
	dth = PI/nth;

	while (stat)
	{
		/*
		if(theta0 > PI)
		{
			break;
			stat = 2;
		}
		*/

		if (cstep == 0)
			sigma = step;

		if (turn==1 && fabs(r0 - receiver_r)<hit_miss)
		{
			stat = 2;
			break;
		}
		get_index(r0,theta0*DEG2RAD,nth,nr,h,id);
		ulx = id[0].x;
		uly = id[0].y;
		urx = id[1].x;
		ury = id[0].y;
		llx = id[0].x;
		lly = id[1].y;
		lrx = id[1].x;
		lry = id[1].y;
	
		uls = sp[uly*nth+ulx];
		urs = sp[ury*nth+urx];
		lls = sp[lly*nth+llx];
		lrs = sp[lry*nth+lrx];
		
		ur = 6371.0 - h[uly];
		lr = 6371.0 - h[lry];
		deg = theta0*DEG2RAD;
		lt = ulx*dth;
		rt = lrx*dth;

		//printf("lt->%f rt->%f deg->%f\n",lt,rt,theta0);
		
		if (turn == 0 && disc != 1)
		{
			if (h[lry+1] - h[lry]<0.1 && fabs(r0 - lr)>hit_miss)
			{
				disc = 1;
				//printf("lr->%.3f\n",lr);
				rdis = lr - 0.0001;
				s1 = lrs;
				s2 = sp[(lry+1)*nth+lrx];
			}
			else
				disc = 0;
		}
		if (turn == 1 && disc != 1 && uly>1)
		{
			if (h[uly] - h[uly-1]<0.1 && fabs(r0 - ur)>hit_miss)	
			{
				disc = 1;
				//printf("ur->%.3f,turn->%d\n",ur,turn);
				//printf("%.3f,%d,%.3f,%d\n",h[uly],uly,h[uly-1],uly-1);
				rdis = ur + 0.0001;
				s1 = uls;
				s2 = sp[(uly-1)*nth+ulx];
			}
			else
				disc = 0;
		}
		//printf("%d %d; %d %d\n",id[0].x,id[0].y,id[1].x,id[1].y);
		
		//break;
		if (hetero == 0)
		{
			s0 = interp_sl(uls,lrs,ur,lr,r0,&kr);
			//s3 = bilinear(uls,urs,lls,lrs,ur,lr,lt,rt,r0,theta0,&kr,&kt);
			//if (r0<3700 && r0>3500 && theta0>PI/2.0)
			//	printf("r->%.3f,s->%f\n",r0,s0);
			//printf("%f %f %f\n",s0-s3,kr-k,kt);
			kt = 0;
		}
		else
		{
			s0 = bilinear(uls,urs,lls,lrs,ur,lr,lt,rt,r0,theta0,&kr,&kt);
			//if (r0<3700 && r0>3500 && theta0>PI/2.0)
			//	printf("r->%.3f,s->%f\n",r0,s0);
		}
		//printf("%f %f %f\n",s0-s3,kr-k,kt);
		//exit(0);
		//printf("%.10f %.10f; %.10f %.10f\n%.10f\n",uls,urs,lls,lrs,s0);
		K1 = tr0;
		L1 = ttheta0/(r0*r0);
		M1 = 0.5*kr + (s0 - tr0*tr0)/r0;
		N1 = 0.5*kt;
		O1 = s0;

		/*----------------------1------------------------------*/
		r = r0 + 0.5*sigma*K1;
		theta = theta0 + 0.5*sigma*L1;
		tr = tr0 + 0.5*sigma*M1;
		ttheta = ttheta0 + 0.5*sigma*N1;
		
		ddeg = r*2*PI/360.0;
		arclen = theta*ddeg;
		lt_arc = lt*ddeg;
		rt_arc = rt*ddeg;

		if (turn==1 && (r - receiver_r)>hit_miss)
		{
			sigma /= 2.0;
			cstep = 1;
			continue;
		}
		
		in = incell(ur,lr,lt,rt,r,theta);
		lmiss = fabs(arclen - rt_arc);
		pass = pass_boundary(rt,theta,hit_miss,lmiss);
		if (in == 0 && pass ==0)
		{
			if (disc==1)
			{
				sigma /= 2.0;
				cstep = 1;
				continue;
			}
			
			if (disc !=1)
			{
				if (turn==0)
					miss = fabs(r-lr);
				else
					miss = fabs(r-ur);


				if (miss>hit_miss)
				{
					sigma /= 2.0;
					//	printf("r->%f,sigma->%f,ur->%f,lr->%f,miss->%f,lmiss->%f,pass->%d\n",r,sigma,ur,lr,miss,lmiss,pass);
					cstep = 1;
					continue;
				}
				else
					cstep = 0;
			}
		}
		
		if (pass)
		{
			sigma /= 2.0;
			cstep = 1;
			//printf("r->%f,sigma->%f,ur->%f,lr->%f,miss->%f,lmiss->%f,pass->%d\n",r,sigma,ur,lr,miss,lmiss,pass);
			continue;
		}

			get_index(r,theta*DEG2RAD,nth,nr,h,id);
		    ulx = id[0].x;
		    uly = id[0].y;
		    urx = id[1].x;
		    ury = id[0].y;
		    llx = id[0].x;
		    lly = id[1].y;
		    lrx = id[1].x;
		    lry = id[1].y;
	
		    uls = sp[uly*nth+ulx];
		    urs = sp[ury*nth+urx];
		    lls = sp[lly*nth+llx];
		    lrs = sp[lry*nth+lrx];
		
		    ur = 6371.0 - h[uly];
		    lr = 6371.0 - h[lry];
			lt = ulx*dth;
			rt = lrx*dth;
		//if (fabs(r-lr)<hit_miss || fabs(r-ur)<hit_miss)
		//	cstep = 0;
		if (hetero == 0)
		{
			s = interp_sl(uls,lrs,ur,lr,r,&kr);
			kt = 0;
		}
		else
			s = bilinear(uls,urs,lls,lrs,ur,lr,lt,rt,r,theta,&kr,&kt);
		K2 = tr;
		L2 = ttheta/(r*r);
		M2 = 0.5*kr + (s - tr*tr)/r;
		N2 = 0.5*kt;
		O2 = s;

		/*------------------------2-------------------------*/
		r = r0 + 0.5*sigma*K2;
		theta = theta0 + 0.5*sigma*L2;
		tr = tr0 + 0.5*sigma*M2;
		ttheta = ttheta0 + 0.5*sigma*N2;

		ddeg = r*2*PI/360.0;
		arclen = theta*ddeg;
		lt_arc = lt*ddeg;
		rt_arc = rt*ddeg;

		if (turn==1 && (r - receiver_r)>hit_miss)
		{
			sigma /= 2.0;
			cstep = 1;
			continue;
		}

		in = incell(ur,lr,lt,rt,r,theta);
		lmiss = fabs(arclen - rt_arc);
		pass = pass_boundary(rt,theta,hit_miss,lmiss);
		if (in == 0)
		{
			if (disc==1)
			{
				sigma /= 2.0;
				cstep = 1;
				continue;
			}
			
			if (disc !=1)
			{
				if (turn==0)
					miss = fabs(r-lr);
				else
					miss = fabs(r-ur);

				if (miss>hit_miss)
				{
					sigma /= 2.0;
					//printf("test,diff->%f,sigma->%f\n",r-lr,sigma);
					cstep = 1;
					continue;
				}
				else
					cstep = 0;
			}	
		}
		if (pass)
		{
			sigma /= 2.0;
			cstep = 1;
			continue;
		}
			get_index(r,theta*DEG2RAD,nth,nr,h,id);
		    ulx = id[0].x;
		    uly = id[0].y;
		    urx = id[1].x;
		    ury = id[0].y;
		    llx = id[0].x;
		    lly = id[1].y;
		    lrx = id[1].x;
		    lry = id[1].y;
	
		    uls = sp[uly*nth+ulx];
		    urs = sp[ury*nth+urx];
		    lls = sp[lly*nth+llx];
		    lrs = sp[lry*nth+lrx];
		
		    ur = 6371.0 - h[uly];
		    lr = 6371.0 - h[lry];
			lt = ulx*dth;
			rt = lrx*dth;
		//if (fabs(r-lr)<hit_miss || fabs(r-ur)<hit_miss)
		//	cstep = 0;
		if (hetero == 0)
		{
			s = interp_sl(uls,lrs,ur,lr,r,&kr);
			kt = 0;
		}
		else
			s = bilinear(uls,urs,lls,lrs,ur,lr,lt,rt,r,theta,&kr,&kt);
		K3 = tr;
		L3 = ttheta/(r*r);
		M3 = 0.5*kr + (s - tr*tr)/r;
		N3 = 0.5*kt;
		O3 = s;

		/*------------------------3------------------------*/
		r = r0 + sigma*K3;
		theta = theta0 + sigma*L3;
		tr = tr0 + sigma*M3;
		ttheta = ttheta0 + sigma*N3;

		ddeg = r*2*PI/360.0;
		arclen = theta*ddeg;
		lt_arc = lt*ddeg;
		rt_arc = rt*ddeg;

		if (turn==1 && (r - receiver_r)>hit_miss)
		{
			sigma /= 2.0;
			cstep = 1;
			continue;
		}

		in = incell(ur,lr,lt,rt,r,theta);
		lmiss = fabs(arclen - rt_arc);
		pass = pass_boundary(rt,theta,hit_miss,lmiss);
		if (in == 0)
		{
			if (disc==1)
			{
				sigma /= 2.0;
				cstep = 1;
				continue;
			}

			if (disc !=1)
			{
				if (turn==0)
					miss = fabs(r-lr);
				else
					miss = fabs(r-ur);

				if (miss>hit_miss)
				{
					sigma /= 2.0;
					//printf("test,diff->%f,sigma->%f\n",r-lr,sigma);
					cstep = 1;
					continue;
				}
				else
					cstep = 0;
			}
		}
		if (pass)
		{
			sigma /= 2.0;
			cstep = 1;
			continue;
		}
			get_index(r,theta*DEG2RAD,nth,nr,h,id);
		    ulx = id[0].x;
		    uly = id[0].y;
		    urx = id[1].x;
		    ury = id[0].y;
		    llx = id[0].x;
		    lly = id[1].y;
		    lrx = id[1].x;
		    lry = id[1].y;
	
		    uls = sp[uly*nth+ulx];
		    urs = sp[ury*nth+urx];
		    lls = sp[lly*nth+llx];
		    lrs = sp[lry*nth+lrx];
		
		    ur = 6371.0 - h[uly];
		    lr = 6371.0 - h[lry];
			lt = ulx*dth;
			rt = lrx*dth;
		//if (fabs(r-lr)<hit_miss || fabs(r-ur)<hit_miss)
		//	cstep = 0;
		if (hetero == 0)
		{
			s = interp_sl(uls,lrs,ur,lr,r,&kr);
			kt = 0;
		}
		else
			s = bilinear(uls,urs,lls,lrs,ur,lr,lt,rt,r,theta,&kr,&kt);
		K4 = tr;
		L4 = ttheta/(r*r);
		M4 = 0.5*kr + (s - tr*tr)/r;
		N4 = 0.5*kt;
		O4 = s;

		t = sigma*(O1 + 2*O2 + 2*O3 + O4)/6.0;
		tmp = sigma*(K1 + 2*K2 + 2*K3 + K4)/6.0;
		
		if (turn == 0)
		{
			if (tmp<0)
				turn = 0;
			else
			{
				if ( disc == 1)
					disc = 0;
				turn = 1;
			}
		}

		r0 += tmp;
		//printf("%.10f, %.1f\n",tmp,sigma);
		//
		if (turn==1 && (r0 - receiver_r)>hit_miss)
		{
			r0 -= tmp;
			sigma /= 2.0;
			cstep = 1;
			continue;
		}

		if (disc==1 && (r0>ur || r0<lr) )
		{
			sigma /= 2.0;
			cstep = 1;
			r0 -= tmp;
			continue;
		}
		theta0 += sigma*(L1 + 2*L2 + 2*L3 + L4)/6.0;
		tr0 += sigma*(M1 + 2*M2 + 2*M3 + M4)/6.0;

		if (disc==1 && fabs(r0-rdis)<hit_miss)
		{
			r0 = rdis;
			Nr = 1.0;
			B = tr0;
			xi = sign(B);
			tr0 = tr0 - (B-xi*sqrtf(s2 - s1 + B*B))*Nr;
			disc = 0;
			cstep = 0;
			//j += 1;
			//printf("j->%d, disc->%d,r->%.3f,rdis->%.3f\n",j,disc,r0,rdis);
			continue;
		}
		//printf("j->%d, disc->%d,r->%.3f,theta->%.3f,t->%.3f,rdis->%.3f,sigma->%f,turn->%d\n",j,disc,r0,theta0*DEG2RAD,t,rdis,sigma,turn);
		//j += 1;

		cstep = 0;
		stat = 0;
	}
	rvar1->r = r0;
	rvar1->tr = tr0;
	rvar1->theta = theta0;
	rvar1->ttheta = ttheta0;
	*time = t;
	*turn0 = turn;
	*disc0 = disc;
	*cstep0 = cstep;
	*sigma0 = sigma;
	*rdis0 = rdis;
	*s10 = s1;
	*s20 = s2;
	return stat;
}

void ray_tracer2d(Ray2d rvar0,int nth,int nr,float *h,double *sp,double *t,double *deg,Raysamp *rsamp)
{
	Ray2d rvar1,rvar2,rvartmp,rvar_half;
	double dt=0,time=0,max_step=20.0,step1,step2,dt1=0,dt2=0,diff=0,time_precise=0.1,t_tmp,t_half=0;
	int stat,i=0,j=0,k=1,l=0,stat1,stat2,recal=0;
	int turn1=0,disc1=0,cstep1=0,turn2=0,disc2=0,cstep2=0;
	double sigma1,s11,s21,rdis1,sigma2,s12,s22,rdis2;
	int oturn,odisc,ocstep,turn_half,disc_half,cstep_half;
	double osigma,os1,os2,ordis,sigma_half,s1_half,s2_half,rdis_half,dth;

	dth = 180.0/nth;

	if (sampray == 1)
	{
		rsamp->r[0] = rvar0.r;
		rsamp->theta[0] = rvar0.theta;
	}
	step1 = max_step;
	step2 = step1/2.0;
		
	do
	{
		oturn = turn1;
		odisc = disc1;
		ocstep = cstep1;
		osigma = sigma1;
		ordis = rdis1;
		os1 = s11;
		os2 = s21;

		if (recal == 0 )
			stat1 = tracer2d_core(rvar0,&rvar1,&turn1,&disc1,&cstep1,&sigma1,&rdis1,&s11,&s21,step1,&dt1,nth,nr,h,sp);
		t_tmp = 0;
		//if (i==11401)
		//	break;
		//printf("test\n");
		/*
		rvartmp = rvar0;
		for (j=0;j<2;j++)
		{
			stat2 = tracer2d_core(rvar0,&rvar2,&turn2,&disc2,&cstep2,&sigma2,&rdis2,&s12,&s22,step2,&dt2,nth,nr,h,sp);
			rvar0 = rvar2;
			if (j == 0)
			{
				t_half = dt2;
				rvar_half = rvar2;
				turn_half = turn2;
				disc_half = disc2;
				cstep_half = cstep2;
				sigma_half = sigma2;
				rdis_half = rdis2;
				s1_half = s12;
				s2_half = s22;
			}
			t_tmp += dt2;
		}
		dt2 = t_tmp;
		
		rvar0 = rvartmp;
		diff = fabs(dt1-dt2);
		recal = 0;
		*/
		//break;
		/*
		if (diff > time_precise)
		{
			//printf("i->%d,l->%d,step->%.3f,diff->%.12f,stat1->%d,stat2->%d\n",i,l,step1,diff,stat1,stat2);

			recal = 1;
			dt1 = t_half;
			rvar1 = rvar_half;
			step1 = step2;
			turn1 = turn_half;
			disc1 = disc_half;
			cstep1 = cstep_half;
			sigma1 = sigma_half;
			rdis1 = rdis_half;
			s11 = s1_half;
			s21 = s2_half;

			turn2 = oturn;
			disc2 = odisc;
			cstep2 = ocstep;
			sigma2 = osigma;
			rdis2 = ordis;
			s12 = os1;
			s22 = os2;
			step2 /= 2.0; 
			l += 1;
			continue;
		}
		*/
		//printf("i->%d,l->%d,step->%.3f,diff->%.12f,stat1->%d,stat2->%d\n",i,l,step1,diff,stat1,stat2);
		//while (diff>time_precise)
		//{
		//	step1 = step2/2.0;
		//	stat1 = tracer2d_core(rvar0,&rvar1,step1,&dt1,nth,nr,h,sp);
				//printf("i->%d,j->%d\n",i,j);
		//}
		stat = stat1;
		dt = dt1;
		time += dt; 
		rvar0 = rvar1;
		//printf("i->%d,stat->%d,r->%.3f,theta->%.3f,t->%.3f\n",i,stat,rvar0.r,rvar0.theta*DEG2RAD,time);
		l=0;	
		if (sampray == 1)
		{
			if (fabs(rvar0.theta*DEG2RAD - dth*k) < 0.1)
			{
				rsamp->r[k] = rvar0.r;
				rsamp->theta[k] = rvar0.theta;
				k = k + 1;
			}
		}
		//printf("i->%d,step->%.3f,diff->%f,stat1->%d,stat2->%d,j->%d\n",i,step1,diff,stat1,stat2,j);
		i++;
		step1 = max_step;
		step2 = step1/2.0;
	}while(stat==0);
	//printf("i->%d,stat->%d\n",i,stat);

	if ( sampray == 1)
	{
		rsamp->n = k;
		rsamp->r[k] = rvar0.r;
		rsamp->theta[k] = rvar0.theta;
	}
	//printf("i->%d,stat->%d,r->%.3f,theta->%.3f,t->%.3f\n",i,stat,rvar0.r,rvar0.theta*DEG2RAD,time);
	*t = time;
	*deg = rvar0.theta*DEG2RAD;
}

int ray_shoot(double delta1,double delta2,int nth,int nr,float *h,double *sp,double deg,double *t,double *delta,Raysamp *rsamp)
{
	double delta0,deg1,deg2,deg0,t0,t1,t2,shoot_precise=0.005;
	Ray2d rvar0;
	int i,stat;

	if (delta1 > delta2)
	{
		printf("delta1 is larger than delta2\n");
		exit(0);
	}
	
	ray_init(delta1,&rvar0);
	ray_tracer2d(rvar0,nth,nr,h,sp,&t1,&deg1,rsamp);

	ray_init(delta2,&rvar0);
	ray_tracer2d(rvar0,nth,nr,h,sp,&t2,&deg2,rsamp);
	if (deg1 < deg2)
	{
		printf("deg1 is smaller than deg2\n");
		exit(0);
	}
	else if (deg>deg1 || deg<deg2)
	{
		printf("object outside shooting range.\n");
		exit(0);
	}
	
	deg0 = 0.0;
	i = 0;
	stat = 0;
	while (fabs(deg0 - deg)>=shoot_precise)
	{
		delta0 = (delta1 + delta2)/2.0;
		ray_init(delta0,&rvar0);
		ray_tracer2d(rvar0,nth,nr,h,sp,&t0,&deg0,rsamp);
		if (deg0 >= deg)
			delta1 = delta0;
		else
			delta2 = delta0;
		i += 1;
		if (i>15)
		{
			stat = -1;
			*delta = delta0;
			break;
		}
		//printf("step->%d, deg0->%f, t0->%f\n",i,deg0,t0);
	}
	*t = t0;
	*delta = delta0;
	return stat;
}

void ray_init(double delta,Ray2d *rvar0)
{
	double delta0;
	delta0 = PI - delta/DEG2RAD;
	rvar0->r = 6371.0;
	rvar0->theta = 0.0;
	rvar0->tr = cosf(delta0)/5.8;
	rvar0->ttheta = rvar0->r * sinf(delta0)/5.8;
}
