template <typename T>
class DataFrame{

	//Functions to be written:
	
	DataFrame() - constructor - creates an empty DataFrame
	DataFrame(DataFrame) - copy constructor
	DataFrame(2D Array) - creates a 2D DataFrame with the given 2D Array
	DataFrame(1D Array) - creates a 1D DataFrame with the given 1D Array


	size() - returns a one dimensional DataFrame of 2 elements [number of rows, number of columns]
	rowLen() - returns the number of rows
	colLen() - returns the number of columns

	T at(int, int) - returns the value at given indices
	T rowAt(int) - returns a one dimensional DataFrame of the row at given index
	T colAt(int) - returns a one dimensional DataFrame of the column at given index
	
	operator = - copies the given DataFrame to calling DataFrame
	copy(int rowStart, int colStart) - copies the sub-dataframe from rowStart, colStart to nRows and nCols of given DataFrame to calling DataFrame
	copy(int rowStart, int colStart, int rowEnd, int colEnd) - copies sub-dataframe from rowStart, colStart to rowEnd, colEnd of given DataFrame to calling DataFrame

	trimRow() - removes the row at index 0
	trimRow(int rowIndex) - removes the row with index rowIndex
	trimCol() - removes the column with index 0
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


};

class IO{
	csv_read() - reads the given csv file and returns a DataFrame object
	csv_write() - writes the given DataFrame to the given file
};