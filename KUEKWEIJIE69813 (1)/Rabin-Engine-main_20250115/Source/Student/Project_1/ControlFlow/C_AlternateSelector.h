#pragma once
#include "BehaviorNode.h"

class C_AlternateSelector : public BaseNode<C_AlternateSelector>
{
public:
    C_AlternateSelector();

protected:
    size_t currentIndex;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};
