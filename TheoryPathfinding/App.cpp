#include "App.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <queue>

App::App()
{
    p1;
    c1;
    InitObstacle();
    DrawGrid();
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
            else if (grid[i][j] == true && i == c1.GetxC() && j == c1.GetyC())
            {
                std::cout << "[" << RED << "H" << White << "]";
            }
            else if (grid[i][j] == true)
            {
                std::cout << "[#]";
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
    if (attainable == false)
    {
        std::cout <<"Inatteignable"<< std::endl;
    }
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
        if (c1.GetxC() != grid.GetRowSize() - 1)
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
        if (c1.GetyC() != grid.GetLineSize() - 1)
        {
            c1.SetyC(c1.GetyC() + 1);
            path.push_back(std::make_pair(c1.GetxC(), c1.GetyC()));
            deplacement++;
        }
        break;
    case ENTER:
        attainable = true;
        BFS(std::make_pair(p1.GetxP(), p1.GetyP()),std::make_pair(c1.GetxC(),c1.GetyC()));
        if (attainable)
        {
            for (int i = 0; i < path.size(); i++)
            {
                deplacement--;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                p1.SetxP(path[i].first);
                p1.SetyP(path[i].second);
                Draw();
            }
        }
        break;
    case ESCAPE:
        exit(0);
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
            
            //verifie si hors limite
            if (nx > rows - 1 || nx < 0)
                continue;
            else if (ny > lines - 1 || ny < 0)
                continue;

            //verifie mur
            if (grid[nx][ny] == true)
                continue;

            //verifie si deja passer
            if (visited[nx][ny])
                continue;
            
            visited[nx][ny] = true;
            parent[nx][ny] = current;
            toVisit.push({ nx, ny });
        }
    }

    std::vector<std::pair<int, int>> result;

    if (visited[target.first][target.second] == false)
    {
        attainable = false;
        return;
    }

    for (std::pair<int, int> cur = target; cur != start; cur = parent[cur.first][cur.second])
        result.push_back(cur);

    std::reverse(result.begin(), result.end());

    deplacement = result.size();
    path = result;
}

void App::InitObstacle()
{
    for (int i = 4; i < 7;i++)
    {
        grid[i][5] = true;
    }
    for (int i = 2; i < 5;i++)
    {
        grid[8][i] = true;
    }
    for (int i = 0; i < 5;i++)
    {
        grid[i][3] = true;
    }
    for (int i = 0; i < 8;i++)
    {
        grid[i][7] = true;
    }
    for (int i = 0; i < 2;i++)
    {
        grid[1][i] = true;
    }
    for (int i = 7; i < 10;i++)
    {
        grid[1][i] = true;
    }
    for (int i = 1; i < 3;i++)
    {
        grid[3][i] = true;
    }
}