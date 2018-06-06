#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<cstring>
#include<cmath>
#include<cstdlib>
#include<ctime>
using namespace std;

class IO;

template <typename T>
class DataFrame{
	//Functions to be written:
	/*
	DataFrame() - constructor - creates an empty DataFrame
	DataFrame(DataFrame) - copy constructor
	DataFrame(int) - creates a 1D DataFrame with all 0 of given size
	DataFrame(int, int) - creates a 2D DataFrame with all 0 of given size
	DataFrame(2D Array) - creates a 2D DataFrame with the given 2D Array
	DataFrame(1D Array) - creates a 1D DataFrame with the given 1D Array

	set(rowIndex, colIndex, value) - sets the value at rowIndex, colIndex to given value

	size() - returns a one dimensional DataFrame of 2 elements [number of rows, number of columns]
	rowLen() - returns the number of rows
	colLen() - returns the number of columns

	T at(int, int) - returns the value at given indices
	rowAt(int) - returns a one dimensional DataFrame of the row at given index
	colAt(int) - returns a one dimensional DataFrame of the column at given index
	subFrame(int, int) - returns a DataFrame starting from the given indices till the last indices
	subFrame(int, int, int, int) - returns a DataFrame starting and ending at given indices

	operator = - copies the given DataFrame to calling DataFrame

	trimTopRow() - removes the row at index 0
	trimBottomRow() - removes the row at index num_rows - 1
	trimRow(int rowIndex) - removes the row with index rowIndex
	trimFirstCol() - removes the column with index 0
	trimLastCol() - removes the column with index num_columns - 1 
	trimCol(int colIndex) - removes the column with index colIndex

	insertRow(array/DataFrame, index) - inserts the given row at the given index
	insertCol(array/DataFrame, index) - inserts the given column at the given index
	appendRow(array/DataFrame) - appends the given array/DataFrame to the end of the calling DataFrame
	appendCol(array/DataFrame) - appends the given array/DataFrame to the end of the calling DataFrame

	transpose() - returns the transpose of calling DataFrame

	Operations on numerical dataframes:
	operator + - returns a sum of DataFrames
	operator - 
	operator *
	If size is incompatible, throws an exception and program is terminated


	normalize() - normalizes the entire DataFrame - subtract the mean and divide by standard deviation/range(max - min)
	normalizeRowWise() - normalizes the entire DataFrame row wise, i.e. each row is normalized individually
	normalizeColWise() - normalizes the entire DataFrame column wise, i.e. each column is normalized individually
	normalizeRow(int) - normalizes the given row
	normalizeCol(int) - normalizes the given column

	<< - displaying the dataframe
	*/
	size_t num_rows, num_columns;
	T **frame;
public:
	//Friend Classes:
	friend class IO;
	DataFrame(){
		frame = NULL;
		num_rows = 0;
		num_columns = 0;
	}
	DataFrame(size_t num_columns){
		this->num_columns = num_columns;
		num_rows = 1;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
	}
	DataFrame(size_t num_rows, size_t num_columns){
		this->num_columns = num_columns;
		this->num_rows = num_rows;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
	}
	DataFrame(size_t num_rows, size_t num_columns, T value){
		this->num_columns = num_columns;
		this->num_rows = num_rows;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
		for(int i=0; i<num_rows; i++)
			for(int j=0; j<num_columns; j++)
				frame[i][j] = value;
	}
	DataFrame(T *array, size_t size){		
		this->num_columns = size;
		num_rows = 1;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
		for(int i=0; i<size; i++) 
			frame[0][i] = array[i];
	}
	DataFrame(T **array, size_t rows, size_t columns){		
		this->num_columns = columns;
		this->num_rows = rows;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
		for(int i=0; i<num_rows; i++) 
			for(int j=0; j<num_columns; j++)
				frame[i][j] = array[i][j];
	}
	DataFrame(const DataFrame &d){
		num_rows = d.num_rows;
		num_columns = d.num_columns;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
		for(int i=0; i<num_rows; i++){
			for(int j=0; j<num_columns; j++)
				frame[i][j] = d.frame[i][j];
		}
	}

	void set(int rowIndex, int columnIndex, T value){
		frame[rowIndex][columnIndex] = value;
	}

