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
    for (int i = 0; i < grid.GetColSize(); i++)
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
            else if (grid[i][j]->GetIsWall() == true && i == c1.GetxC() && j == c1.GetyC())
            {
                std::cout << "[" << RED << "H" << White << "]";
            }
            else if (grid[i][j]->GetIsWall() == true)
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

    Node* tempNode1 = new Node();
    Node* tempNode2 = new Node();

    switch (k)
    {
    case'z':
        if (c1.GetxC() != 0)
        {
            c1.SetxC(c1.GetxC() - 1);
            tempNode1->SetX(c1.GetxC());
            tempNode1->SetY(c1.GetyC());
            path.push_back(tempNode1);
			deplacement++;
        }
        break;
    case's':
        if (c1.GetxC() != grid.GetRowSize() - 1)
        {
            c1.SetxC(c1.GetxC() + 1);
            tempNode1->SetX(c1.GetxC());
            tempNode1->SetY(c1.GetyC());
            path.push_back(tempNode1);
            deplacement++;
        }
        break;
    case'q':
        if (c1.GetyC() != 0)
        {
            c1.SetyC(c1.GetyC() - 1);
            tempNode1->SetX(c1.GetxC());
            tempNode1->SetY(c1.GetyC());
            path.push_back(tempNode1);
            deplacement++;
        }
        break;
    case'd':
        if (c1.GetyC() != grid.GetColSize() - 1)
        {
            c1.SetyC(c1.GetyC() + 1);
            tempNode1->SetX(c1.GetxC());
            tempNode1->SetY(c1.GetyC());
            path.push_back(tempNode1);
            deplacement++;
        }
        break;
    case ENTER:
        attainable = true;

        tempNode1->SetX(p1.GetxP());
        tempNode1->SetY(p1.GetyP());
        tempNode2->SetX(c1.GetxC());
        tempNode2->SetY(c1.GetyC());

        if (ActivateCost)
        {
            Djikstra(tempNode1, tempNode2);
        }
        else
        {
            BFS(tempNode1,tempNode2);
        }

        if (attainable)
        {
            for (int i = 0; i < path.size(); i++)
            {
                deplacement--;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                p1.SetxP(path[i]->GetX());
                p1.SetyP(path[i]->GetY());
                Draw();
            }
        }
        break;
    case ESCAPE:
        exit(0);
    }
}

void App::BFS(Node* start, Node* target)
{
    int lines = grid.GetColSize();
    int rows = grid.GetRowSize();

    std::queue<Node*> toVisit;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<Node*>> parent;

    visited.resize(lines);
    parent.resize(lines);

    Node* tempNode = new Node();

    for (int i = 0; i < lines; i++)
    {
        visited[i].resize(rows);
        parent[i].resize(rows);

        for (int j = 0; j < rows; j++)
        {
            visited[i][j] = false;
            parent[i][j]->SetX(-1);
            parent[i][j]->SetY(-1);
        }
    }

    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    toVisit.push(start);

    visited[start->GetX()][start->GetY()] = true;

    while (toVisit.empty() == false)
    {

        Node* current = toVisit.front();
        toVisit.pop();

        if (current == target)
            break;

        for (int d = 0; d < 4; d++)
        {

            int nx = current->GetX() + dx[d];
            int ny = current->GetY() + dy[d];

            //verifie si hors limite
            if (nx > rows - 1 || nx < 0)
                continue;
            else if (ny > lines - 1 || ny < 0)
                continue;

            //verifie mur
            if (grid[nx][ny]->GetIsWall() == true)
                continue;

            //verifie si deja passer
            if (visited[nx][ny])
                continue;

            visited[nx][ny] = true;
            parent[nx][ny] = current;
            tempNode->SetX(nx);
            tempNode->SetY(ny);
            toVisit.push(tempNode);
        }
    }

    std::vector<Node*> result;

    if (visited[target->GetX()][target->GetY()] == false)
    {
        attainable = false;
        return;
    }

    for (Node* cur = target; cur != start; cur = parent[cur->GetX()][cur->GetY()])
        result.push_back(cur);

    std::reverse(result.begin(), result.end());

    deplacement = result.size();
    path = result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::Djikstra(Node* start, Node* target)
{
    int lines = grid.GetColSize();
    int rows = grid.GetRowSize();

    std::vector<Node*> toVisit;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<Node*>> parent;

    visited.resize(lines);
    parent.resize(lines);

    Node* tempNode = new Node();

    for (int i = 0; i < lines; i++)
    {
        visited[i].resize(rows);
        parent[i].resize(rows);

        for (int j = 0; j < rows; j++)
        {
            visited[i][j] = false;
            parent[i][j]->SetX(-1);
            parent[i][j]->SetY(-1);
        }
    }

    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    toVisit.push_back(start);

    visited[start->GetX()][start->GetY()] = true;

    int minCost = 0;

    while (toVisit.empty() == false)
    {

        Node* current = toVisit.front();
        toVisit.pop_back();

        if (current == target)
            break;
        
        for (int d = 0; d < 4; d++)
        {

            int nx = current->GetX() + dx[d];
            int ny = current->GetY() + dy[d];

            //verifie si hors limite
            if (nx > rows - 1 || nx < 0)
                continue;
            else if (ny > lines - 1 || ny < 0)
                continue;

            //verifie mur
            if (grid[nx][ny]->GetIsWall() == true)
                continue;

            //verifie si deja passer
            if (visited[nx][ny])
                continue;

            visited[nx][ny] = true;
            parent[nx][ny] = current;
            cost = cost + current->GetLevel();
            tempNode->SetX(nx);
            tempNode->SetY(ny);
            toVisit.push_back(tempNode);
        }
    }

    std::vector<Node*> result;

    if (visited[target->GetX()][target->GetY()] == false)
    {
        attainable = false;
        return;
    }

    for (Node* cur = target; cur != start; cur = parent[cur->GetX()][cur->GetY()])
        result.push_back(cur);

    std::reverse(result.begin(), result.end());

    deplacement = result.size();
    path = result;
}

void App::InitObstacle()
{
    for (int i = 4; i < 7;i++)
    {
        grid[i][5]->SetIsWall(true);
    }
    for (int i = 2; i < 5;i++)
    {
        grid[8][i]->SetIsWall(true);
    }
    for (int i = 0; i < 5;i++)
    {
        grid[i][3]->SetIsWall(true);
    }
    for (int i = 0; i < 8;i++)
    {
        grid[i][7]->SetIsWall(true);
    }
    for (int i = 0; i < 2;i++)
    {
        grid[1][i]->SetIsWall(true);
    }
    for (int i = 7; i < 10;i++)
    {
        grid[1][i]->SetIsWall(true);
    }
    for (int i = 1; i < 3;i++)
    {
        grid[3][i]->SetIsWall(true);
    }
}
