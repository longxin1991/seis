#include "mem.h"
#include "struct.h"
#include "numc.h"
#include "velocity.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define t_pre 0.01
float *phase_parse(char *phase,Model *mod)
{
    float *V;
    switch(phase[0]){
        case 'S':
            V = mod->Vs;
            break;
        case 'P':
            V = mod->Vp;
            break;
        default:
            printf("only parse P or S phase.\n");
            exit(0);
    }
    return V;
}

int point_layer(float *h,int ln,float z0)
{
    int i=0;
    float dep=0;
    for(i=0;i<ln;i++)
    {
        dep += h[i];
        if (z0<=dep)
        {   
            break;
        }
    }
    return i;
}

void depth_to_thickness(float *depth,float *d,int ln)
{
    int i;
    for (i=0;i<ln;i++)
        {
            d[i] = depth[i+1] - depth[i];
        }
}

float check_precision(float r,float theta,float v,float p)
{
	return fabs(p - r*sin(theta)/v);
}


void ray_cal_core(){
	/*使用R-K方法计算射线的核心程序部分*/

;

}



void ray_tracing_sp(struct raycor_sphere R,Model *mod,RayDataSP *Data)
{
	int k=0,i=0,j,turn=0,n,ln,sn,bn;
	/*take arc length s as monotone varialle of ray*/
	float ds=1.0;
	float K1,K2,L1,L2,M1,M2,N1,N2;
	float p,*r,*phi;
	float *h,*v,r0,v0,v1,r1,*d,t,theta,theta1,sl,diff;
	float r2,v2,r3,v3,theta2,theta3,K3,K4,L3,L4,M3,M4,N3,N4;
	float t_temp[2],theta_temp;
	int ref_code,info;

	t_temp[0] = 0;
	t_temp[1] = 100;

	r = (float *)malloc(MAXLEN*sizeof(float));
	phi = (float *)malloc(MAXLEN*sizeof(float));

	Data->r = r;
	Data->phi = phi;
	h = mod->h;
	n = mod->n;
	ln = n - 1;
    /* Array of layer thickness*/
    d = (float *)malloc((n-1)*sizeof(float));

	depth_to_thickness(h,d,ln);

	v = phase_parse(Data->phase,mod);

	r[0] = R.r;
	phi[0] = R.phi;
	theta = R.theta;
	t = 0;

	sn = point_layer(d,ln,Re - r[0]);
	v0 = vl(v[sn],v[sn+1],h[sn],h[sn+1],Re - r[0]);
	p = r[0]*sin(theta)/v0;
	
	if (strcmp(Data->phase,"PcP")==0)
		ref_code = 50;
	else if (strcmp(Data->phase,"PKiKP")==0)
		ref_code = 74;
	else
		ref_code = ln;

	/*Downgoing part*/
	for (i=sn;i<ln;i++)
	{	if (d[i]<0.01)
		{
			theta = asin(sin(theta)*v[i+1]/v[i]);
			continue;
		}
		if(theta >= PI/2)
		{
			bn = i - 1;
			break;
		}
		
		if(turn == 1){
			break;
		}
		while(k < MAXLEN)
		{
			if((Re-r[k])>=h[ref_code] && turn == 0)
			{
				r[k] = Re - h[ref_code];
				bn = ref_code -1;
				turn = 1;
				break;
			}
			
			if((Re-r[k])>=h[i+1])
			{
				/*Bug 射线hit每一层不准确，对PKiKP累积走时误差可到0.3s,待修正。*/
				//printf("%f\n",Re -r[k] -h[i+1]);
				r[k] = Re - h[i+1];
				break;
			}

			if (theta >= PI/2.0)
				break;
			else
			{	
				v0 = vl(v[i],v[i+1],h[i],h[i+1],Re - r[k]);
				diff = check_precision(r[k],theta,v0,p);

				sl = -1*(v[i+1]-v[i])/(h[i+1]-h[i]);
				K1 = sin(theta)/v0*(v0/r[k] - sl);
				L1 = -1*cos(theta);
				M1 = sin(theta)/r[k];
				N1 = 1/v0;
				if(isnan(K1)||isnan(M1)){
					K1 = 0;
					M1 = 0;
				}
				
				while(fabs(t_temp[0]-t_temp[1]) > t_pre){
						//printf("t1->%f t2->%f biject s\n",t_temp[0],r[k]);
				for (j=0;j<=1;j++){

				ds = ds/(j+1);

				r1 = r[k]+0.5*ds*L1;
				v1 = vl(v[i],v[i+1],h[i],h[i+1],Re - r1);
				theta1 = theta+0.5*ds*K1;
				K2 = sin(theta1)/v1*(v1/r1 - sl);
				L2 = -1*cos(theta1);
				M2 = sin(theta1)/r1;
				N2 = 1/v1;
				if (isnan(K2)||isnan(M2)){
					K2 = 0;
					M2 = 0;
				}
				
				theta2 = theta + 0.5*ds*K2;
				r2 = r[k]+0.5*ds*L2;
				v2 = vl(v[i],v[i+1],h[i],h[i+1],Re - r2); 
				K3 = sin(theta2)/v2*(v2/r2 - sl);
				L3 = -1*cos(theta2);
				M3 = sin(theta2)/r2;
				N3 = 1/v2;
				if (isnan(K3)||isnan(M3)){
					K3 = 0;
					M3 = 0;
				}

				theta3 = theta + ds*K3;
				r3 = r[k]+ds*L3;
				v3 = vl(v[i],v[i+1],h[i],h[i+1],Re - r3);
				K4 = sin(theta3)/v3*(v3/r3 - sl);
				L4 = -1*cos(theta3);
				M4 = sin(theta3)/r3;
				N4 = 1/v3;
				if (isnan(K4)||isnan(M4)){
					K4 = 0;
					M4 = 0;
				}

				theta_temp = ds/6.0*(K1+2*K2+2*K3+K4);
				r[k+1] = r[k] + ds/6.0*(L1+2*L2+2*L3+L4);
				phi[k+1] = phi[k] + ds/6.0*(M1+2*M2+2*M3+M4);
				t_temp[j] = ds/6.0*(N1+2*N2+2*N3+N4);

				}
				}

				t += ds/6.0*(N1+2*N2+2*N3+N4);
				theta += theta_temp;
				t_temp[0] = 0;
				t_temp[1] = 100;
				
			}
			ds = 1.0;
			k += 1;
			//printf("%f %f %f %f %f\n",Re-r[k],phi[k]*180/PI,theta,t,diff);
		}
	}
	//printf("k->%d\n",k);
	//exit(0);
	/*Upgoing part.*/
	for(i=bn;i>=0;i--)
	{
		if(d[i]<0.01)
		{
			theta = asin(sin(theta)*v[i]/v[i+1]);
			/*数值不稳定或者精度问题造成的过临界角出射
			 *如在近180度震中距的PKPab相可能无法折回，
			 *此时射线在核幔边界的出射角近90度，有必要
			 *将theta校正为PI/2。
			 * */
			if(isnan(theta))
				theta = PI/2.0;
			continue;
		}
		while(k < MAXLEN)
		{
			if((Re - r[k])<=h[i])
			{
				r[k] = Re - h[i];
				break;
			}

			v0 = vl(v[i],v[i+1],h[i],h[i+1],Re - r[k]);

			if(v0<0.00001)
				break;

			sl = -1*(v[i+1]-v[i])/(h[i+1]-h[i]);
			K1 = sin(theta)/v0*(sl - v0/r[k]);
			L1 = 1*cos(theta);
			M1 = sin(theta)/r[k];
			N1 = 1/v0;

			if(isnan(K1)||isnan(M1)){
				K1 = 0.0;
				M1 = 0.0;
			}
				diff = check_precision(r[k],theta,v0,p);
				while(fabs(t_temp[0]-t_temp[1]) > t_pre){
					//printf("t1->%f t2->%f biject s\n",t_temp[0],r[k]);
					for (j=0;j<=1;j++){
				
						ds = ds/(j+1);
				r1 = r[k]+0.5*ds*L1;
				v1 = vl(v[i],v[i+1],h[i],h[i+1],Re - r1);
				theta1 = theta+0.5*ds*K1;
				K2 = sin(theta1)/v1*(sl - v1/r1);
				L2 = 1*cos(theta1);
				M2 = sin(theta1)/r1;
				N2 = 1/v1;
				

				theta2 = theta + 0.5*ds*K2;
				r2 = r[k]+0.5*ds*L2;
				v2 = vl(v[i],v[i+1],h[i],h[i+1],Re - r2); 
				K3 = sin(theta2)/v2*(sl - v2/r2);
				L3 = 1*cos(theta2);
				M3 = sin(theta2)/r2;
				N3 = 1/v2;

				theta3 = theta + ds*K3;
				r3 = r[k]+ds*L3;
				v3 = vl(v[i],v[i+1],h[i],h[i+1],Re - r3);
				K4 = sin(theta3)/v3*(sl - v3/r3);
				L4 = 1*cos(theta3);
				M4 = sin(theta3)/r3;
				N4 = 1/v3;

				theta_temp = ds/6.0*(K1+2*K2+2*K3+K4);
				r[k+1] = r[k] + ds/6.0*(L1+2*L2+2*L3+L4);
				phi[k+1] = phi[k] + ds/6.0*(M1+2*M2+2*M3+M4);
				t_temp[j] = ds/6.0*(N1+2*N2+2*N3+N4);

					}
				}

				t += ds/6.0*(N1+2*N2+2*N3+N4);
				theta += theta_temp;
				t_temp[0] = 0;
				t_temp[1] = 100;

			k += 1;
			ds = 1.0;
			//printf("%f %f %f %f %f\n",Re - r[k],phi[k]*180/PI,theta,t,diff);
		}	
	}
	//printf("k->%d\n",k);
	//exit(0);
	Data->delta = phi[k]*180/PI;
	Data->t = t;
	Data->np = k;
	//printf("%f %f %d\n",phi[k]*180/PI,t,k);
}

