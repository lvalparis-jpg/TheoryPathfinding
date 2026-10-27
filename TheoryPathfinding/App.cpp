#include "App.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <queue>

App::App()
{
    DrawGrid();
    p1;
    c1;
}

App::~App()
{
}

void App::Update()
{
    HandleInput();
    Draw();
}

void App::Draw()
{
    system("cls");
    DrawGrid();
}

void App::DrawGrid()
{
    for (int i = 0; i < grid.GetLineSize(); i++)
    {
        for (int j = 0; j < grid.GetRowSize(); j++)
        {
            if (i == p1.GetxP() && j == p1.GetyP() && i == c1.GetxC() && j == c1.GetyC())
            {
                std::cout << "[" << RED << "O" << White << "]";
            }
            else if (i == p1.GetxP() && j == p1.GetyP())
            {
                std::cout << "[O]";
            }
            else if (i == c1.GetxC() && j == c1.GetyC())
            {
                std::cout << "[" << RED << " " << White <<"]";
            }
            else
            {
                std::cout << "[ ]" ;
            }
        }
        std::cout << std::endl;
    }
	std::cout << std::endl;
	std::cout << deplacement << std::endl;
}

void App::HandleInput()
{
    int k =_getch();
    
    switch (k)
    {
    case'z':
        if (c1.GetxC() != 0)
        {
            c1.SetxC(c1.GetxC() - 1);
            path.push_back(std::make_pair(c1.GetxC(), c1.GetyC()));
			deplacement++;
        }
        break;
    case's':
        if (c1.GetxC() != 9)
        {
            c1.SetxC(c1.GetxC() + 1);
            path.push_back(std::make_pair(c1.GetxC(), c1.GetyC()));
            deplacement++;
        }
        break;
    case'q':
        if (c1.GetyC() != 0)
        {
            c1.SetyC(c1.GetyC() - 1);
            path.push_back(std::make_pair(c1.GetxC(), c1.GetyC()));
            deplacement++;
        }
        break;
    case'd':
        if (c1.GetyC() != 9)
        {
            c1.SetyC(c1.GetyC() + 1);
            path.push_back(std::make_pair(c1.GetxC(), c1.GetyC()));
            deplacement++;
        }
        break;
    case ENTER:
        BFS(std::make_pair(p1.GetxP(), p1.GetyP()),std::make_pair(c1.GetxC(),c1.GetyC()));
		for (int i = 0; i < path.size(); i++)
		{
            deplacement--;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			p1.SetxP(path[i].first);
			p1.SetyP(path[i].second);
			Draw();
		}
        break;
    }
}

void App::BFS(std::pair<int, int> start, std::pair<int, int> target)
{
    int lines = grid.GetLineSize();
    int rows = grid.GetRowSize();

    std::queue<std::pair<int, int>> toVisit;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<std::pair<int, int>>> parent;

    visited.resize(lines);
    parent.resize(lines);

    for (int i = 0; i < lines; i++)
    {
        visited[i].resize(rows);
        parent[i].resize(rows);

        for (int j = 0; j < rows; j++)
        {
            visited[i][j] = false;
            parent[i][j] = std::make_pair(-1, -1);
        }
    }

    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    toVisit.push(start);

    visited[start.first][start.second] = true;

    while (toVisit.empty() == false)
    {
        std::pair<int, int> current = toVisit.front();
        toVisit.pop();

        if (current == target)
            break;

        for (int d = 0; d < 4; d++)
        {
            int nx = current.first + dx[d];
            int ny = current.second + dy[d];
            
            if (nx > 9 || nx < 0)
                continue;
            else if (ny > 9 || ny < 0)
                continue;

            if (visited[nx][ny])
                continue;
            
            visited[nx][ny] = true;
            parent[nx][ny] = current;
            toVisit.push({ nx, ny });
        }
    }

    std::vector<std::pair<int, int>> result;

    if (visited[target.first][target.second] == false)
        std::cout << "Inatteignable" << std::endl;

    for (std::pair<int, int> cur = target; cur != start; cur = parent[cur.first][cur.second])
        result.push_back(cur);

    std::reverse(result.begin(), result.end());

    deplacement = result.size();
    path = result;
}
