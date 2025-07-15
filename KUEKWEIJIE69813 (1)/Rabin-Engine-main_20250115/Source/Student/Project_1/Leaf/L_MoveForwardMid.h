#pragma once
#include "BehaviorNode.h"

class L_MoveForwardMid : public BaseNode<L_MoveForwardMid>
{
protected:
    void on_enter() override;
    void on_update(float dt) override;

private:
    Vec3 targetPos;
};
