#include "Node.h"

int Node::GetX()
{
    return m_pos.first;
}

int Node::GetY()
{
    return m_pos.second;
}

bool Node::GetIsVisited()
{
    return IsVisited;
}

bool Node::GetIsWall()
{
    return IsWall;
}

void Node::SetX(int x)
{
    m_pos.first = x;
}

void Node::SetY(int y)
{
    m_pos.second = y;
}

void Node::SetIsVisited(bool visited)
{
    IsVisited = visited;
}
void Node::SetIsWall(bool wall)
{
    IsWall = wall;
}
