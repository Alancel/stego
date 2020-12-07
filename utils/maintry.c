#include <stdio.h>
#include <math.h>
#include <complex.h>
 
double PI = 3.14159;
typedef double complex cplx;
 
void _fft(cplx buf[], cplx out[], int n, int step)
{
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);
 
		for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}
 
void fft(cplx buf[], int n)
{
	cplx out[n];
	for (int i = 0; i < n; i++) out[i] = buf[i];
 
	_fft(buf, out, n, 1);
}
 
 
void show(const char * s, cplx buf[]) {
	printf("%s", s);
	for (int i = 0; i < 4; i++)
		if (!cimag(buf[i]))
			printf("%g\n", creal(buf[i]));
		else
			printf("(%g, %g)\n", creal(buf[i]), cimag(buf[i]));
}

 
int main()
{
	cplx buf[] = {0, 1304, 2586, 3824};
	int a = 45;
	buf[1] = 45;
	int k = -1;
	show("Data: ", buf);
	fft(buf, 4);
	// show("\nFFT : ", buf);
	// while(++k < 4){
	// printf("\ntest magn = %g", sqrt((creal(buf[k])*creal(buf[k])) + (cimag(buf[k])*cimag(buf[k]))) );
	// printf("\ntest phase =  %g", (atan(cimag(buf[k]) / creal(buf[k]))));
	// printf("\nim = %g", creal(buf[k]));
	// printf("\nre = %g\n", cimag(buf[k])); 
	// }
	return 0;
}