#include<iostream>
using namespace std;
template <typename T>
T** new_2D_array(size_t rows, size_t columns){
	T **array = new T*[rows];
	for(size_t i=0;i<rows;i++) array[i] = new T[columns];
	return array;
}
template <typename T>
void subtract(T** A, T **B, T **C, size_t row, size_t col){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			C[i][j] = A[i][j] - B[i][j];
		}
	}
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
template <typename T>
void gradient_descent(T** X, T** Y, T** theta, size_t m, size_t n, double alpha, long int num_iter){
	T** hyp = new_2D_array<T>(m,1);
	T** error = new_2D_array<T>(m,1);
	T** err_adj = new_2D_array<T>(n,1);
	T** final_error = new_2D_array<T>(n,1);
	T** X_trans = new_2D_array<T>(n,m);
	T** beta = new_2D_array<T>(1,1);
	beta[0][0] = T(alpha/m);
	transpose(X,X_trans,m,n);
	for(long int itr=0; itr<num_iter; itr++){
		multiply<T>(X,theta,hyp,m,n,n,1);
		subtract<T>(hyp,Y,error,m,1);
		multiply<T>(X_trans,error,err_adj,n,m,m,1);
		multiply<T>(err_adj,beta,final_error,n,1,1,1);
		subtract<T>(theta,final_error,theta,n,1);
	}
}
