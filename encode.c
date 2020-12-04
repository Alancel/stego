#include "wave.h"
#include <string.h>
#include <math.h>




char *message_encode(char *str, int i, int *arr) // i = samples number arr = samples array
{
	size_t Lm = 8 * strlen(str); // находим битувую длину сообщения
	int v = ceil((log(Lm)/log(2)) + 1); // степень двойки для получения сегментов
	int k = pow(2, v + 1); // длина одного сегмента
	int N = ceil(i / k); // количество сегментов
	double Re = 0, Im = 0, summaRe = 0, summaIm = 0, Ak[1024] = {0}, Ak_1[1024] = {0}, Arg = 0;
	
	
	for (int i = 0; i < k; i++)
	{
		summaRe = 0; summaIm = 0;
		for (int j = 0; j < k; j++)
		{
			Arg = 2.0*M_PI*j*i/k;
			Re = cos(Arg)*(arr[j]); // amplitudes
			Im = sin(Arg)*(arr[j]); // phases
			summaRe = summaRe + Re;
			summaIm = summaIm + Im;
		}
		Ak[i] = sqrt(summaRe*summaRe + summaIm*summaIm);
	}
}
