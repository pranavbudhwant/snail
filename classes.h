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

	trimRowTop() - removes the row at index 0
	trimRowBottom() - removes the row at index num_rows - 1
	trimRow(int rowIndex) - removes the row with index rowIndex
	trimColTop() - removes the column with index 0
	trimColBottom() - removes the column with index num_columns - 1 
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

	DataFrame size(){
		DataFrame s(2);
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

	DataFrame rowAt(int rowIndex){
		DataFrame row(frame[rowIndex], num_columns);
		return row;
	}

	DataFrame columnAt(int columnIndex){
		DataFrame column(num_rows, 1);
		for(int i=0; i<num_rows; i++)
			column.set(i,0,frame[i][columnIndex]);
		return column;
	}

	DataFrame subFrame(int rowIndex, int columnIndex){
		DataFrame subframe( num_rows - rowIndex, num_columns - columnIndex );
		for(int i=rowIndex, m=0; i<num_rows; i++, m++){
			for(int j=columnIndex, n=0; j<num_columns; j++, n++)
				subframe.set(m, n, frame[i][j]);
		}
		return subframe;
	}

	DataFrame subFrame(int rowIndexBeg, int columnIndexBeg, int rowIndexEnd, int columnIndexEnd){
		DataFrame subframe( rowIndexEnd - rowIndexBeg + 1, columnIndexEnd - columnIndexBeg + 1);
		for(int i=rowIndexBeg, m=0; i<rowIndexEnd; i++, m++){
			for(int j=columnIndexBeg, n=0; j<columnIndexEnd; j++, n++)
				subframe.set(m, n, frame[i][j]);
		}
		return subframe;
	}

	friend ostream& operator << (ostream &out, const DataFrame &d){
		for(int i=0; i<d.num_rows; i++){ 
			for(int j=0; j<d.num_columns; j++)
				out<<d.frame[i][j]<<" ";
			out<<endl;
		}
		return out;
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

};

class IO{/*
	csv_read() - reads the given csv file and returns a DataFrame object
	csv_write() - writes the given DataFrame to the given file
	*/
};