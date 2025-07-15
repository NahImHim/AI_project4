#pragma once
#include "BehaviorNode.h"

class L_SpawnEnemyMinions : public BaseNode<L_SpawnEnemyMinions>
{
protected:
    void on_enter() override;
    void on_update(float dt) override;

private:
    float spawnTimer = 0.0;
    const float spawnInterval = 7.0f;  // seconds
};
