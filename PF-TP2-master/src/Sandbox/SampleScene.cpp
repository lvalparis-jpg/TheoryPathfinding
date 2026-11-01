#include "SampleScene.h"

int GetRandomNumber(int min, int max)
{
    static std::random_device rd;

    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

#define maybe GetRandomNumber(0,1)

void SampleScene::OnInitialize()
{
    GenerateMaze();

    const float gridW = mGrid.GetRowSize() * CELL_SIZE;
    const float gridH = mGrid.GetColSize() * CELL_SIZE;

    mOrigin = { (GetWindowWidth() - gridW) * 0.5f, (GetWindowHeight() - gridH) * 0.5f };

    mpPlayerNode = mGrid[0][1];
    mpCursorNode = mGrid[0][1];

    mpPlayer = CreateEntity<DummyEntity>(10.f, Color::Cyan);
    mpPlayer->SetPosition(NodeCenter(mpPlayerNode));
}

void SampleScene::OnUpdate()
{
    HandleInput();
    UpdateMovement();
    DrawGrid();

    Debug::DrawText({ 10.f, 10.f },
        "ZQSD : deplacer le curseur - Entree : lancer le pathfinding", Color::White);

    if (mReachable == false)
    {
        Debug::DrawText({ 10.f, 40.f }, "Inatteignable", Color::Orange);
    }
    else if (mpPlayerNode->GetX() == mpCursorNode->GetX() && mpPlayerNode->GetY() == mpCursorNode->GetY())
    {
        Debug::DrawText({ 10.0f, 60.0f }, "C'est atteint", Color::Cyan);
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

    if (input->IsKeyDown(SDLK_ESCAPE))
        exit(0);

    if (input->IsMouseDown(SDL_BUTTON_LEFT))
    {
        Vector2f mouse = input->GetMousePosition();
        int col = static_cast<int>(std::floor((mouse.x - mOrigin.x) / CELL_SIZE));
        int row = static_cast<int>(std::floor((mouse.y - mOrigin.y) / CELL_SIZE));

        if (IsInGrid(row, col))
            mpCursorNode = mGrid[row][col];
    }
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
    return row >= 0 && row < mGrid.GetColSize() && col >= 0 && col < mGrid.GetRowSize();
}

void SampleScene::StartPathfinding()
{
    mApp.BFS(mGrid, mpPlayerNode, mpCursorNode);   // ou mApp.Djikstra(mGrid, ...)
    mReachable = mApp.GetAttainable();
    mPath = mApp.GetPath();                        // on récupère le chemin calculé

    if (mReachable && mPath.empty() == false)
    {
        mPathIndex = 0;
        mStepTimer = 0.f;
        mIsMoving = true;
    }
}

void SampleScene::ResetVisited()
{
    for (int i = 0; i < mGrid.GetColSize(); i++)
    {
        for (int j = 0; j < mGrid.GetRowSize(); j++)
            mGrid[i][j]->SetIsVisited(false);
    }
}

void SampleScene::UpdateMovement()
{
    if (mIsMoving == false)
        return;

    mStepTimer += GetDeltaTime();

    if (mStepTimer < STEP_DELAY)
        return;

    mStepTimer -= STEP_DELAY;

    /*if (maybe)//simule la fatigue
    {
        mpPlayerNode = mPath[mPathIndex];
        mPathIndex++;
        mpPlayer->SetPosition(NodeCenter(mpPlayerNode));

        if (mPathIndex >= mPath.size())
        {
            mIsMoving = false;
            mPath.clear();
            mPathIndex = 0;
        } 
    }*/

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
    Vector2f center = { mOrigin.x + (node->GetY() + 0.5f) * CELL_SIZE ,mOrigin.y + (node->GetX() + 0.5f) * CELL_SIZE };
    return center;
}

void SampleScene::DrawGrid()
{
    const int lines = mGrid.GetColSize();
    const int rows = mGrid.GetRowSize();
    Color gridColor{ 0, 0, 0 };

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
            Debug::DrawCircle(NodeCenter(mPath[k]), 3.f, Color::Red, true);
    }

    Color red{ 255, 0, 0 };

    if (mpCursorNode == mpPlayerNode)
        DrawSquare(NodeCenter(mpCursorNode), 26.f, red, false);
    else
        DrawSquare(NodeCenter(mpCursorNode), 15.f, red, true);
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

void SampleScene::addEntranceAndExit() {

    // Set the entrance at the top (fixed)
    for (int x = 1; x < mGrid.GetRowSize(); x += 2) { // start at cell next to left/top border, move by unit of 2 to stay in structure
        if (mGrid[1][x]->GetIsWall() == false) { // Find a valid entry point
            mGrid[0][x]->SetIsWall(false);// Open entrance
            mpPlayerNode = mGrid[0][x]; // Set player start position
            mpCursorNode = mGrid[0][x]; // Set cursor start position
            break;
        }
    }

    // Find all possible exit locations on the bottom row and add that index to vector
    std::vector<int> validExits;
    for (int x = 1; x < mGrid.GetRowSize(); x += 2) {
        if (mGrid[mGrid.GetColSize() - 2][x]->GetIsWall() == false)
        {
            validExits.push_back(x); // Store all possible exits
        }
    }

    // Pick a random exit (index) from vector, if there are valid options
    if (!validExits.empty()) {
        int exitIndex = validExits[m_RNG() % validExits.size()]; // Select a random valid exit
        mGrid[mGrid.GetColSize() - 2][exitIndex]->SetIsWall(false); // Open exit
    }
}

void SampleScene::CarveMaze(int x, int y) {

    // Moves two cells at a time to ensure clear paths are created and walls remain intact between paths.
    static const int dx[] = { 0,  0, -2,  2 }; // Movement offsets for Left & Right
    static const int dy[] = { -2, 2,  0,  0 }; // Movement offsets for Up & Down

    // List of direction indices (0: Up, 1: Down, 2: Left, 3: Right) - reused across function calls
    static std::vector<int> dirs = { 0, 1, 2, 3 };

    // A <algorithm> library function to randomly shuffle the directions to create unique maze structures on each generation
    std::shuffle(dirs.begin(), dirs.end(), SampleScene::m_RNG);

    // Mark the current cell/location (passed in as arg) as an open path (' ')
    mGrid[y][x]->SetIsWall(false);

    // lambda function to easily check the bounds of ny and nx(below) to ensure we are not at border or beyond
    auto isBounds = [](int ax, int bound)
        {
        return (ax > 0) && (ax < bound - 1);
        };

    // Iterate through shuffled directions to determine where to move next
    for (int dir : dirs)
    {
        // Calculate the new cell coordinates by moving two steps in the chosen direction
        // This moves in unit of 2
        int nx = x + dx[dir]; // new x position
        int ny = y + dy[dir]; // new y position

        // Check if the new cell is within the grid boundaries and is still a wall ('#')
        if (isBounds(ny, mGrid.GetColSize()) && isBounds(nx, mGrid.GetRowSize()) && mGrid[ny][nx]->GetIsWall())
        {

            // Carve a passage by opening the wall between the current cell and the new cell
            // We divide by 2 to get the actual neighbor cell (since we move by a unit of 2) and set element to space to connect passage with the current cell
            mGrid[y + dy[dir] / 2][x + dx[dir] / 2]->SetIsWall(false);

            // Recursively call CarveMaze on the new cell to continue carving paths - will carve down to last cell and return back up
            CarveMaze(nx, ny);
        }
    }
}

void SampleScene::GenerateMaze()
{

    if (mGrid.GetColSize() <= 0 || mGrid.GetRowSize() <= 0)
    {
        std::cout << "Set the maze dimensions first!\n";
        return;
    }

    m_RNG.seed(std::random_device{}()); // generate new seed for randomness

    for (int i = 0; i < mGrid.GetColSize();i++) // Fill entire grid with walls - initializes the vector to a new size and fills it
    {
        for (int j = 0; j < mGrid.GetRowSize();j++)
        {
            mGrid[i][j]->SetIsWall(true);
        }
    }

    CarveMaze(1, 1); // Recursively carve path ways, randomly starting from (0,0) position (inside border)

    addEntranceAndExit(); // adds an entrance and exit to the existing new maze
}
