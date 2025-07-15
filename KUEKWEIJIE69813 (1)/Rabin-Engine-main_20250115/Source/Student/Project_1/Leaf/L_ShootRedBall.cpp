#include <pch.h>
#include "L_ShootRedBall.h"
#include "Agent/BehaviorAgent.h"
#include "Projects/ProjectOne.h"

void L_ShootRedBall::on_enter()
{
    const auto& allAgents = agents->get_all_agents();
    std::vector<BehaviorAgent*> candidates;

    for (auto* a : allAgents)
    {
        if (a != agent)
        {
            candidates.push_back(dynamic_cast<BehaviorAgent*>(a));
        }
    }

    if (candidates.empty())
    {
        on_failure();
        return;
    }

    target = candidates[rand() % candidates.size()];

    projectile = agents->create_behavior_agent("RedBall", BehaviorTreeTypes::Idle, Agent::AgentModel::Ball);
    projectile->set_position(agent->get_position());
    projectile->set_color(Vec3(1.0f, 0.0f, 0.0f)); // Red
    projectile->set_scaling(Vec3(0.3f, 0.3f, 0.3f));

    BehaviorNode::on_leaf_enter();
}

void L_ShootRedBall::on_update(float dt)
{
    if (!projectile || !target)
    {
        on_failure();
        return;
    }

    Vec3 projPos = projectile->get_position();
    Vec3 targetPos = target->get_position();

    float distance = Vec3::Distance(projPos, targetPos);
    if (distance < 0.5f)
    {
        agents->destroy_agent(projectile);
        on_success();
        return;
    }

    Vec3 direction = targetPos - projPos;
    float dist = Vec3::Distance(Vec3(0, 0, 0), direction); // magnitude
    Vec3 step = (direction / dist) * speed * dt;

    projectile->set_position(projPos + step);

    display_leaf_text();
}
