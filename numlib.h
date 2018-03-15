#include<omp.h>

/*
new_2D_array():
	Creates a 2D array and returns the pointer to the array.
	Parameters:
		size_t num_rows: number of rows in 2D array.
		size_t num_columns: number of columns in 2D array.
	Returns:
		T**: double pointer to the 2D array of type T.
*/
template <typename T>
T** new_2D_array(size_t num_rows, size_t num_columns){
	T **array = new T*[num_rows];
	for(size_t i=0;i<num_rows;i++) array[i] = new T[num_columns];
	return array;
}

/*
subtract():
	Performs matrix subtraction, C = A - B.
	Parameters:
		T **A: 2D matrix of type T, input matrix A
		T **B: 2D matrix of type T, input matrix B
		T **C: 2D matrix of type T, resultant matrix of A - B
		size_t num_rows: number of rows in input matrices.
		size_t num_columns: number of columns in input matrices.
	Returns:
		void
*/
template <typename T>
void subtract(T **A, T **B, T **C, size_t num_rows, size_t num_columns){
	int i,j;
	#pragma omp parallel for schedule(static) private(j) shared(A,B,C)
	for(i=0;i<num_rows;i++){
		for(j=0;j<num_columns;j++)
			C[i][j] = A[i][j] - B[i][j];
	}
}

/*
transpose():
	Computes the transpose of given matrix.
	Parameters:
		T **array: 2D matrix of type T, input matrix.
		T **trans: 2D matrix of type T, transpose of input matrix.
		size_t num_rows: number of rows in input matrix.
		size_t num_columns: number of columns in input matrix.
	Returns:
		void
*/
template <typename T>
void transpose(T** array, T** trans, size_t num_rows, size_t num_columns){
	int i,j;
	#pragma omp parallel for schedule(static) private(j)
	for(i=0;i<num_rows;i++) 
		for (j=0;j<num_columns;j++) 
			trans[j][i] = array[i][j];
}

/*
multiply():
	Performs matrix multiplication, C = A * B.
	Parameters:
		T **A: 2D matrix of type T, input matrix A
		T **B: 2D matrix of type T, input matrix B
		T **C: 2D matrix of type T, resultant matrix of A * B
		size_t num_rows_A: number of rows in input matrix A
		size_t num_columns_A: number of columns in input matrix A
		size_t num_rows_B: number of rows in input matrix B
		size_t num_columns_B: number of columns in input matrix B
	Returns:
		bool: true, if matrix multiplication is possible(matrix sizes are compatible).
			  false, if matrix multiplication is not possible(matrix sizes are incompatible).
*/
template <typename T>
bool multiply(T** A, T** B, T** C, size_t num_rows_A, size_t num_columns_A, size_t num_rows_B, size_t num_columns_B){
	if(num_columns_A == num_rows_B){
		int i,j,k;
		#pragma omp parallel for private(j,k) schedule(static) shared(A,B,C)
		for(i=0;i<num_rows_A;i++){
			for(j=0;j<num_columns_B;j++){
				C[i][j] = 0;
				for(k=0;k<num_columns_A;k++) 
					C[i][j] += A[i][k]*B[k][j];
			}
		}
		return 1;
	}
	return 0;
}

/*
gradient_descent():
	Computes the parameter vector theta, using gradient descent.
	Parameters:
		T **X: Input vector, with size mxn.
		T **Y: Output vector, with size mx1.
		T **theta: Parameter vector, with size nx1.
		size_t m: number of training examples.
		size_t n: number of features.
		double alpha: learning rate.
		long int num_iter: number of iterations.
	Returns:
		void
*/
template <typename T>
void gradient_descent(T** X, T** Y, T** theta, size_t m, size_t n, double alpha, long int num_iter){
	T** hypothesis = new_2D_array<T>(m,1); //Stores the value of hypothesis = X*theta
	T** difference = new_2D_array<T>(m,1); //Stores the difference between hypothesis and output (hypothesis - Y)
	T** derivative = new_2D_array<T>(n,1); //Stores the values of derivative terms( (hypothesis - Y)*X )
	T** error = new_2D_array<T>(n,1); //Stores the final error values to be subtracted from parameter vector
	T** X_transpose = new_2D_array<T>(n,m); //Stores the transpose of input vector X
	T** beta = new_2D_array<T>(1,1); //Stores the constant learning rate/number of training examples(alpha/m)
	beta[0][0] = T(alpha/m);
	transpose(X,X_transpose,m,n);
	for(long int itr=0; itr<num_iter; itr++){
		multiply<T>(X,theta,hypothesis,m,n,n,1); //hypothesis = X*theta
		subtract<T>(hypothesis,Y,difference,m,1); //difference = hypothesis - Y
		multiply<T>(X_transpose,difference,derivative,n,m,m,1); //derivative = transpose(X)*difference
		multiply<T>(derivative,beta,error,n,1,1,1); //error = (alpha/m) * derivative
		subtract<T>(theta,error,theta,n,1); //theta -= error
	}
}
