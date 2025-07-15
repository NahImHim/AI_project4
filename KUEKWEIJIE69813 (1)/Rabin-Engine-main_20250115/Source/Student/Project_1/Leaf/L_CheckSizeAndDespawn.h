#pragma once
#include "BehaviorNode.h"

class L_CheckSizeAndDespawn : public BaseNode<L_CheckSizeAndDespawn>
{
protected:
    void on_enter() override;
};