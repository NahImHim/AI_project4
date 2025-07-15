#pragma once
#include "BehaviorNode.h"

class D_RepeatForever : public BaseNode<D_RepeatForever>
{
protected:
    virtual void on_update(float dt) override;
    virtual void on_exit() override;
};