	DataFrame<int> size(){
		DataFrame<int> s(2);
		s.set(0, 0, num_rows);
		s.set(0, 1, num_columns);
		return s;
	}

	size_t rowLength(){
		return num_rows;
	}

	size_t columnLength(){
		return num_columns;
	}

	T valueAt(int rowIndex, int columnIndex){
		return frame[rowIndex][columnIndex];
	}

	T max(){
		T max = frame[0][0];
		for(int i=0; i<num_rows; i++)
			for(int j=0; j<num_columns; j++)
				if(frame[i][j] > max) max = frame[i][j];
		return max;
	}

	T min(){
		T min = frame[0][0];
		for(int i=0; i<num_rows; i++)
			for(int j=0; j<num_columns; j++)
				if(frame[i][j] < min) min = frame[i][j];
		return min;
	}

	DataFrame<T> rowAt(int rowIndex){
		DataFrame<T> row(frame[rowIndex], num_columns);
		return row;
	}

	DataFrame<T> columnAt(int columnIndex){
		DataFrame<T> column(num_rows, 1);
		for(int i=0; i<num_rows; i++)
			column.set(i,0,frame[i][columnIndex]);
		return column;
	}

	DataFrame<T> subFrame(int rowIndex, int columnIndex){
		DataFrame<T> subframe( num_rows - rowIndex, num_columns - columnIndex );
		for(int i=rowIndex, m=0; i<num_rows; i++, m++){
			for(int j=columnIndex, n=0; j<num_columns; j++, n++)
				subframe.set(m, n, frame[i][j]);
		}
		return subframe;
	}

	DataFrame<T> subFrame(int rowIndexBeg, int columnIndexBeg, int rowIndexEnd, int columnIndexEnd){
		DataFrame<T> subframe( rowIndexEnd - rowIndexBeg + 1, columnIndexEnd - columnIndexBeg + 1);
		for(int i=rowIndexBeg, m=0; i<rowIndexEnd; i++, m++){
			for(int j=columnIndexBeg, n=0; j<columnIndexEnd; j++, n++)
				subframe.set(m, n, frame[i][j]);
		}
		return subframe;
	}

	void trimTopRow(){
		if(num_rows >= 1){
			for(int i=0; i+1<num_rows; i++){
				for(int j=0; j<num_columns; j++){
					frame[i][j] = frame[i+1][j];
				}
			}
			delete frame[num_rows - 1];
			num_rows--;
			if(num_rows == 0) num_columns = 0;
		}
	}
	void trimBottomRow(){
		if(num_rows >= 1){
			delete frame[num_rows - 1];
			num_rows--;
			if(num_rows == 0) num_columns = 0;
		}
	}
	void trimRow(int rowIndex){
		if(num_rows >= 1){
			if(rowIndex >= 0 && rowIndex < num_rows){
				for(int i=rowIndex; i+1<num_rows; i++){
					for(int j=0; j<num_columns; j++){
						frame[i][j] = frame[i+1][j];
					}
				}
				delete frame[num_rows - 1];
				num_rows--;
				if(num_rows == 0) num_columns = 0;			
			}
		}
	}

	void trimFirstColumn(){
		if(num_columns >= 1){
			for(int j=0; j+1<num_columns; j++){
				for(int i=0; i<num_rows; i++){
					frame[i][j] = frame[i][j+1];
				}
			}
			num_columns--;
			if(num_columns == 0) num_rows = 0;
		}
	}
	void trimLastColumn(){
		if(num_columns >= 1){
			num_columns--;
			if(num_columns == 0) num_rows = 0;			
		}
	}
	void trimColumn(int columnIndex){
		if(num_columns >= 1){
			if(columnIndex >= 0 && columnIndex < num_columns){
				for(int j=columnIndex; j+1<num_columns; j++){
					for(int i=0; i<num_rows; i++){
						frame[i][j] = frame[i][j+1];
					}
				}
				num_columns--;
				if(num_columns == 0) num_rows = 0;				
			}
		}
	}

	DataFrame<T> insertRow(const DataFrame row, int index){
		if(row.num_rows == 1 && row.num_columns == num_columns && index >= 0 && index <= num_rows){
			DataFrame new_data(num_rows + 1, num_columns);

			for(int i=0; i<index; i++)
				for(int j=0; j<num_columns; j++)
					new_data.frame[i][j] = frame[i][j];

			for(int j=0; j<num_columns; j++)
				new_data.frame[index][j] = row.frame[0][j];
			
			for(int i = index+1; i<num_rows + 1; i++)
				for(int j=0; j<num_columns; j++)
					new_data.frame[i][j] = frame[i-1][j];

			return new_data;
		}
	}

