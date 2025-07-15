#pragma once
#include "BehaviorNode.h"

class L_ChaseChampion : public BaseNode<L_ChaseChampion>
{
protected:
    Vec3 targetPos;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};