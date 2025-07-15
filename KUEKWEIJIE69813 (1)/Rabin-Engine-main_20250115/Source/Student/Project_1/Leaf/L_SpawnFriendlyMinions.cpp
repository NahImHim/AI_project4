#include <pch.h>
#include "L_SpawnFriendlyMinions.h"
#include "Agent/BehaviorAgent.h"
#include "Projects/ProjectOne.h"

void L_SpawnFriendlyMinions::on_enter()
{
    spawnTimer = 0.0f;
    BehaviorNode::on_leaf_enter();
}

void L_SpawnFriendlyMinions::on_update(float dt)
{
    spawnTimer += dt;

    if (spawnTimer >= spawnInterval)
    {
        Vec3 spawnPos = Vec3(5.0f, 0.0f, 0.0f);  // Friendly nexus position
        Vec3 minionScale = { 0.0075f, 0.0075f, 0.0075f };

        for (int i = 0; i < 6; ++i)
        {
            auto minion = agents->create_behavior_agent("FriendlyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
            minion->set_position(spawnPos + Vec3(static_cast<float>(i), 0.0f, 0.0f));
            minion->set_scaling(minionScale);
        }

        spawnTimer = 0.0f;
    }

    this->set_status(NodeStatus::RUNNING);
    this->set_result(NodeResult::IN_PROGRESS);
    display_leaf_text();
}
