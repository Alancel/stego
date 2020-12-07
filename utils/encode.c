#include "wave.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <complex.h>
 
double PI = 3.14;
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


char *calculating_values(char *str, int i, int *amplitudes) // i = samples number arr = samples array
{
	size_t Lm = 8 * strlen(str); // находим битувую длину сообщения
	int v = ceil((log(Lm)/log(2)) + 1); // степень двойки для получения сегментов
	int k = pow(2, v + 1); // длина одного сегмента
	int N = ceil(i / k); // количество сегментов
	str = str_to_bit(str);
	cplx data[i];
	float phases[i];
	for (int j = 0; j < i; j++)
		data[j] = amplitudes[j];
	fft(phases, i);
	//fft(amplitudes, i);
	phase_init(phases, i);

}

char *str_to_bit(char *str)
{
	char *res = calloc(8, strlen(str));
	int j;
	int i;

	j = 0;
    for(; *str != 0; ++str)
    {
        for(i = 6; i >= 0; --i) 
		{
            (*str & 1 << i) ? (res[j] = '1')  : (res[j] = '0');
			j++;
		}
	}
    return (res);
}

void phase_init(cplx *data, int k)
{
	float phases[k];
	
	for (int i = 0; i < k; i++)
		phases[i] = atan(cimag(data[i]/creal(data[i])));
}

