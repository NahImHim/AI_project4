#pragma once
#include "BehaviorNode.h"

class L_SpawnFriendlyMinions : public BaseNode<L_SpawnFriendlyMinions>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float spawnTimer = 0.0;
    const float spawnInterval = 7.0f;  // seconds
};