	DataFrame<T> insertColumn(const DataFrame column, int index){
		if(column.num_rows == num_rows && column.num_columns == 1 && index >= 0 && index <= num_columns){
			DataFrame new_data(num_rows, num_columns + 1);

			for(int j=0; j<index; j++)
				for(int i=0; i<num_rows; i++)
					new_data.frame[i][j] = frame[i][j];

			for(int i=0; i<num_rows; i++)
				new_data.frame[i][index] = column.frame[i][0];

			for(int j=index+1; j<num_columns + 1; j++)
				for(int i=0; i<num_rows; i++)
					new_data.frame[i][j] = frame[i][j-1];
			
			return new_data;
		}
	}

	DataFrame<T> transpose(){
		DataFrame<T> d(num_columns, num_rows);
		for(int i=0; i<num_rows; i++)
			for(int j=0; j<num_columns; j++)
				d.frame[j][i] = frame[i][j];
		return d;
	}

	void normalize(){
		double mean = 0;
		T max = frame[0][0], min = frame[0][0];
		for(int i=0; i<num_rows; i++){
			for(int j=0; j<num_columns; j++){
				mean += frame[i][j];
				if(frame[i][j] > max) max = frame[i][j];
				if(frame[i][j] < min) min = frame[i][j];
			}
		}
		mean /= num_rows*num_columns;
		double range = max - min;
		if(range==0) range = 1;
		for(int i=0; i<num_rows; i++){
			for(int j=0; j<num_columns; j++){
				frame[i][j] = double(double(frame[i][j]) - mean)/range;
			}
		}				
	}

	void normalizeRowWise(){
		for(int i=0; i<num_rows; i++){
			double mean = 0;
			T max, min = frame[i][0];
			for(int j=0; j<num_columns; j++){
				mean += frame[i][j];
				if(frame[i][j] > max) max = frame[i][j];
				if(frame[i][j] < min) min = frame[i][j];
			}
			mean /= num_columns;
			double range = max - min;
			if(range==0) range = 1;
			for(int j=0; j<num_columns; j++){
				frame[i][j] = double(double(frame[i][j]) - mean)/range;
			}
		}
	}

	void normalizeColumnWise(){
		for(int j=0; j<num_columns; j++){
			double mean = 0;
			T max, min = frame[0][j];
			for(int i=0; i<num_rows; i++){
				mean += frame[i][j];
				if(frame[i][j] > max) max = frame[i][j];
				if(frame[i][j] < min) min = frame[i][j];
			}
			mean /= num_rows;
			double range = max - min;
			if(range==0) range = 1;
			for(int i=0; i<num_rows; i++){
				frame[i][j] = double(double(frame[i][j]) - mean)/range;
			}
		}
	}

	void normalizeRow(int rowIndex){
		double mean = 0;
		T max, min = frame[rowIndex][0];
		for(int j=0; j<num_columns; j++){
			mean += frame[rowIndex][j];
			if(frame[rowIndex][j] > max) max = frame[rowIndex][j];
			if(frame[rowIndex][j] < min) min = frame[rowIndex][j];
		}
		mean /= num_columns;
		double range = max - min;
		if(range==0) range = 1;
		for(int j=0; j<num_columns; j++){
			frame[rowIndex][j] = double(double(frame[rowIndex][j]) - mean)/range;
		}

	}

	void normalizeColumn(int columnIndex){
		double mean = 0;
		T max, min = frame[0][columnIndex];
		for(int i=0; i<num_rows; i++){
			mean += frame[i][columnIndex];
			if(frame[i][columnIndex] > max) max = frame[i][columnIndex];
			if(frame[i][columnIndex] < min) min = frame[i][columnIndex];
		}
		mean /= num_rows;
		double range = max - min;
		if(range==0) range = 1;
		for(int i=0; i<num_rows; i++){
			frame[i][columnIndex] = double(double(frame[i][columnIndex]) - mean)/range;
		}		
	}
/*	DataFrame<double> normalize(){
		DataFrame<double> norm(num_rows, num_columns);
		double mean = 0;
		T max = frame[0][0], min = frame[0][0];
		for(int i=0; i<num_rows; i++){
			for(int j=0; j<num_columns; j++){
				mean += frame[i][j];
				if(frame[i][j] > max) max = frame[i][j];
				if(frame[i][j] < min) min = frame[i][j];
			}
		}
		mean /= num_rows*num_columns;
		double range = max - min;
		for(int i=0; i<num_rows; i++){
			for(int j=0; j<num_columns; j++){
				norm.set(i, j, double(double(frame[i][j]) - mean)/range);
			}
		}		
		return norm;
	}
*/

