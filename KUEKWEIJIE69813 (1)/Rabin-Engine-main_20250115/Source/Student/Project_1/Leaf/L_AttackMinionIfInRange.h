
#pragma once
#include "BehaviorNode.h"

class L_AttackMinionIfInRange : public BaseNode<L_AttackMinionIfInRange>
{
public:
    void on_enter() override;
    void on_update(float dt) override;

private:
    BehaviorAgent* target = nullptr;
    float attackRange = 10.0f;
    float cooldown = 1.0f;
    float timer = 0.0f;
};