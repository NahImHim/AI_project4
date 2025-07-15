#include <pch.h>
#include "C_AlternateSelector.h"

C_AlternateSelector::C_AlternateSelector() : currentIndex(0)
{}

void C_AlternateSelector::on_enter()
{
    BehaviorNode::on_enter();

    // Wrap around to prevent out-of-bounds
    if (children.empty()) {
        on_failure();
        return;
    }

    currentIndex = currentIndex % children.size();
}

void C_AlternateSelector::on_update(float dt)
{
    if (children.empty()) {
        on_failure();
        return;
    }

    BehaviorNode* currentNode = children[currentIndex];
    currentNode->tick(dt);

    if (currentNode->succeeded())
    {
        currentIndex = (currentIndex + 1) % children.size();
        on_success();
    }
    else if (currentNode->failed())
    {
        currentIndex = (currentIndex + 1) % children.size();
        on_failure();
    }
    else
    {
        // Node still running — mark this node as running too
        set_status(NodeStatus::RUNNING);
    }
}
