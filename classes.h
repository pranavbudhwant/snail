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
	DataFrame operator + (const DataFrame &d){
		DataFrame sum(num_rows, num_columns);
		if(num_rows == d.num_rows && num_columns == d.num_columns){
			for(int i=0; i<num_rows; i++){
				for(int j=0; j<num_columns; j++)
					sum.set(i, j, frame[i][j] + d.frame[i][j]);
			}
		}
		return sum;
	}

	DataFrame operator - (const DataFrame &d){
		DataFrame diff(num_rows, num_columns);
		if(num_rows == d.num_rows && num_columns == d.num_columns){
			for(int i=0; i<num_rows; i++){
				for(int j=0; j<num_columns; j++)
					diff.set(i, j, frame[i][j] - d.frame[i][j]);
			}
		}
		return diff;
	}

	DataFrame operator * (const DataFrame &d){
		DataFrame product(num_rows, d.num_columns);
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

class IO{/*
	csv_read() - reads the given csv file and returns a DataFrame object
	csv_write() - writes the given DataFrame to the given file
	*/
};