void ray_tracing_ml(struct raycor R,Model *mod, RayData *Data)
{

    int k=0,i=0,turn=0,n,ln,sn,bn;
    float dt=0.1;
    float p,K1,K2,*x,*z,z_ref,ph;
    float *h,*v,i0,*d,z1,v1,x1,v0,t;

    x = (float *)malloc(MAXLEN*sizeof(float));
    z = (float *)malloc(MAXLEN*sizeof(float));

    Data->x = x;
    Data->z = z;
    h = mod->h;
    n = mod->n;
    ln = n - 1;

    /* Array of layer thickness*/
    d = (float *)malloc((n-1)*sizeof(float));

    depth_to_thickness(h,d,ln);

    v = phase_parse(Data->phase,mod);

    x[0] = R.rx;
    z[0] = R.rz;
    i0 = R.ri;

    sn = point_layer(d,ln,z[0]);
    v0 = vl(v[sn],v[sn+1],h[sn],h[sn+1],z[0]);
    //printf("%.2f\n", v0);
    //exit(0);
    p = sin(i0)/v0;
    

    /*Tracing the downgoing part of the ray*/
    for (i=sn;i<ln;i++)
    {
        if(d[i]<0.01)
            continue;
		if(p*p*v0*v0>=0.999999)
		{
			bn=i-1;
			break;
		}

        while(k < MAXLEN)
        {
            //printf("%d\n", k);
            if (z[k]>=h[ln] && turn ==0)
            {
                z[k] = h[ln];
                turn = 1;
                //k += 1;
				bn=ln-1;
                break;
            }

            if (z[k]>=h[i+1])
            {
                z[k] = h[i+1];
                //k += 1;
                break;
            }
            //printf("%.2f %.2f %.2f %.6f %d\n",x[k],z[k],v0,p*p*v0*v0,i);
			/*Decide whether the critical angle is surpassed*/
			if (p*p*v0*v0>0.999999)
				break;
			else
			{
				v0 = vl(v[i],v[i+1],h[i],h[i+1],z[k]);
				K1 = v0*sqrt(1-p*p*v0*v0);
			}
            z1 = 0.5*dt*K1 + z[k];
            v1 = vl(v[i],v[i+1],h[i],h[i+1],z1);
            K2 = v1*sqrt(1-p*p*v1*v1);
            z[k+1] = z[k] + dt*K2;

            K2 = v1*v1*p;
            x[k+1] = x[k] + dt*K2;

            k += 1;

        }


    }

    /*Tracing the upgoing part of the ray*/
    for (i=bn;i>=0;i--)
    {
        if(d[i]<0.01)
            continue;

        while(k<MAXLEN)
        {
            if(z[k]<=h[i] && i>=1)
            {
                z[k] = h[i];
                //k += 1;
                //printf("%s %.2f %.2f\n","next", z[k],d[i]);
                break;
            }

            if(z[k]<=0)
                break;
            //printf("%.2f %.2f %.2f %.4f d\n",x[k],z[k],p*p*v0*v0,K1);
            v0 = vl(v[i],v[i+1],h[i],h[i+1],z[k]);

            /*encounter water, S wave velocity equals zero.*/
            if (v0<0.001)
                break;

			if (p*p*v0*v0<1.0)
				K1 = -1*v0*sqrt(1-p*p*v0*v0);
			else
				K1 = -K1;
            z1 = 0.5*dt*K1 + z[k];
            v1 = vl(v[i],v[i+1],h[i],h[i+1],z1);
            K2 = -1*v1*sqrt(1-p*p*v1*v1);
            z[k+1] = z[k] + dt*K2;

            K2 = v1*v1*p;
            x[k+1] = x[k] + dt*K2;

            k += 1;            
        }

    }

    Data->dis = x[k-1];
    t = dt*k;
    Data->np =k;
    Data->t = t;
	//printf("%.2f\n",x[k-1]);
}

