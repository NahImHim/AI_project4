#include <pch.h>
#include "L_AttackMinionIfInRange.h"
#include "Agent/BehaviorAgent.h"

void L_AttackMinionIfInRange::on_enter()
{
    BehaviorNode::on_leaf_enter();
}

void L_AttackMinionIfInRange::on_update(float dt)
{
    const Vec3& myPos = agent->get_position();

    // Decide who to attack based on team
    std::string team = agent->get_type();
    const char* targetType = (team == "FriendlyMinion") ? "EnemyMinion" : "FriendlyMinion";
    const auto& targets = agents->get_all_agents_by_type(targetType);

    BehaviorAgent* closestTarget = nullptr;
    float closestDist = std::numeric_limits<float>::max();

    for (auto* other : targets)
    {
        if (other == agent) continue;

        float dist = Vec3::Distance(myPos, other->get_position());
        if (dist <= attackRange && dist < closestDist)
        {
            closestTarget = dynamic_cast<BehaviorAgent*>(other);
            closestDist = dist;
        }
    }

    if (closestTarget != nullptr)
    {
        // Stop by "moving" to current location
        agent->move_toward_point(myPos, 0.0f);

        // Attack immediately
        Vec3 scale = closestTarget->get_scaling();
        closestTarget->set_scaling(scale * 0.75f);
        std::cout << "[Minion] Instant attack -> new size: " << closestTarget->get_scaling().x << "\n";

        on_success();
    }
    else
    {
        on_failure();  // not in range
    }

    display_leaf_text();
}