	DataFrame<T> polynomialFeatures(DataFrame<int> columnIndices, DataFrame<int> columnDegrees){
		if(columnIndices.rowLength() == 1 && columnDegrees.rowLength() == 1 && columnIndices.columnLength() == columnDegrees.columnLength()){
			int extraFeatures = 0;
			for(int i=0; i<columnDegrees.columnLength(); i++)
				extraFeatures += columnDegrees.valueAt(0,i) - 1;
			
			DataFrame<T> polyFeatures(num_rows, num_columns + extraFeatures);
			for(int i=0; i<num_rows; i++)
				for(int j=0; j<num_columns; j++)
					polyFeatures.set(i, j, frame[i][j]);

			for(int i=0, j=num_columns; i<columnIndices.columnLength(); i++)
				for(int k=2; k<=columnDegrees.valueAt(0,i); k++, j++)
					for(int r=0; r<num_rows; r++)
						polyFeatures.set(r, j, pow(frame[r][columnIndices.valueAt(0,i)],k));

			return polyFeatures;
		}
	}

	//Randomizes the frame in the range [min, max)
	void randomizeFrame(int min, int max){
		srand(time(0));
		for(int i=0; i<num_rows; i++)
			for(int j=0; j<num_columns; j++)
				frame[i][j] = rand()%max + min;
	}

	DataFrame<T> operator + (const DataFrame &d){
		DataFrame<T> sum(num_rows, num_columns);
		if(num_rows == d.num_rows && num_columns == d.num_columns){
			for(int i=0; i<num_rows; i++){
				for(int j=0; j<num_columns; j++)
					sum.set(i, j, frame[i][j] + d.frame[i][j]);
			}
		}
		return sum;
	}

	DataFrame<T> operator - (const DataFrame &d){
		DataFrame<T> diff(num_rows, num_columns);
		if(num_rows == d.num_rows && num_columns == d.num_columns){
			for(int i=0; i<num_rows; i++){
				for(int j=0; j<num_columns; j++)
					diff.set(i, j, frame[i][j] - d.frame[i][j]);
			}
		}
		return diff;
	}

	DataFrame<T> operator * (const DataFrame &d){
		DataFrame<T> product(num_rows, d.num_columns);
		if(num_columns == d.num_rows){
			for(int i=0; i<num_rows; i++){
				for(int j=0; j<d.num_columns; j++){
					product.frame[i][j]=0;
					for(int k=0; k<num_columns; k++)
						product.frame[i][j] += frame[i][k]*d.frame[k][j];
				}
			}
		}
		return product;
	}

	void operator = (const DataFrame &d){
		num_rows = d.num_rows;
		num_columns = d.num_columns;
		frame = new T*[num_rows];
		for(size_t i=0; i<num_rows; i++) 
			frame[i] = new T[num_columns];
		for(int i=0; i<num_rows; i++){
			for(int j=0; j<num_columns; j++)
				frame[i][j] = d.frame[i][j];
		}
	}	

	friend ostream& operator << (ostream &out, const DataFrame &d){
		for(int i=0; i<d.num_rows; i++){ 
			for(int j=0; j<d.num_columns; j++)
				out<<d.frame[i][j]<<" ";
			out<<endl;
		}
		return out;
	}
};

template <typename T>
class LinearRegression{
	DataFrame<T> theta;
public:
	void fit(DataFrame<T> X, DataFrame<T> y); //Fits the model to given dataframe of X and y
	void fit(DataFrame<T> X, DataFrame<T> y, double alpha, long long int num_iter){
		DataFrame<T> th(X.columnLength(), 1);
		th.randomizeFrame(0,5);
		DataFrame<T> hypothesis(X.rowLength(), 1);
		DataFrame<T> X_transpose(X.transpose());
		DataFrame<T> C(X.columnLength(), X.columnLength(), (alpha/X.rowLength()) );
		for(int iter=0; iter<num_iter; iter++){
			hypothesis = X*th;
			th = th - (C*(X_transpose*(hypothesis - y)));
		}
		this->theta = th;
	}

