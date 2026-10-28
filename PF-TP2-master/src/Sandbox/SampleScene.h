#pragma once

#include "Scene.h"
#include "Vector.h"
#include "grid.h"

#include <vector>

class DummyEntity;

class SampleScene : public Scene
{
    float CELL_SIZE = 32.f;
    float STEP_DELAY = 0.3f;

    Grid mGrid;
    Vector2f mOrigin = { 0.f, 0.f };

    DummyEntity* mpPlayer = nullptr;
    Node* mpPlayerNode = nullptr;
    Node* mpCursorNode = nullptr;

    std::vector<Node*> mPath;
    size_t mPathIndex = 0;
    float mStepTimer = 0.f;
    bool mIsMoving = false;
    bool mReachable = true;

    void InitObstacles();
    void HandleInput();
    void MoveCursor(int dRow, int dCol);
    void StartPathfinding();
    bool BFS(Node* start, Node* target);
    void ResetVisited();
    void UpdateMovement();

    Vector2f NodeCenter(Node* node);
    bool IsInGrid(int row, int col);

    void DrawGrid();
    void DrawSquare(Vector2f center, float size, const Color& color, bool filled);

public:

    void OnInitialize() override;
    void OnUpdate() override;

};
