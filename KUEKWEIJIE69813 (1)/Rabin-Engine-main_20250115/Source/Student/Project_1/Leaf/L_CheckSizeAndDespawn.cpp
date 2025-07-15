#include <pch.h>
#include "L_CheckSizeAndDespawn.h"
#include "Agent/BehaviorAgent.h"

void L_CheckSizeAndDespawn::on_enter()
{
    const Vec3& scale = agent->get_scaling();

    if (scale.x <= 0.05f || scale.y <= 0.05f || scale.z <= 0.05f)
    {
        agents->destroy_agent(agent);
        std::cout << "[Minion] Destroyed due to small size\n";
        on_success();
    }
    else
    {
        on_failure();
    }

    display_leaf_text();
}