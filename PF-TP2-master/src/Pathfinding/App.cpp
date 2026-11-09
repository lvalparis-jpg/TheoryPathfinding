#include "App.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <queue>
#include <algorithm>
#include <limits>

App::App()
{
    p1;
    c1;
    /*InitObstacle();*/
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
                std::cout << "[" << Ansi::Red << "O" << Ansi::White << "]";
            }
            else if (i == p1.GetxP() && j == p1.GetyP())
            {
                std::cout << "[O]";
            }
            else if (i == c1.GetxC() && j == c1.GetyC())
            {
                std::cout << "[" << Ansi::Red << " " << Ansi::White <<"]";
            }
            else if (grid[i][j]->GetIsWall() == true && i == c1.GetxC() && j == c1.GetyC())
            {
                std::cout << "[" << Ansi::Red << "H" << Ansi::White << "]";
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
    case KEY_ENTER:
        attainable = true;

        tempNode1->SetX(p1.GetxP());
        tempNode1->SetY(p1.GetyP());
        tempNode2->SetX(c1.GetxC());
        tempNode2->SetY(c1.GetyC());

        if (ActivateCost)
        {
            Djikstra(grid,tempNode1, tempNode2);
        }
        else
        {
            BFS(grid,tempNode1,tempNode2);
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
    case KEY_ESCAPE:
        exit(0);
    }
}

static std::vector<Node*> RebuildPath(const std::vector<std::vector<Node*>>& parent,
    Node* s, Node* t)
{
    std::vector<Node*> result;
    for (Node* cur = t; cur != s; cur = parent[cur->GetX()][cur->GetY()])
        result.push_back(cur);
    std::reverse(result.begin(), result.end());
    return result;
}

void App::BFS(Grid& g, Node* start, Node* target)
{
    attainable = true;
    path.clear();
    deplacement = 0;

    const int lines = g.GetColSize();
    const int cols = g.GetRowSize();

    Node* s = g[start->GetX()][start->GetY()];
    Node* t = g[target->GetX()][target->GetY()];

    std::vector<std::vector<bool>>  visited(lines, std::vector<bool>(cols, false));//Crée un tableau de tableau de booléens pour savoir si on a déjà visité une case // j'adore cette feature 
    std::vector<std::vector<Node*>> parent(lines, std::vector<Node*>(cols, nullptr));//Crée un tableau de tableau de pointeurs vers les parents pour reconstruire le chemin
    std::queue<Node*> toVisit;

    const int dx[4] = { -1, 1, 0, 0 };
    const int dy[4] = { 0, 0, -1, 1 };

    toVisit.push(s);
    visited[s->GetX()][s->GetY()] = true;

    while (!toVisit.empty())
    {
        Node* current = toVisit.front();
        toVisit.pop();

        if (current == t)
            break;

        for (int d = 0; d < 4; d++)
        {
            int nx = current->GetX() + dx[d];
            int ny = current->GetY() + dy[d];

            if (nx < 0 || nx >= lines || ny < 0 || ny >= cols)
                continue; // hors limites

            if (g[nx][ny]->GetIsWall())
                continue; // mur

            if (g[nx][ny]->GetIsBlocked())
                continue; //case prise par agent

            if (visited[nx][ny])
                continue; // déjà vu

            visited[nx][ny] = true;
            parent[nx][ny] = current;
            toVisit.push(g[nx][ny]);
        }
    }

    if (!visited[t->GetX()][t->GetY()])
    {
        attainable = false;
        return;
    }

    path = RebuildPath(parent, s, t);
    deplacement = static_cast<int>(path.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::Djikstra(Grid& g, Node* start, Node* target)
{
    attainable = true;
    path.clear();
    deplacement = 0;
    cost = 0;

    const int lines = g.GetColSize();
    const int cols = g.GetRowSize();

    constexpr int INF = std::numeric_limits<int>::max();// Valeur maximale d'un int pour représenter l'infini

    Node* s = g[start->GetX()][start->GetY()];
    Node* t = g[target->GetX()][target->GetY()];

    std::vector<std::vector<int>>   dist(lines, std::vector<int>(cols, INF));
    std::vector<std::vector<Node*>> parent(lines, std::vector<Node*>(cols, nullptr));

    // File de priorité : le plus petit coût sort en premier

    using QueueItem = std::pair<int, Node*>;   // (coût, case)
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> toVisit;

    const int dx[4] = { -1, 1, 0, 0 };
    const int dy[4] = { 0, 0, -1, 1 };

    dist[s->GetX()][s->GetY()] = 0;
    toVisit.push({ 0, s });

    while (!toVisit.empty())
    {
        auto [d, current] = toVisit.top();
        toVisit.pop();

        //on a déjà trouvé mieux pour ce nœud
        if (d > dist[current->GetX()][current->GetY()])
            continue;

        if (current == t)
            break;

        for (int i = 0; i < 4; i++)
        {
            int nx = current->GetX() + dx[i];
            int ny = current->GetY() + dy[i];

            if (nx < 0 || nx >= lines || ny < 0 || ny >= cols)//hors limites
                continue;

            if (g[nx][ny]->GetIsWall())//mur
                continue;

            if (g[nx][ny]->GetIsBlocked())//agent
                continue;

            int nd = d + g[nx][ny]->GetLevel() + 1;

            if (nd < dist[nx][ny])      // meilleur chemin trouvé
            {
                dist[nx][ny] = nd;

                parent[nx][ny] = current;

                toVisit.push({ nd, g[nx][ny] });
            }
        }
    }

    if (dist[t->GetX()][t->GetY()] == INF)
    {
        attainable = false;
        return;
    }

    cost = dist[t->GetX()][t->GetY()];
    path = RebuildPath(parent, s, t);
    deplacement = static_cast<int>(path.size());
}
