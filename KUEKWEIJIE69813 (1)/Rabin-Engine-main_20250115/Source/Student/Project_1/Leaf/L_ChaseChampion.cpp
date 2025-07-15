#include <pch.h>
#include "L_ChaseChampion.h"
#include "Agent/BehaviorAgent.h"

void L_ChaseChampion::on_enter()
{
    // Find the champion agent
    const auto& allAgents = agents->get_all_agents_by_type("Champion");

    if (!allAgents.empty())
    {
        Agent* champ = allAgents.front();  // Assume only 1 champion
        targetPos = champ->get_position();
        BehaviorNode::on_leaf_enter();
    }
    else
    {
        on_failure();  // No champion to chase
    }
}

void L_ChaseChampion::on_update(float dt)
{
    if (agent->move_toward_point(targetPos, dt))
    {
        on_success();
    }

    display_leaf_text();
}
