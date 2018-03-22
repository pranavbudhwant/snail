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
/*
kmeans():
	Assignes cluster ID to each sample in dataset
	Parameters:
		T **X: Input vector, with size mxn.
		int *C: vector to store the cluster number for each sample point, has size mx1
		T **mu: centroid vector with random initialized values, of size kxn.
		size_t m: number of training examples.
		size_t n: number of features.
		size_t k: number of clusters.
		long int num_iter: number of iterations.
	Returns:
		void			
*/
template <typename T>
void kmeans(T** X, int *C, T **mu, long long int m, long long int n, long long int k, long long int num_iter){
	int l;
	for(int i=0;i<num_iter;i++){	//Run K Means
		#pragma omp parallel for schedule(static) private(l)	//Assigning a cluster to each point in sample dataset
		for(int j=0;j<m;j++){
			double min_distance = 99999;
			int cent = 0;
				
			for(l=0;l<k;l++){	//Calculate distance from every cluster centroid mu
				double sum=0;
				for(int p=0;p<n;p++)	//Calculate the euclidian distance
					sum += pow((X[j][p] - mu[l][p]),2); 
				sum = sqrt(sum);
				if(sum < min_distance){
					min_distance = sum;
					cent = l;
				}
			}
			C[j] = cent; 	//Assign the cluster with minimum distance
		}
		#pragma omp parallel for schedule(static) private(l)	//Centroid update
		for(int j=0;j<k;j++){
			double sum[n] = {0}; int count=0;
			for(int l=0;l<m;l++){
				if(C[l]==j){
					for(int k=0;k<n;k++){
						sum[k]+=X[l][k];
					}
					count++;
				}
			}
			for(int l=0;l<n;l++){
				mu[j][l] = sum[l]/count;
			}
		}
	}
}

/*
cluster():
	Computes the global optima for all the random initializations of centroid coordinates in num_iter iterations
	Parameters:
		T **X: Input vector, with size mxn.
		int *C: vector to store the cluster number for each sample point, has size mx1
		size_t m: number of training examples.
		size_t n: number of features.
		size_t k: number of clusters.
		long int num_iter: number of iterations.
	Returns:
		void			
*/
template <typename T>
void cluster(T **X, int *C, size_t m, size_t n, size_t k, long int num_iter){
	int temp_C[m], i,j,random_index;
	double min_cost = 999999999, temp_cost, **mu;
	mu = new_2D_array<T>(k,n);
	
	#pragma omp parallel for schedule(static) private(i,j,temp_cost)
	for(long long int q=0; q<num_iter; q++){
		
		#pragma omp parallel for schedule(static) private(j) shared(mu, X, m) num_threads(2)
		for(i=0;i<k;i++){
			random_index = rand()%m; 	//Set Initial Centroids to random values
			for(j=0;j<n;j++)
				mu[i][j] = X[random_index][j];
		}
		
		kmeans<T>(X, temp_C, mu, m, n, k, 500); 	//Run K Means for qth iteration each 500 times

		temp_cost = 0;
		for(i=0;i<m;i++){ 	//Compute Cost for qth iteration
			for(j=0;j<n;j++){
				temp_cost += pow((X[i][j] - mu[ temp_C[i] ][j]),2);
			}
		}
		temp_cost /= m;		// mean squared difference (or distance)
		
		if(temp_cost < min_cost){	//Update values of C array with values of temp_C array of iteration with mininmum cost
			min_cost = temp_cost;
			for(j=0;j<m;j++) C[j] = temp_C[j];
		}
	}
}
