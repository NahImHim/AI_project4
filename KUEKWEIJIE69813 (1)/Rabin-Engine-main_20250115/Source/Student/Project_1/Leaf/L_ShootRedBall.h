#pragma once
#include "BehaviorNode.h"

class L_ShootRedBall : public BaseNode<L_ShootRedBall>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    BehaviorAgent* projectile = nullptr;
    BehaviorAgent* target = nullptr;
    float speed = 15.0f;
};
