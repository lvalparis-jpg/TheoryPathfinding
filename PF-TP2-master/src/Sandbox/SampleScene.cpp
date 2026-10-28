#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"

#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>

void SampleScene::OnInitialize()
{
    InitObstacles();

    float gridW = mGrid.GetRowSize() * CELL_SIZE;
    float gridH = mGrid.GetLineSize() * CELL_SIZE;
    mOrigin = { (GetWindowWidth() - gridW) * 0.5f, (GetWindowHeight() - gridH) * 0.5f };

    mpPlayerNode = mGrid[0][0];
    mpCursorNode = mGrid[0][0];

    mpPlayer = CreateEntity<DummyEntity>(10.f, Color::Cyan);
    mpPlayer->SetPosition(NodeCenter(mpPlayerNode));
}

void SampleScene::OnUpdate()
{
    HandleInput();
    UpdateMovement();
    DrawGrid();

    Debug::DrawText({ 10.f, 10.f },
        "ZQSD : deplacer le curseur - Entree :lancer le pathfinding", Color::White);

    if (mReachable == false)
    {
        Debug::DrawText({ 10.f, 40.f }, "Inatteignable", Color::Orange);
    }
    else if (mIsMoving)
    {
        std::string text = "Deplacements restants : " + std::to_string(mPath.size() - mPathIndex);
        Debug::DrawText({ 10.f, 40.f }, text.c_str(), Color::White);
    }
}

void SampleScene::HandleInput()
{
    InputManager* input = InputManager::Get();

    if (input->IsKeyDown(SDLK_Z)) MoveCursor(-1, 0);
    if (input->IsKeyDown(SDLK_S)) MoveCursor(1, 0);
    if (input->IsKeyDown(SDLK_Q)) MoveCursor(0, -1);
    if (input->IsKeyDown(SDLK_D)) MoveCursor(0, 1);

    if (input->IsKeyDown(SDLK_RETURN) && mIsMoving == false)
        StartPathfinding();
}

void SampleScene::MoveCursor(int dRow, int dCol)
{
    int row = mpCursorNode->GetX() + dRow;
    int col = mpCursorNode->GetY() + dCol;

    if (IsInGrid(row, col))
        mpCursorNode = mGrid[row][col];
}

bool SampleScene::IsInGrid(int row, int col)
{
    return row >= 0 && row < mGrid.GetLineSize() && col >= 0 && col < mGrid.GetRowSize();
}

void SampleScene::StartPathfinding()
{
    mReachable = BFS(mpPlayerNode, mpCursorNode);

    if (mReachable && mPath.empty() == false)
    {
        mPathIndex = 0;
        mStepTimer = 0.f;
        mIsMoving = true;
    }
}

void SampleScene::ResetVisited()
{
    for (int i = 0; i < mGrid.GetLineSize(); i++)
    {
        for (int j = 0; j < mGrid.GetRowSize(); j++)
            mGrid[i][j]->SetIsVisited(false);
    }
}

bool SampleScene::BFS(Node* start, Node* target)
{
    ResetVisited();
    mPath.clear();

    std::queue<Node*> toVisit;
    std::unordered_map<Node*, Node*> parent;

    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    toVisit.push(start);
    start->SetIsVisited(true);

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

            if (IsInGrid(nx, ny) == false)
                continue;

            Node* next = mGrid[nx][ny];

            if (next->GetIsWall())
                continue;

            if (next->GetIsVisited())
                continue;

            next->SetIsVisited(true);
            parent[next] = current;
            toVisit.push(next);
        }
    }

    if (target->GetIsVisited() == false)
        return false;

    for (Node* cur = target; cur != start; cur = parent[cur])
        mPath.push_back(cur);

    std::reverse(mPath.begin(), mPath.end());
    return true;
}

void SampleScene::UpdateMovement()
{
    if (mIsMoving == false)
        return;

    mStepTimer += GetDeltaTime();

    if (mStepTimer < STEP_DELAY)
        return;

    mStepTimer -= STEP_DELAY;

    mpPlayerNode = mPath[mPathIndex];
    mPathIndex++;
    mpPlayer->SetPosition(NodeCenter(mpPlayerNode));

    if (mPathIndex >= mPath.size())
    {
        mIsMoving = false;
        mPath.clear();
        mPathIndex = 0;
    }
}

Vector2f SampleScene::NodeCenter(Node* node)
{
    return {
        mOrigin.x + (node->GetY() + 0.5f) * CELL_SIZE,
        mOrigin.y + (node->GetX() + 0.5f) * CELL_SIZE
    };
}

void SampleScene::DrawGrid()
{
    int lines = mGrid.GetLineSize();
    int rows = mGrid.GetRowSize();
    Color gridColor{ 90, 90, 110 };

    for (int i = 0; i <= lines; i++)
    {
        float y = mOrigin.y + i * CELL_SIZE;
        Debug::DrawLine({ mOrigin.x, y }, { mOrigin.x + rows * CELL_SIZE, y }, gridColor);
    }
    for (int j = 0; j <= rows; j++)
    {
        float x = mOrigin.x + j * CELL_SIZE;
        Debug::DrawLine({ x, mOrigin.y }, { x, mOrigin.y + lines * CELL_SIZE }, gridColor);
    }

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            Node* node = mGrid[i][j];

            if (node->GetIsWall())
                DrawSquare(NodeCenter(node), CELL_SIZE - 2.f, Color::Black, true);
        }
    }

    if (mIsMoving)
    {
        for (size_t k = mPathIndex; k < mPath.size(); k++)
            Debug::DrawCircle(NodeCenter(mPath[k]), 3.f, Color::Orange, true);
    }

    Color red{ 255, 0, 0 };

    if (mpCursorNode == mpPlayerNode)
        DrawSquare(NodeCenter(mpCursorNode), 26.f, red, false);
    else
        DrawSquare(NodeCenter(mpCursorNode), 12.f, red, true);
}

void SampleScene::DrawSquare(Vector2f center, float size, const Color& color, bool filled)
{
    const float h = size * 0.5f;

    if (filled)
    {
        for (float y = -h; y <= h; y += 1.f)
            Debug::DrawLine({ center.x - h, center.y + y }, { center.x + h, center.y + y }, color);
    }
    else
    {
        Debug::DrawLine({ center.x - h, center.y - h }, { center.x + h, center.y - h }, color);
        Debug::DrawLine({ center.x + h, center.y - h }, { center.x + h, center.y + h }, color);
        Debug::DrawLine({ center.x + h, center.y + h }, { center.x - h, center.y + h }, color);
        Debug::DrawLine({ center.x - h, center.y + h }, { center.x - h, center.y - h }, color);
    }
}

void SampleScene::InitObstacles()
{
    for (int i = 4; i < 7; i++)  mGrid[i][5]->SetIsWall(true);
    for (int i = 2; i < 5; i++)  mGrid[8][i]->SetIsWall(true);
    for (int i = 0; i < 5; i++)  mGrid[i][3]->SetIsWall(true);
    for (int i = 0; i < 8; i++)  mGrid[i][7]->SetIsWall(true);
    for (int i = 0; i < 2; i++)  mGrid[1][i]->SetIsWall(true);
    for (int i = 7; i < 10; i++) mGrid[1][i]->SetIsWall(true);
    for (int i = 1; i < 3; i++)  mGrid[3][i]->SetIsWall(true);
}
