#include <pch.h>
#include "D_RepeatForever.h"

void D_RepeatForever::on_update(float dt)
{
    BehaviorNode* child = children.front();

    child->tick(dt);

    // Always reset child after it finishes so it repeats forever
    if (child->succeeded() || child->failed())
    {
        child->set_status(NodeStatus::READY);
        child->set_result(NodeResult::IN_PROGRESS);
    }

    set_status(NodeStatus::RUNNING);
    set_result(NodeResult::IN_PROGRESS);
}

void D_RepeatForever::on_exit()
{
    // Ensure this node resets properly
    set_status(NodeStatus::READY);
}
