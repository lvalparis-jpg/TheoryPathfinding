#include "App.h"
#include <conio.h>

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
}

void App::HandleInput()
{
    int k =_getch();
    
    switch (k)
    {
    case'z':
        if(c1.GetxC() != 0)
            c1.SetxC(c1.GetxC() - 1);
        break;
    case's':
        if (c1.GetxC() != 9)
            c1.SetxC(c1.GetxC() + 1);
        break;
    case'q':
        if (c1.GetyC() != 0)
            c1.SetyC(c1.GetyC() - 1);
        break;
    case'd':
        if (c1.GetyC() != 9)
            c1.SetyC(c1.GetyC() + 1);
        break;
    case ENTER:
        p1.SetxP(c1.GetxC());
        p1.SetyP(c1.GetyC());
        break;
    }
}