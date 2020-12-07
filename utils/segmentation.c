int *phase_difference(int *phases, int i, int k)
{
	int iter = 0;
	int tmp = 0;
	float diff[i];

	while (iter < i)
	{	
		if (iter <=k )
		{
		diff[iter] = phases[iter + (k * tmp)] - phases[iter + (k * (tmp + 1))];
		iter++;
		}
		else
		tmp++;
	}
	return(diff);
}

