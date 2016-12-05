#include "numc.h"
#include "rtf.h"
#include "dtype.h"

float geom(float evdp,float v_s,float v_r,float p,float d,float d2t)
{
	float is,ir,g,Rs,Rr,D,tmp;

	p = p*DEG2RAD;
	Rs = Re - evdp;
	D = d/DEG2RAD;
	is = asinf(v_s*p/Rs);
	ir = asinf(v_r*p/Re);
	tmp = sin(is)*v_s*d2t/(sinf(D)*cos(ir)*Rs*cos(is));
	g = sqrtf(-1*tmp);
	return g;

}

float corr_gs(float evdp,float v_s,float v_r,float p1,float p2,float d2t_1,float d2t_2)
{
	float i1s,i1r,i2s,i2r,r1,r2,acorr,Rs;

	p1 = p1*DEG2RAD;
	p2 = p2*DEG2RAD;
	Rs = Re - evdp;
	i1s = asinf(v_s*p1/Rs);
	i2s = asinf(v_s*p2/Rs);

	//printf("i1s->%f  i2s->%f\n",i1s*DEG2RAD,i2s*DEG2RAD);
	i1r = asinf(v_r*p1/Re);
	i2r = asinf(v_r*p2/Re);
	//printf("i1r->%f  i2r->%f\n",i1r*DEG2RAD,i2r*DEG2RAD);
	r1 = p1/(cos(i1s)*cos(i1r)*d2t_1);
	r2 = p2/(cos(i2s)*cos(i2r)*d2t_2);
	//printf("r1->%f  r2->%f\n",r1,r2);
	acorr = sqrtf(fabs(r1/r2));	
	return acorr;
}

float corr_rt(Model mod,float p1,float p2)
{
	ITF itf_cmb,itf_icb;	
	int i,n,stat;
	float i1,i2,cmb,icb,t1_s2l,t1_l2s,t2_s2l,t2_l2s;
	float c1,c2;
	RTF_P RT1,RT2;

	p1 = p1*DEG2RAD;
	p2 = p2*DEG2RAD;
	cmb = mod.h[49];
	icb = mod.h[73];
	/* cmb */
	itf_cmb.a1 = mod.Vp[49];
	itf_cmb.b1 = mod.Vs[49];
	itf_cmb.p1 = mod.rho[49];
	itf_cmb.a2 = mod.Vp[50];
	itf_cmb.b2 = mod.Vs[50];
	itf_cmb.p2 = mod.rho[50];

	i1 = asinf(itf_cmb.a1*p1/(Re-cmb));
	stat = s2l(itf_cmb,i1,&RT1);
	c1 = RT1.p1p2;
	i2 = asinf(itf_cmb.a1*p2/(Re-cmb));
	stat = s2l(itf_cmb,i2,&RT2);
	c2 = RT2.p1p2;
	//printf("i1->%f  i2->%f\n",i1*DEG2RAD,i2*DEG2RAD);

	reverse_layer(&itf_cmb);
	i1 = asinf(itf_cmb.a1*p1/(Re-cmb));
	i2 = asinf(itf_cmb.a1*p2/(Re-cmb));
	stat = l2s(itf_cmb,i1,&RT1);
	stat = l2s(itf_cmb,i2,&RT2);
	c1 *= RT1.p1p2;
	c2 *= RT2.p1p2;
	//printf("i1->%f  i2->%f\n",i1*DEG2RAD,i2*DEG2RAD);
	/* icb */
	itf_icb.a1 = mod.Vp[73];
	itf_icb.b1 = mod.Vs[73];
	itf_icb.p1 = mod.rho[73];
	itf_icb.a2 = mod.Vp[74];
	itf_icb.b2 = mod.Vs[74];
	itf_icb.p2 = mod.rho[74];

	i1 = asinf(itf_icb.a1*p1/(Re-icb));
	stat = l2s(itf_icb,i1,&RT1);
	c1 *= RT1.p1p2;
	//printf("i1->%f %f\n",i1*DEG2RAD,RT1.p1p2);

	reverse_layer(&itf_icb);
	i1 = asinf(itf_icb.a1*p1/(Re-icb));
	stat = s2l(itf_icb,i1,&RT1);
	c1 *= RT1.p1p2;
	//printf("i1->%f %f\n",i1*DEG2RAD,RT1.p1p2);
	
	//printf("t1->%f  t2->%f\n",RT1.p1p2,RT2.p1p2);
	//printf("c1->%f  c2->%f\n",c1,c2);
	//print_parameter(itf_cmb);
	//print_parameter(itf_icb);
	return c1/c2;
}

float cal_radp(CMT mt,float theta,float az)
{
	float m[3][3],p[3],a=0;
	int i,j;

	m[2][2] = mt.mrr;
	m[0][0] = mt.mtt;
	m[1][1] = mt.mff;
	m[2][0] = mt.mrt;
	m[0][2] = m[2][0];
	m[2][1] = -1*mt.mrf;
	m[1][2] = m[2][1];
	m[0][1] = -1*mt.mtf;
	m[1][0] = m[0][1];

	p[0] = sinf(theta)*cosf(az);
	p[1] = sinf(theta)*sinf(az);
	p[3] = cosf(theta);

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			a += p[i]*m[i][j]*p[j];
	return a;
}

float corr_rp(CMT mt,float evdp,float v_s,float p1,float p2,float az)
{
	float i1,i2,a1,a2;

	p1 = p1*DEG2RAD;
	p2 = p2*DEG2RAD;
	i1 = asinf(v_s*p1/(Re-evdp));
	i2 = asinf(v_s*p2/(Re-evdp));

	a1 = cal_radp(mt,i1,az);
	a2 = cal_radp(mt,i2,az);

	return a1/a2;
}
