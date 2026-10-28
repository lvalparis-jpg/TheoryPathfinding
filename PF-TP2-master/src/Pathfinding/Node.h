#pragma once
#include <iostream>

class Node
{
private:

    std::pair<int, int> m_pos;

    bool IsVisited;
    bool IsWall;

public:


    //Get
    int GetX();
    int GetY();

    bool GetIsVisited();
    bool GetIsWall();

    //Set
    void SetX(int x);
    void SetY(int y);

    void SetIsVisited(bool visited);
    void SetIsWall(bool wall);

};
