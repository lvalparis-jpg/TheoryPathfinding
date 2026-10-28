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
            grid[i][j] = new Node();
            grid[i][j]->SetX(i);
            grid[i][j]->SetY(j);
            grid[i][j]->SetIsWall(false);
            grid[i][j]->SetIsVisited(false);
        }
    }
}

Grid::~Grid()
{
    for (auto& line : grid)
        for (Node* n : line)
            delete n;
}

std::vector<Node*>& Grid::operator[](size_t row)
{
	return grid[row];
}
