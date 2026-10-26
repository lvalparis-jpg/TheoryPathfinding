#include "grid.h"
#include <corecrt_malloc.h>
#include <cstdlib>
#include <iostream>

Grid::Grid()
{
	grid.resize(lines);
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].resize(row);
        for (int j = 0; j < grid.size(); j++)
        {
            grid[i][j] = false;
        }
    }
}

Grid::~Grid()
{

}

std::vector<bool>& Grid::operator[](size_t row)
{
	return grid[row];
}
