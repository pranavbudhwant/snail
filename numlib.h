#include <omp.h>
#include <cstring>
#include <fstream>
#include <iomanip>

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
csv_write():
	Writes the given 2D array as csv to the given file.
	Parameters:
		const char *file_name: name of the file to which the array has to be written.
		char mode: 
			'a': append
			'w': overwrite
		T **array: 2D array to be written to the file.
		size_t num_rows: number of rows in 2D array.
		size_t num_columns: number of columns in 2D array.
	Returns:
		bool:
			0: Error in writing.
			1: Successful operation.
}
*/
template <typename T>
bool csv_write(const char *file_name, char mode, T **array, size_t num_rows, size_t num_columns){
	std::ofstream file; //Output file stream object, used to handle the file.
	if(mode == 'a') file.open(file_name, std::ios::app); //Mode 'a' == append to file.
	else if(mode == 'w') file.open(file_name, std::ios::out); //Mode 'w' == write to file.
	else return 0; //Invalid mode
	if(file.is_open()){
		for(int i=0;i<num_rows;i++){
			for(int j=0;j<num_columns;j++){
				if(j==num_columns-1) file<<std::setprecision(10)<<array[i][j];
				else file<<std::setprecision(10)<<array[i][j]<<",";
			}
			file<<std::endl;
		}
		file.close(); //Close the current file.
		return 1; //Return successful operation.
	}
	return 0; //Error in opening the file.
} 

/*
csv_read():
	Reads given csv file and stores the data in the given 2D array.
	Parameters:
		const char *file_name: name of the file to which the array has to be written. 
		int mode:
			0: Numerical mode
			1: String mode
		T **array: 2D array to store the data from the file.
		long long int num_rows: Number of rows read from the file.
		long long int num_columns: Number of columns read from the file.
	Returns:
		bool:
			0: Error in reading.
			1: Successful operation.
*/
template <typename T>
bool csv_read(const char *file_name, int mode, T **array, long long int &num_rows, long long int &num_columns){
	//Initialize
	num_rows = 0;
	num_columns = 0;
	std::string value; //Stores the line read from the file.
	char *token; //Stores comma seperated values in a line.
	std::ifstream file; //File object to handle the file
	file.open(file_name, std::ios::in);
	if(file.is_open()){
		while( file.good() ){
			std::getline(file, value, '\n'); //Read a line
			if(value.length()){
				num_columns = 0;
				token = strtok(((char*)value.c_str()),","); //Split the string seperated by ",".
				while(token!=NULL){
					if(token){
						if(!mode) array[num_rows][num_columns] = (T)atof(token); //Convert value to double if numerical mode is selected.
						else array[num_rows][num_columns] = (T)(*token);
					}
					token = strtok(NULL,",");
					num_columns++; 
				}
				num_rows++;
			}
		}
		file.close(); //Close the file.
		return 1; //Successful operation.
	}
	return 0; //Error in opening file.
}

/*
copy_matrix():
	Copies specific rows of one matrix to the other.
	Parameters:
		T **dest: Destination matrix.
		T **src: Source matrix.
		unsigned long long int row_start: starting index of the row to be copied.
		unsigned long long int column_start: starting index of the column to be copied.
		unsigned long long int num_rows: number of rows to be copied.
		unsigned long long int num_columns: number of columns to be copied.
	Returns:
		void
*/
template <typename T>
void copy_matrix(T **dest, T **src, unsigned long long int row_start, unsigned long long int column_start, unsigned long long int num_rows, unsigned long long int num_columns){
	unsigned long long int i,j;
	#pragma omp parallel for private(j) schedule(static) num_threads(2)
	for(i=row_start; i<num_rows;i++){
		for(j=column_start; j<num_columns; j++){
			dest[i][j] = src[i][j];
		}
	}
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