	//Predicts the outcome for the given dataframe X
	DataFrame<T> predict(DataFrame<T> X){
		return X*theta;
	} 
	DataFrame<T> parameterVector(){
		return theta;
	}
};

int main(int argc, char **argv){

	double alpha = atof(argv[0]);
	int num_iter = atoi(argv[1]);

	DataFrame<double> X(20,1);
	DataFrame<double> y(20,1);
	for(int i=0; i<20; i++){
		//X.set(i,0,1);
		X.set(i,0,i);
		y.set(i,0,i+2+pow(i,2));
	}
	int index_arr[] = {0}, deg[] = {2};
	DataFrame<int> indices(index_arr, 1), degrees(deg, 1);
	X = X.polynomialFeatures(indices, degrees);

	//Normalize:
	X.normalize();
	//y.normalize();

	DataFrame<double> ones(X.rowLength(), 1, 1);
	X = X.insertColumn(ones, 0);
	DataFrame<double> dataset(X);
	dataset = dataset.insertColumn(y, 3);
	cout<<endl<<dataset;
	LinearRegression<double> regressor;
	regressor.fit(X, y, alpha, num_iter);
	cout<<endl<<"Parameter vector: "<<endl<<regressor.parameterVector();
	cout<<endl<<"Prediction: "<<endl<<regressor.predict(X);

/*
	
	DataFrame<int> d(3,4);
	for (int i = 0; i < 3; ++i)
	{
		for(int j=0; j<4; j++){
			d.set(i, j, j+1);
		}
	}

	cout<<endl<<d;

	DataFrame<int> column(3,1);
	for(int i=0; i<3; i++) column.set(i, 0, 1);

	DataFrame<int> row(1,5);
	for(int i=0; i<5; i++) row.set(0, i, 9);

	d = d.insertColumn(column, 0);
	cout<<endl<<d;
	cout<<endl<<d.size();

	d = d.insertRow(row, 3);
	cout<<endl<<d;
	cout<<endl<<d.size();

	/*DataFrame<int> indices(1,2), degrees(1,2);
	indices.set(0,0,1);
	indices.set(0,1,2);
	degrees.set(0,0,5);
	degrees.set(0,1,10);

	cout<<endl<<d.polynomialFeatures(indices, degrees);

	
	IO Testing
	
	IO write;
	DataFrame<double> X(200,130);
	
	write.csv_write<double>("io_test.csv", 'w', X);
	*/
	/* DataFrame Testing
	DataFrame<int> d1(10), d2(5,5);
	int arr[3] = {1,2,3};

	int **ar = new int*[3];
	for(int i=0; i<3; i++)
		ar[i] = new int[4];

	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++)
			ar[i][j] = i;
	}

	double **dar = new double*[3];
	for(int i=0; i<3; i++)
		dar[i] = new double[4];

	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++)
			dar[i][j] = i;
	}


	DataFrame<int> d3(arr, 3);
	DataFrame<double> d4(dar, 3, 4);
	
	cout<<d1;
	cout<<endl<<"Size: "<<d1.size();
	cout<<endl;
	cout<<d2;
	cout<<endl<<"Size: "<<d2.size();
	cout<<endl;
	cout<<d3;
	cout<<endl<<"Size: "<<d3.size();
	cout<<endl;
	cout<<d4;
	cout<<endl<<"Size: "<<d4.size();

	DataFrame<double> d5(d4);
	cout<<endl;
	cout<<d5;
	cout<<endl<<"Size: "<<d5.size();

	cout<<endl<<"Rows: ";
	for(int i=0; i<3; i++){
		cout<<endl<<d5.rowAt(i);
		cout<<endl<<"Size: "<<d5.rowAt(i).size();
	}

	cout<<endl<<"Columns: ";
	for(int i=0; i<4; i++){
		cout<<endl<<d5.columnAt(i);
		cout<<endl<<"Size: "<<d5.columnAt(i).size();
	}

	cout<<endl<<"Subframe 0,0:"<<endl<<d5.subFrame(0,0);
	cout<<endl<<"Subframe 1,1:"<<endl<<d5.subFrame(1,1);
	cout<<endl<<"Subframe 2,2:"<<endl<<d5.subFrame(2,2);

	DataFrame<double> t;
	t = d5.subFrame(0,0);
	cout<<endl<<"T: "<<endl<<t;

	cout<<endl<<"d4 transpose: "<<endl<<d4.transpose()<<endl<<"Size: "<<d4.transpose().size();
	cout<<endl<<"d4 * d4"<<endl<<d4*d4.transpose();
	cout<<endl<<"d4 + d4"<<endl<<d4+d4;
	cout<<endl<<"d4 - d4"<<endl<<d4-d4;
	d4.normalize();
	cout<<endl<<"d4 normalized:\n"<<d4;


	d5.normalizeColumn(1);
	d5.normalizeRow(2);
	cout<<endl<<"d5 norm: "<<endl<<d5;
	cout<<endl<<d5.rowAt(2);
	cout<<endl<<d5.rowAt(2).max();
	cout<<endl<<d5.rowAt(2).min();
	cout<<endl<<d5.columnAt(2);
	cout<<endl<<d5.columnAt(2).max();
	cout<<endl<<d5.columnAt(2).min();

	cout<<endl<<"d5:\n"<<d5<<endl<<"Size: "<<d5.size();
	d5.trimTopRow();
	cout<<endl<<"d5:\n"<<d5<<endl<<"Size: "<<d5.size();
	d5.trimRow(1);
	cout<<endl<<"d5:\n"<<d5<<endl<<"Size: "<<d5.size();
	d5.trimBottomRow();
	cout<<endl<<"d5:\n"<<d5<<endl<<"Size: "<<d5.size();
	d5.trimTopRow();

	cout<<endl<<"d4:\n"<<d4<<endl<<"Size: "<<d4.size();
	d4.trimFirstColumn();
	cout<<endl<<"d4:\n"<<d4<<endl<<"Size: "<<d4.size();
	d4.trimColumn(1);
	cout<<endl<<"d4:\n"<<d4<<endl<<"Size: "<<d4.size();
	d4.trimLastColumn();
	cout<<endl<<"d4:\n"<<d4<<endl<<"Size: "<<d4.size();
	d4.trimColumn(0);
	cout<<endl<<"d4:\n"<<d4<<endl<<"Size: "<<d4.size();
 	*/

	return 0;
}


