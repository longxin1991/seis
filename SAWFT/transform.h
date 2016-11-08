#include <complex.h>
#include <gsl/gsl_fft_complex.h>
#define REAL(z,i) ((z)[2*(i)])
#define IMAG(z,i) ((z)[2*(i)+1])

void hilbert_t(int n,const double complex *in,double complex *out);
void envelope(int n,const double complex *in,double complex *out);
void convolve(int n,const double *d1,const double *d2,double *out);