void ray_tracing_m(struct raycor R,Model *mod, RayData *Data){

    int k=0,i=0,turn=0,n,sn,cn=100,cnk,pass_slab=0,dip=0;
    float dt = 0.1,t,dep=0,slab_dep;
    float a,p,K1,K2,*x,*z,h_cmb=0,th=PI/6.0,ph,cdep,diff;
    float *Vp,*Vs,*h,*V,i0;

    x = (float *)malloc(MAXLEN*sizeof(float));
    z = (float *)malloc(MAXLEN*sizeof(float));

    /*preparation for the ray tracing*/
    Data->x = x;
    Data->z = z;
    h = mod->h;
    n = mod->n;
    //Vp = mod->Vp;
    //Vs = mod->Vs;
    V  = phase_parse(Data->phase,mod);
    //V = Vs;
    for(i=0;i<n;i++)
        h_cmb += h[i];
    x[0] = R.rx;
    z[0] = R.rz;
    i0 = R.ri;
    sn = point_layer(h,n,z[0]);/*get the layer number where the source is in*/
    for (i=1;i<=sn;i++)
    {
        dep += h[i-1];
    }
    p = sin(i0)/V[0];

    /*------------------------------*/
    /*tracing the downgoing part of the ray*/
    for (i=sn;i<n;i++){

        dep += h[i];

    while (k<MAXLEN)
    {
        if (k==0)
        {
            K1 = V[i]*p;
            K2 = V[i]*p;
            x[1] = x[0] + dt*K2;

            K2 = V[i]*sqrt(1-p*p*V[i]*V[i]);
            z[1] = z[0] + dt*K2;
            k = k+1;
            continue;
        }
        
            K1 = V[i]*p;
            K2 = V[i]*p;
            x[k] = x[k-1] + dt*K2;
            
            K2 = V[i]*sqrt(1-p*p*V[i]*V[i]);
            
            if (z[k-1]>=h_cmb && turn ==0)
            {
                z[k-1]=h_cmb;
                turn=1;
                k += 1;
                break;
            }
            /*
            if (turn==1)
                K2=-1*K2;
            */
            z[k] = z[k-1] + dt*K2;
            
           if (z[k]>=dep && K2>0 &&i<n){
               k += 1;
               break;
           }
           k = k + 1;
    }
    }
    /*---------------------------------------*/
    
    /*tracing the upgoing part of the ray*/
     for (i=n-1;i>=0;i--){

        dep -= h[i];
        a = asin(p*V[i]);
        //printf("%d %d\n",i,pass_slab);

    while (k<MAXLEN)
    {		
    		if(dip==1){
    		slab_dep = slab(th,x[k-1]);
    		diff = z[k-1] - slab_dep;
    		if(diff <=0 && fabs(diff) <5 && pass_slab==0)
    		{
    			pass_slab=1;
    			cdep=slab_dep;
    			cn = point_layer(h,n,cdep);
    			if (th>=a)
    				ph = th - a;
           		else
               		ph = a - th;
               	V = mod->Vp;
               	p = sin(ph)/V[i];
    			//printf("%d %.3f\n",i,z[0]);
    		}
    		}
    /*
    		if(i==0 && pass_slab==0)
    		{
    			slab_dep=slab(th,x[k-1]);
    			diff = slab_dep - z[k-1];
    			if ( diff >=0 )
    			{
    				cdep=slab_dep;
    				cn = point_layer(h,n,cdep);
    				pass_slab=1;
    				if (th>=a)
    					ph = th - a;
            		else
                		ph = a - th;
                	V[i] = mod->Vp[i];
                	p = sin(ph)/V[i];
    			}
    		}
    */
            K1 = V[i]*p;
            K2 = V[i]*p;
            x[k] = x[k-1] + dt*K2;
            
            K2 = -1*V[i]*sqrt(1-p*p*V[i]*V[i]);

            z[k] = z[k-1] + dt*K2;
            
           if (z[k]<=dep && K2<0){
               k += 1;
               break;
           }
           
           if (z[k]< 0)
           {
                   break;
           }
           k = k + 1;
    }
     }
        /*------------------------------------*/

     /*store the tracing result to RayData*/
    Data->dis = x[k-1];
    t = dt*k;
    Data->np = k;
    Data->t = t;
    //printf("%.3f\n",x[k-1]);
    /*-------------------------------------*/
}

