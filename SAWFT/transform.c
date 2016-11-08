#include <complex.h>
#include "transform.h"
#include <math.h>

/*hilbert_t  Hilbert transformation using fftw, pi/2 phase is advanced. */
/*			 Only the situation of even point number is considered.     */
/*envelope   Halculate envelope function using hilbert_t.               */
/*convolve   Convolve two series using fftw.                            */
/*longxin 2016/10/21  @eri u-tokyo                                      */

/*
void fftshift(int n,fftw_complex *in,fftw_complex *out)
{
	int i;
	for (i=0;i<n;i++)
	{
		out[i][0] = 0;
		out[i][1] =0;
	}

	for (i=0;i<n/2;i++)
	{
		out[n/2-i][0] = in[n-i-1][0];
		out[n/2+i][0] = in[i][0];
		out[n/2-i][1] = in[n-i-1][1];
		out[n/2+i][1] = in[i][1];
	}
}
*/
void hilbert_t(int n,const double complex *in,double complex *out)
{
	int i;
	double d[2*n],tmp;
	gsl_fft_complex_wavetable *wavetable;
	gsl_fft_complex_workspace *workspace;

	wavetable = gsl_fft_complex_wavetable_alloc (n);
	workspace = gsl_fft_complex_workspace_alloc (n);
	//fftw_complex a1[n],a2[n];
	//fftw_plan p;

	//p = fftw_plan_dft_1d(n,a1,a2,-1,FFTW_ESTIMATE);
	
	for (i=0;i<n;i++)
	{
		//a1[i][0] = creal(in[i]);
		//a1[i][1] = cimag(in[i]);
		REAL(d,i) = creal(in[i]);
		IMAG(d,i) = cimag(in[i]);
	}
	//fftw_execute(p);
	gsl_fft_complex_forward(d,1,n,wavetable,workspace);

	//p = fftw_plan_dft_1d(n,a1,a2,1,FFTW_ESTIMATE);

	//a1[0][0] = a2[0][0];
	//a1[0][1] = a2[0][1];

	//a1[n/2][0] = a2[n/2][0];
	//a1[n/2][1] = a2[n/2][1];
	for (i=1;i<n/2;i++)
	{	
		/*There is a difference in the defination of Hilbert Transformation*/
		/*Whether the phase advances pi/2 or delays pi/2                   */ 
		//a1[i][0] = a2[i][1]*(-1);
		//a1[i][1] = a2[i][0];
		tmp = REAL(d,i);
		REAL(d,i) = IMAG(d,i)*(-1);
		IMAG(d,i) = tmp;
		//a1[n-i][0] = a1[i][0];
		//a1[n-i][1] = a1[i][1]*(-1);
		REAL(d,n-i) = REAL(d,i);
		IMAG(d,n-i) = IMAG(d,i)*(-1);
	}	
	//fftw_execute(p);
	gsl_fft_complex_inverse(d,1,n,wavetable,workspace);	

	for (i=0;i<n;i++)
	{
		//out[i] = (a2[i][0] + I * a2[i][1])/(n-1);
		out[i] = REAL(d,i) + I*IMAG(d,i);
	}
	gsl_fft_complex_wavetable_free (wavetable);
	gsl_fft_complex_workspace_free (workspace);
}

void envelope(int n,const double complex *in,double complex *out)
{
	int i;

	hilbert_t(n,in,out);

	for (i=0;i<n;i++)
	{
		out[i] = creal(in[i])*creal(in[i]) + creal(out[i])*creal(out[i]);
		out[i] = sqrt(out[i]);
	}
}

void convolve(int n,const double *d1,const double *d2,double *out)
{
	int i;
	double a1[2*n],a2[2*n],tmp;
	gsl_fft_complex_wavetable *wavetable;
	gsl_fft_complex_workspace *workspace;
	//fftw_complex *a1,*a2,*b1,*b2;
	//fftw_plan p1,p2;
	wavetable = gsl_fft_complex_wavetable_alloc (n);
	workspace = gsl_fft_complex_workspace_alloc (n);
	//a1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);
	//a2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);
	//b1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);
	//b2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);

	//p1 = fftw_plan_dft_1d(n,a1,b1,-1,FFTW_ESTIMATE);

	for (i=0;i<n;i++)
	{
		//a1[i][0] = d1[i];
		//a1[i][1] = 0;
		REAL(a1,i) = d1[i];
		IMAG(a1,i) = 0;
		REAL(a2,i) = d2[i];
		IMAG(a2,i) = 0;
	}

	//fftw_execute(p1);
	gsl_fft_complex_forward(a1,1,n,wavetable,workspace);
	gsl_fft_complex_forward(a2,1,n,wavetable,workspace);

	
	//fftw_execute(p1);

	//p2 = fftw_plan_dft_1d(n,b2,a1,1,FFTW_ESTIMATE);
	
	for (i=0;i<n;i++)
	{
		//b2[i][0] = a2[i][0] * b1[i][0] - a2[i][1] * b1[i][1];
		//b2[i][1] = a2[i][0] * b1[i][1] + a2[i][1] * b1[i][0];
		tmp = REAL(a1,i);
		REAL(a1,i) = REAL(a1,i) * REAL(a2,i) - IMAG(a1,i) * IMAG(a2,i);
		IMAG(a1,i) = tmp * IMAG(a2,i) + IMAG(a1,i) * REAL(a2,i);
	}

	//fftw_execute(p2);
	gsl_fft_complex_inverse(a1,1,n,wavetable,workspace);
	for (i=0;i<n;i++)
		out[i] = REAL(a1,i);
	
	gsl_fft_complex_wavetable_free (wavetable);
	gsl_fft_complex_workspace_free (workspace);
	//fftw_free(a1);fftw_free(a2);fftw_free(b1);fftw_free(b2);
	//fftw_destroy_plan(p1);fftw_destroy_plan(p2);
}
