#include <pch.h>
#include "L_PlayJungleSound.h"

void L_PlayJungleSound::on_enter()
{
    audioManager->PlaySoundEffect(L"Assets\\Audio\\jungle.wav");
    BehaviorNode::on_leaf_enter();
    on_success();
}
