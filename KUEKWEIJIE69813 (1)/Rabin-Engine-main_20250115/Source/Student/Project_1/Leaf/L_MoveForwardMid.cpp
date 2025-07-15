#include <pch.h>
#include "L_MoveForwardMid.h"
#include "Agent/BehaviorAgent.h"

void L_MoveForwardMid::on_enter()
{
    std::string team = agent->get_type();  // returns "FriendlyMinion" or "EnemyMinion"

    if (team == "FriendlyMinion")
    {
        targetPos = Vec3(95.0f, 0.0f, 90.0f);  // Enemy nexus position
    }
    else if (team == "EnemyMinion")
    {
        targetPos = Vec3(5.0f, 0.0f, 0.0f);    // Friendly nexus position
    }
    else
    {
        // Default fail-safe
        targetPos = agent->get_position();
    }

    BehaviorNode::on_leaf_enter();
}

void L_MoveForwardMid::on_update(float dt)
{
    if (agent->move_toward_point(targetPos, dt))
    {
        on_success();
    }

    display_leaf_text();
}
