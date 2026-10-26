#pragma once
#include <iostream>
#include "App.h"


int main()
{
    App app;
    bool running = true;
    
    while (running)
    {
        app.Update();
    }

    return 0;
}