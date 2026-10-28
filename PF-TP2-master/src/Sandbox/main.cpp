#pragma once
#include "GameManager.h"
#include "SampleScene.h"

#include <Debug.h>

int main()
{
    GameManager* game = GameManager::Get();

    game->LaunchScene<SampleScene>(1920, 1080, "Pathfinding", Color{204, 204, 204});

    return 0;
}


//#include <iostream>
//#include "App.h"
//
//int main()
//{
//    App app;
//    bool running = true;
//
//    while (running)
//    {
//        app.Update();
//    }
//
//    return 0;
//}
