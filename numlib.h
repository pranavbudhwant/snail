#include<iostream>
using namespace std;
template <typename T>
T** new_2D_array(size_t rows, size_t columns){
	T **array = new T*[rows];
	for(size_t i=0;i<columns;i++) array[i] = new T[columns];
	return array;
}
template <typename T>
double transpose(T** array, T** trans, size_t rows, size_t cols){
	int i,j;
	clock_t beg = clock();
	#pragma omp parallel for schedule(guided) private(i,j) num_threads(int(rows/10))
	for(i=0;i<rows;i++) for (j=0;j<cols;j++) trans[j][i] = (array[i][j]);
	clock_t end = clock();
	return (double(end-beg)/CLOCKS_PER_SEC);
}
