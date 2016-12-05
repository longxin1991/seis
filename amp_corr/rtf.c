#include "rtf.h"
#include "lapacke.h"
#include <math.h>
#include <stdio.h>

int s2l(ITF intf,float phi,RTF_P *RT){

	float phi_1,phi_2,phi_3;
	float a1,a2,b1,b2,p1,p2;
	float M[9],N[3];
	lapack_int info,n,nrhs,lda,ldb,ipiv[3];

	n = 3;
	nrhs = 1;
	lda = 3;
	ldb = 3;

	a1 = intf.a1;
	b1 = intf.b1;
	p1 = intf.p1;
	a2 = intf.a2;
	b2 = intf.b2;
	p2 = intf.p2;

	phi_1 = phi;
	phi_2 = asinf(a2*sinf(phi)/a1);
	phi_3 = asinf(b1*sinf(phi)/a1);

	N[0] = -1*cosf(phi);
	N[1] = -1*b1*sinf(2*phi)/a1;
	N[2] = 1 - 2*powf(b1*sinf(phi)/a1,2);

	M[0] = b1*sinf(phi)/a1;
	M[1] = 2*powf(b1*sinf(phi)/a1,2) - 1;
	M[2] = 2*powf(b1/a1,2)*sinf(phi)*cosf(phi_3);
	M[3] = -1*cosf(phi);
	M[4] = -1*b1*sinf(2*phi)/a1;
	M[5] = 2*powf(b1*sinf(phi)/a1,2) - 1;
	M[6] = -1*cosf(phi_2);
	M[7] = 0;
	M[8] = a2*p2/(a1*p1);

	//print_matrix(COL_MAJOR,N,3,1);
	//print_matrix(COL_MAJOR,M,3,3);
	info = LAPACKE_sgesv(LAPACK_COL_MAJOR,n,nrhs,M,lda,ipiv,N,ldb);
	
	if (info != 0)
		return info;
	else{
		RT->p1s1 = -1*N[0];
		RT->p1p1 = N[1];
		RT->p1p2 = N[2];
		return info;
	}
}

int l2s(ITF intf,float phi,RTF_P *RT){

	float phi_1,phi_2,phi_3;
	float a1,a2,b1,b2,p1,p2;
	float M[9],N[3];
	lapack_int info,n,nrhs,lda,ldb,ipiv[3];

	n = 3;
	nrhs = 1;
	lda = 3;
	ldb = 3;

	a1 = intf.a1;
	b1 = intf.b1;
	p1 = intf.p1;
	a2 = intf.a2;
	b2 = intf.b2;
	p2 = intf.p2;

	phi_1 = phi;
	phi_2 = asinf(a2*sinf(phi)/a1);
	phi_3 = asinf(b2*sinf(phi)/a1);

	N[0] = cosf(phi);
	N[1] = 0;
	N[2] = -1;

	M[0] = cosf(phi);
	M[1] = 0;
	M[2] = 1;
	M[3] = cosf(phi_2);
	M[4] = 2*b2*sinf(phi)*cosf(phi_2)/a1;
	M[5] = -1*p2*a2*(1-2*powf(b2*sinf(phi)/a1,2))/(p1*a1);
	M[6] = -1*b2*sinf(phi)/a1;
	M[7] = 1 - 2*powf(b2*sinf(phi)/a1,2);
	M[8] = 2*p2*powf(b2/a1,2)*sinf(phi)*cos(phi_3)/p1;

	//print_matrix(COL_MAJOR,N,3,1);
	//print_matrix(COL_MAJOR,M,3,3);
	info = LAPACKE_sgesv(LAPACK_COL_MAJOR,n,nrhs,M,lda,ipiv,N,ldb);
	if (info != 0)
		return info;
	else{
		RT->p1p1 = N[0];
		RT->p1p2 = N[1];
		RT->p1s2 = N[2];
		return info;
	}
}
void print_parameter(ITF intf)
{
	float a1,b1,p1,a2,b2,p2;
	a1 = intf.a1;
	b1 = intf.b1;
	p1 = intf.p1;
	a2 = intf.a2;
	b2 = intf.b2;
	p2 = intf.p2;
	
	printf("%.4f %.4f %.4f %.4f %.4f %.4f\n",a1,b1,p1,a2,b2,p2);
}

void reverse_layer(ITF *intf)
{
	float tmp;

	tmp = intf->a1;
	intf->a1 = intf->a2;
	intf->a2 = tmp;

	tmp = intf->b1;
	intf->b1 = intf->b2;
	intf->b2 = tmp;

	tmp = intf->p1;
	intf->p1 = intf->p2;
	intf->p2 = tmp;
}
