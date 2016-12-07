#include "stdafx.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include "mpi.h" 
using namespace std;

int main(int argc, char* argv[])
{
	int procs_rank, procs_count;
	int n = 10, local_n;
	double *local_a, *local_c, *a, *c;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procs_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_rank);

	local_n = n / procs_count;
	local_a = (double *)malloc((local_n * n) * sizeof(double));
	local_c = (double *)malloc(local_n * sizeof(double));
	c = (double *)malloc(n * sizeof(double));
	a = (double *)malloc((n * n) * sizeof(double));

	if (procs_rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				a[i*n + j] = rand() % 10 + 1;
				cout << a[i*n + j] << " ";
			}
			cout << endl;
		}
	}

	MPI_Scatter(a, n * local_n, MPI_DOUBLE, local_a, n * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	for (int i = 0; i < local_n; i++)
	{
		local_c[i] = 0;

		for (int j = 0; j < n; j++)
		{
			local_c[i] += local_a[i * n + j];
		}
	}
	 
	MPI_Gather(local_c, local_n, MPI_DOUBLE, c, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

	if (procs_rank == 0)
	{
		int index = 0;
		int replacementIndex = n - 1;

		cout << "-----------------------------" << endl;

		for (int i = 0; i < n; i++)
		{
			cout << c[i] << endl;
		}

		for (int kol = 0; kol < n; kol++)
		{
			int max = 0;

			for (int i = 0; i < n; i++)
			{
				if (c[i] > max)
				{
					index = i;
					max = c[i];
				}
			}

			for (int j = 0; j < n; j++)
			{				
				int temp = a[index * n + j];
				a[index * n + j] = a[replacementIndex * n + j];
				a[replacementIndex * n + j] = temp;
			}

			c[index] = c[replacementIndex];
			c[replacementIndex] = 0;
			replacementIndex--;
		}

		cout << "-----------------------------" << endl;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << a[i*n + j] << " ";
			}
			cout << endl;
		}
	}
	MPI_Finalize();
	return 0;
}