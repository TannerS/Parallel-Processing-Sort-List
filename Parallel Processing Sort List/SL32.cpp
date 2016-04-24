#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#define List_Size 25


void insertionSort(int[], int, int);
void generateArr(int[], int);
void displayArr(int[], int);

// argc = cpu count, argv = file.cpp
int main(int argc, char *argv[])
{
	int n = 0;
	int* p0_list;
	int* p1_list;
	int* combined_list;

	int myid, numprocs = 0;
	// processor name
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int count = 0;
	int namelen = 0;
	//initialize MPI execution environment
	MPI_Init(&argc, &argv);
	//each process get total # of processes 
	//the total # of processes specified in mpirun –np n
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	//each process gets its own id
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	// Gets the name of the processor
	MPI_Get_processor_name(processor_name, &namelen);
	// display info
	fprintf(stderr, "process %d on %s\n", myid, processor_name);
	fflush(stderr);

	if (myid == 0)
	{
		std::cin >> n;
		MPI_Send(&n, 1, MPI_INT, 1, 12345, MPI_COMM_WORLD);
		p0_list = new int[n];
		generateArr(p0_list, n);
		fprintf(stderr, "Un-sorted root\n");
		displayArr(p0_list, n);
		insertionSort(p0_list, 0, n);
		fprintf(stderr, "sorted root\n");
		displayArr(p0_list, n);
		delete[] p0_list;
	}
	else
	{
		MPI_Recv(&n, 1, MPI_INT, 0, 12345, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		p1_list = new int[n];
		generateArr(p1_list, n);
		fprintf(stderr, "Un-sorted slave\n");
		displayArr(p1_list, n);
		insertionSort(p1_list, 0, n);
		fprintf(stderr, "sorted slave\n");
		displayArr(p1_list, n);
		delete[] p1_list;
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

void generateArr(int arr[], int size)
{
	srand(time(0));

	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % 100 + 1;
	}

}

void displayArr(int arr[], int size)
{
	std::cout << "Start of array" << std::endl;
	for (int i = 0; i < size; i++)
		std::cout << arr[i] << std::endl;
	std::cout << "End of array" << std::endl;
}




