#include<iostream>
using namespace std;
template <typename T>
T** new_2D_array(size_t rows, size_t columns){
	T **array = new T*[rows];
	for(size_t i=0;i<rows;i++) array[i] = new T[columns];
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
template <typename T>
void multiply(T** A, T** B, T** C, size_t row_A, size_t col_A, size_t row_B, size_t col_B){
	if(col_A == row_B){
		int i,j,k,chunk = 10;
		#pragma omp parallel for private(j,k) schedule(guided, chunk) num_threads(int((row_A)/10)) shared(A,B,C)
		for(i=0;i<row_A;i++){
			for(j=0;j<col_B;j++){
				C[i][j] = 0;
				for(k=0;k<col_A;k++) C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
	else
		cout<<endl<<"Incompatible matrix sizes!";
}