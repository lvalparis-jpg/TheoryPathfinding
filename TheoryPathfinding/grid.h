#pragma once
#include <iostream>
#include <vector>

class Grid
{
public:

	Grid();
	~Grid();

	//Set
	void SetRowSize(int value) { row = value; }
	void SetLineSize(int value) { lines = value; }

	//Get
	int GetRowSize() { return row; }
	int GetLineSize() { return lines; }

	//surcharge
	std::vector<bool>& operator[](size_t row);

private:

	std::vector<std::vector<bool>> grid;

	int row = 20;
	int lines = 20;
};

