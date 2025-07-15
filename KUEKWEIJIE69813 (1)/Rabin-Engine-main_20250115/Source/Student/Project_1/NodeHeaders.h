#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes
#include "ControlFlow/C_AlternateSelector.h"


// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_RepeatForever.h"
#include "Decorator/D_Cooldown.h"


// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"
#include "Leaf/L_PlaySound.h"

// Student Leaf Nodes

#include "Leaf/L_ChaseChampion.h"

#include "Leaf/L_AttackMinionIfInRange.h"
#include "Leaf/L_MoveForwardMid.h"
#include "Leaf/L_CheckSizeAndDespawn.h"
#include "Leaf/L_SpawnFriendlyMinions.h"
#include "Leaf/L_SpawnEnemyMinions.h"
#include "Leaf/L_ShootBlueBall.h" 
#include "Leaf/L_ShootRedBall.h" 

#include "Leaf/L_PlayJungleSound.h" 