/*
class IO{
public:
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
	template <typename T>
	void csv_write(const char *file_name, char mode, DataFrame<T> data){
		std::ofstream file; //Output file stream object, used to handle the file.
		if(mode == 'a') file.open(file_name, std::ios::app); //Mode 'a' == append to file.
		else if(mode == 'w') file.open(file_name, std::ios::out); //Mode 'w' == write to file.
		else return; //Invalid mode
		if(file.is_open()){
			for(int i=0;i<data.num_rows;i++){
				for(int j=0;j<data.num_columns;j++){
					if(j==data.num_columns-1) file<<std::setprecision(10)<<data.frame[i][j];
					else file<<std::setprecision(10)<<data.frame[i][j]<<",";
				}
				file<<std::endl;
			}
			file.close(); //Close the current file.
			return; //Return successful operation.
		}
		return; //Error in opening the file.
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
	
	template <typename T>
	DataFrame<T> csv_read(const char *file_name, int mode){
		//Initialize
		long int num_rows = 0;
		long int num_columns = 0;
		std::string value; //Stores the line read from the file.
		char *token; //Stores comma seperated values in a line.

		vector<T> array;

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
							if(!mode) array.push_back((double)atof(token)); //Convert value to double if numerical mode is selected.
							else array.push_back((char*)(*token));
						}
						token = strtok(NULL,",");
						num_columns++; 
					}
					num_rows++;
				}
			}
			file.close(); //Close the file.
			DataFrame<T> data(num_rows, num_columns);
			for(int i=0; i<num_rows; i++){
				for(int j=0; j<num_columns; j++){
					data.frame[i][j] = array[i + j*num_rows];
				}
			}
			return 1; //Successful operation.
		}
		return 0; //Error in opening file.
	}

};
*/
