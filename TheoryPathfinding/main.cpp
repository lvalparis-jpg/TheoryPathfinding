#include <iostream>
#include <cstdlib>
#include "grid.h"
#include "cell.h"

int main()
{
    srand(time(NULL));
    bool replay = true;

    while (replay)
    {
        Grid grid;
        CreateTab(&grid);

        int x;
        int y;
        bool Play = true;
        bool Win = false;

        std::cout << "Entrez des coordonnees : " << std::endl;
        std::cout << "x ? : " << std::endl;
        std::cin >> x;

        while (x >= grid.line || x < 0)
        {
            std::cout << "Erreur nouvelles coordonnees requises " << std::endl;
            std::cout << "x ? : " << std::endl;
            std::cin >> x;
        }

        std::cout << "y ? : " << std::endl;
        std::cin >> y;
        while (y >= grid.column || y < 0)
        {
            std::cout << "Erreur nouvelles coordonnees requises " << std::endl;
            std::cout << "y ? : " << std::endl;
            std::cin >> y;
        }

        grid.celltab[x][y].reveal = true;
        grid.celltab[x][y].mine = false;

        InitMine(&grid,x, y);
        InitNb(&grid);
        RevealGrid(&grid, x, y);

        while (Play == true)
        {
            DisplayTab(&grid);

            std::cout << "Entrez les coordonnees de la case a reveler ou a marquer :" << std::endl;
            std::cout << "x ? : ";
            std::cin >> x;
            std::cout << "y ? : ";
            std::cin >> y;

            while (x < 0 || x >= grid.line || y < 0 || y >= grid.column)
            {
                std::cout << "Coordonnees invalides. Reessayez." << std::endl;
                std::cout << "x ? : ";
                std::cin >> x;
                std::cout << "y ? : ";
                std::cin >> y;
            }

            std::cout << "Souhaitez-vous poser un drapeau ? (1 = oui, 0 = non) : ";
            int flagChoice;
            std::cin >> flagChoice;

            if (flagChoice == 1 && grid.celltab[x][y].flag == false)
            {
                Flag(&grid, x, y);
            }
            else if (flagChoice == 1 && grid.celltab[x][y].flag == true)
            {
                antiFlag(&grid, x, y);
            }
            else
            {
                if (grid.celltab[x][y].mine == true)
                {
                    EndReveal(&grid);
                    DisplayTab(&grid);
                    std::cout << " Vous avez perdu ! Une mine a explose." << std::endl;
                    Play = false;
                    break;
                }

                RevealGrid(&grid, x, y);

                if (CheckWin(&grid) == true)
                {
                    EndReveal(&grid);
                    DisplayTab(&grid);
                    std::cout << " Felicitations ! Vous avez gagne !" << std::endl;
                    Play = false;
                    break;
                }
            }
        }
        std::cout << "Voulez-vous rejouer ? (1 = oui, 0 = non) : ";
        int inputReplay;
        std::cin >> inputReplay;
        replay = (inputReplay == 1);
        std::cout << std::endl;
    }
    std::cout << "Merci d'avoir joue !" << std::endl;
    return 0;
}

