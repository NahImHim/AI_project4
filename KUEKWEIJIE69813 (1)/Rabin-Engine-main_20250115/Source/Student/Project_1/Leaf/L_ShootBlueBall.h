#pragma once
#include "BehaviorNode.h"

class L_ShootBlueBall : public BaseNode<L_ShootBlueBall>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    BehaviorAgent* projectile = nullptr;
    BehaviorAgent* target = nullptr;
    float speed = 15.0f;
};
