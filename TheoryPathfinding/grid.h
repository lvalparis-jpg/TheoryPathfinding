#pragma once
#include <iostream>
#include <vector>

class Grid
{
public:

	Grid();
	~Grid();

	//Set

	//Get
	int GetRowSize() { return row; }
	int GetLineSize() { return row; }

	//surcharge
	std::vector<bool>& operator[](size_t row);

private:

	std::vector<std::vector<bool>> grid;
	int row = 10;
	int lines = 10;
};

