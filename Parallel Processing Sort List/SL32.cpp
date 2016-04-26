#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

void generateArr(int[], int, int);
void insertionSort(int[], int, int);

// argc = cpu count, argv = -np
int main(int argc, char *argv[])
{
	double total_time_start = 0;
	double total_time_end = 0; 
	double start_alg_time = 0;
	double end_alg_time = 0;
	int* p0_list;
	int* p1_list;
	int myid, numprocs = 0;
	int n = 10000;
	// processor name
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen = 0;
	//initialize MPI execution environment
	MPI_Init(&argc, &argv);
	// each process get total # of processes 
	// the total # of processes specified in mpirun –np n
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	// each process gets its own id
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	// Gets the name of the processor
	MPI_Get_processor_name(processor_name, &namelen);
	// display info
	fprintf(stderr, "process %d on %s\n", myid, processor_name);
	fflush(stderr);

	if (myid == 0)
		total_time_start = MPI_Wtime();

	if (myid == 0)
	{
		// send size to p1
		MPI_Send(&n, 1, MPI_INT, 1, 12345, MPI_COMM_WORLD);
		// allocate space
		p0_list = new int[n];
		// generate array with random numbers
		generateArr(p0_list, n, 0);
		// sort array
		insertionSort(p0_list, 0, n);
		// temp var to hold number
		int temp = -1;
		// start time
		start_alg_time = MPI_Wtime();
		// loop list until the condition is true
		// condition if p0 element < p1 element 
		while(true)
		{ 
			// send number at [i]
			MPI_Send(&p0_list[n-1], 1, MPI_INT, 1, 11111, MPI_COMM_WORLD);
			// receive temp
			MPI_Recv(&temp, 1, MPI_INT, 1, 22222, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// if the list are already done
			// meaning only larger values than the sent values
			// are being reiceved so no need to go on
			if (temp >= p0_list[n-1])
					break;
			// send we sent over [i], we dont need that element
			// so we place temp inside of that and sort
			p0_list[n-1] = temp;
			// sort to get new larger element
			insertionSort(p0_list, 0, n);
		}

		// take time
		end_alg_time = MPI_Wtime();
		//fprintf(stderr, "P0 ARRAY (Smaller): \n");
		//for (int i = 0; i < n; i++)
			//fprintf(stderr, "P0: %d\n", p0_list[i]);
		// delete allocated space
		delete[] p0_list;
	}
	else
	{
		// rectrieve size of array
		MPI_Recv(&n, 1, MPI_INT, 0, 12345, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// allocate array
		p1_list = new int[n];
		// generate array with random numbers
		generateArr(p1_list, n, 10);
		// sort array
		insertionSort(p1_list, 0, n);
		// temp var to hold number
		int temp = -1;
		// loop list until the condition is true
		// condition if p0 element < p1 element 
		while (true)
		{
			// receive number temp
			MPI_Recv(&temp, 1, MPI_INT, 0, 11111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// send number at [i]
			MPI_Send(&p1_list[0], 1, MPI_INT, 0, 22222, MPI_COMM_WORLD);
			// if the list are already done
			// meaning only smaller values than the sent values
			// are being received so no need to go on
			if (temp <= p1_list[0])
				break;
			// we sent over [i], we dont need that element
			// so we place temp inside of that and sort
			p1_list[0] = temp;
			insertionSort(p1_list, 0, n);
		}
		// display results
		//fprintf(stderr, "P1 ARRAY (Larger): \n");
		//for (int i = 0; i < n; i++)
			//fprintf(stderr, "P1: %d\n", p1_list[i]);
		// delete allocated space
		delete[] p1_list;
	}

	// wait for all processes to finish
	MPI_Barrier(MPI_COMM_WORLD);

	if(myid == 0)
	{ 
		// take time
		total_time_end = MPI_Wtime();
		// show times
		std::cout << "Algorithm Time: " << ((end_alg_time - start_alg_time) / 2) << std::endl;
		fprintf(stderr, "Total Time: %f\n", ((total_time_end - total_time_start) / 2));
	}

	MPI_Finalize();
}


void insertionSort(int arr[], int low, int high)
{
	for (int i = low, j = 0; i <= high; i++)
	{
		int element = arr[i];
		j = i;

		while (j > 0 && arr[j-1] > element)
		{
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = element;
	}
}

void generateArr(int arr[], int size, int offset)
{
	srand(time(0) + offset);

	for (int i = 0; i < size; i++)
	{
		arr[i] = (rand() % 100 + 1);
	}

}
