#pragma once
#include <iostream>
#include <vector>

#include "Node.h"

class Grid
{
public:

	Grid();
	~Grid();

	//Set
	void SetRowSize(int value) { row = value; }
	void SetLineSize(int value) { lines = value; }

	//Get
    int GetRowSize() const { return row; }
    int GetColSize() const { return lines; }

	//surcharge
	std::vector<Node*>& operator[](size_t row);

private:

	std::vector<std::vector<Node*>> grid;

	int row = 33;
	int lines = 33;
};

