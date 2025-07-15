#include <pch.h>
#include "D_Cooldown.h"

D_Cooldown::D_Cooldown() : timer(0.0f)
{}

void D_Cooldown::on_enter()
{
    timer = 5.0f; // fixed cooldown duration
    BehaviorNode::on_enter();
}

void D_Cooldown::on_update(float dt)
{
    timer -= dt;

    if (timer <= 0.0f)
    {
        BehaviorNode* child = children.front();
        child->tick(dt);

        // Inherit child result
        set_status(child->get_status());
        set_result(child->get_result());
    }
}
