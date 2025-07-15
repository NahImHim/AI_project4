Student Name:
Kuek Wei Jie
Project Name:
The Legend Of The Leagues
What I implemented:

Directions (if needed):

What I liked about the project and framework:

What I disliked about the project and framework:

Any difficulties I experienced while doing the project:

Hours spent:

About 20 hours, 
New selector node (name):
C_AlternateSelector
New decorator nodes (names):
D_RepeatForever
D_Cooldown
10 total nodes (names):


L_AttackMinionIfInRange

L_MoveForwardMid

L_CheckSizeAndDespawn

L_SpawnFriendlyMinions

L_SpawnEnemyMinions

L_ShootRedBall

L_ShootBlueBall

L_ReturnToSpawn

L_ChaseChampion


4 Behavior trees (names):
Minions

FriendlyNexus and enemy Nexus

Champion

JungleMonster


Extra credit:
NA


What I liked:
I get to experience how league of legends, a game that seem to be complicated when I first entered digipen can be produced. Although it definitely isn't comparable to the actual game, I gained a deeper understanding of how the NPCs in the game move and this makes MOBA games much more doable for a project game and I think I would love to make such games in the future.

What I disliked:
One thing I disliked was how fragile the registration system could be. If a node wasn’t registered or included properly, nothing would work, and sometimes there were no useful error messages to help track that down. It was also easy to misconfigure tree connections in the editor and not realize until the logic broke silently. Debugging behavior tree logic took trial and error, especially when nodes didn’t re-tick as expected.

Difficulties I encountered:
I ran into several difficulties — from forgetting to include new node headers, to nodes only running once because their state wasn’t resetting properly. Timing issues like cooldowns and repeat decorators not working were tricky until I understood the framework’s tick/update lifecycle. There were also Visual Studio issues where files weren’t being recognized until they were manually added and rebuilt.