RayData *ray_shoot(struct raycor R1,struct raycor R2,Model *mod,float dis,char *phase)
{
    float d1,d2,d0=0,i0=0,i1,i2,z0,x0;
    int i=0;
    RayData *Data;
    struct raycor R0;

    Data = (RayData *)malloc(1*sizeof(RayData));
    Data->phase = phase;
    /*bisection method*/
    ray_tracing_ml(R1,mod,Data);
    free_path(Data); 
    d1 = Data->dis;

	if (d1>=dis)
	{
		printf("Object distace larger than that of minmum initial incident angle!\n");
		printf("Abort\n");
		exit(0);
	}
    i1 = R1.ri;
    //exit(0);
    ray_tracing_ml(R2,mod,Data);
    free_path(Data); 
    d2 = Data->dis;
    i2 = R2.ri;
    //exit(0);
    z0 = R2.rz;
    x0 = R2.rx;
    R0.rx = x0;
    R0.rz = z0;
    while(fabs(d0 - dis)>=0.1)
    {
        i0 =(i1+i2)/2;
        R0.ri = i0;
        ray_tracing_ml(R0,mod,Data);
        d0 = Data->dis;

        //printf("%.3f\n",d0);
        
        if (fabs(d0-dis)>=0.1)
            free_path(Data);
        if (d0>dis){
            i2 = i0;
        }
        else{
            i1 = i0;
        }
        i=i+1;
        if (i>100)
        {
            printf("Shooting Fail! Target may not be reached.\n");
            exit(0);
        }
    }
    /* ---------------- */
    if (VERBOSE == 1)
    {
        printf("------\n");
        printf("Result\n");
        printf("---------------------------------\n");
        printf("Steps |Date Points|Time |Distance\n");
        printf("---------------------------------\n");
    }
    printf("%d\t%d\t%.2f\t%.3f\n",i,Data->np,Data->t,Data->dis);
    return Data;
}
RayDataSP *ray_shoot_sp(struct raycor_sphere R1,struct raycor_sphere R2,Model *mod,float delta,char *phase)
{
    float d1,d2,d0=0,i0=0,i1,i2,r0,phi0;
    int i=0;
	float ds=0.01;
    RayDataSP *Data;
    struct raycor_sphere R0;

    Data = (RayDataSP *)malloc(1*sizeof(RayDataSP));
    Data->phase = phase;
    /*bisection method*/
    ray_tracing_sp(R1,mod,Data);
    free_path_sp(Data); 
    d1 = Data->delta;

	if (d1>=delta && delta>=ds)
	{
		printf("Object distace larger than that of minmum initial incident angle!\n");
		printf("Abort\n");
		exit(0);
	}
	

    i1 = R1.theta;
    //exit(0);
    ray_tracing_sp(R2,mod,Data);
    free_path_sp(Data); 
    d2 = Data->delta;
    i2 = R2.theta;
	//printf("d1->%f,d2->%f\n",d1,d2);
    //exit(0);
    r0 = R2.r;
    phi0 = R2.phi;
    R0.r = r0;
    R0.phi = phi0;
	if (delta>ds){
    while(fabs(d0 - delta)>= ds)
    {
        i0 =(i1+i2)/2;
        R0.theta = i0;
        ray_tracing_sp(R0,mod,Data);
        d0 = Data->delta;

        //printf("%.3f %.3f\n",i0,d0);
        
        if (fabs(d0-delta)>= ds)
            free_path_sp(Data);
        if (d0>delta){
            i2 = i0;
        }
        else{
            i1 = i0;
        }
        i=i+1;
        if (i>1000)
        {
            printf("Shooting Fail! Target may not be reached.\n");
            exit(0);
        }
    }
	}
	else if (delta<=ds)
	{
		ray_tracing_sp(R1,mod,Data);
	}
    /* ---------------- */
    if (VERBOSE == 1)
    {
        printf("------\n");
        printf("Result\n");
        printf("---------------------------------\n");
        printf("Steps |Date Points|Time |Distance\n");
        printf("---------------------------------\n");
    }
    printf("%d\t%d\t%.4f\t%.4f\n",i,Data->np,Data->t,Data->delta);
    return Data;
}
