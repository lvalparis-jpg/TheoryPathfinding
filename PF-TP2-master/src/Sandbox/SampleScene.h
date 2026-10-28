#pragma once

#include "Scene.h"

class DummyEntity;

class SampleScene : public Scene
{
    DummyEntity* mpEntity1 = nullptr;
    DummyEntity* mpEntity2 = nullptr;

    DummyEntity* mpSelectedEntity = nullptr;

public:
    void OnInitialize() override;
    void OnUpdate() override;
};
