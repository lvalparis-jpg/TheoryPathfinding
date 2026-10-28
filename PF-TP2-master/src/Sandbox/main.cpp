#pragma once
#include "GameManager.h"
#include "SampleScene.h"

int main()
{
    GameManager* game = GameManager::Get();

    game->LaunchScene<SampleScene>(1280, 720, "Pathfinding", Color{ 200, 200, 205 });

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
