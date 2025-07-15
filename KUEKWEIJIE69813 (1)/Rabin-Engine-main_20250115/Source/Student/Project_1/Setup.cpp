#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()


{
    // ADDING OF MODELS
    Agent::add_model("Assets\\bird.sdkmesh", Agent::AgentModel::Bird);
    Agent::add_model("Assets\\tree.sdkmesh", Agent::AgentModel::Tree);
    Agent::add_model("Assets\\car.sdkmesh", Agent::AgentModel::Car);
    Agent::add_model("Assets\\ball.sdkmesh", Agent::AgentModel::Ball);
    Agent::add_model("Assets\\hut.sdkmesh", Agent::AgentModel::Hut);
    Agent::add_model("Assets\\mannequin_rig.sdkmesh", Agent::AgentModel::Man);

    // Create an agent (using the default "Agent::AgentModel::Man" model)


    //CREATION


    // You can change properties here or at runtime from a behavior tree leaf node
    // Look in Agent.h for all of the setters, like these:
    // AGENT CHANGES
   // man->set_scaling(Vec3(7,7,7));
    // man->set_position(Vec3(100, 0, 100));

    // Create an agent with a different 3D model:
    // 1. (optional) Add a new 3D model to the framework other than the ones provided:
    //    A. Find a ".sdkmesh" model or use https://github.com/walbourn/contentexporter
    //       to convert fbx files (many end up corrupted in this process, so good luck!)
    //    B. Add a new AgentModel enum for your model in Agent.h (like the existing Man or Tree).
    // 2. Register the new model with the engine, so it associates the file path with the enum
    //    A. Here we are registering all of the extra models that already come in the package.

     //TOWER

    // 3. Create the agent, giving it the correct AgentModel type.
    auto tower1 = agents->create_behavior_agent("FriendlyTower", BehaviorTreeTypes::Idle, Agent::AgentModel::Tree);
    tower1->set_position(Vec3(20, 0, 20));

    auto tower2 = agents->create_behavior_agent("FriendlyTower", BehaviorTreeTypes::Idle, Agent::AgentModel::Tree);
    tower2->set_position(Vec3(30, 0, 30));

    //enemy
    auto tower3 = agents->create_behavior_agent("EnemyTower", BehaviorTreeTypes::Idle, Agent::AgentModel::Tree);
    tower3->set_position(Vec3(60, 0, 60));

    auto tower4 = agents->create_behavior_agent("EnemyTower", BehaviorTreeTypes::Idle, Agent::AgentModel::Tree);
    tower4->set_position(Vec3(70, 0, 70));

  
    // 4. (optional) You can also set the pitch of the model, if you want it to be rotated differently
    tower1->set_pitch(PI / 2);
    tower2->set_pitch(PI / 2);
    tower3->set_pitch(PI / 2);
    tower4->set_pitch(PI / 2);

    // 5. (optional) Set other aspects to make it start out correctly
    tower1->set_color(Vec3(0, 0, 1));    // Blue
    tower2->set_color(Vec3(0, 0, 1));    // Blue
    tower3->set_color(Vec3(1, 0, 0));    // Red
    tower4->set_color(Vec3(1, 0, 0));    // Red

    Vec3 towerScale = { 1.0f, 0.75f, 1.0f };  
    tower1->set_scaling(towerScale);
    tower2->set_scaling(towerScale);
    tower3->set_scaling(towerScale);
    tower4->set_scaling(towerScale);


    //NEXUS

    auto nexus1 = agents->create_behavior_agent("Team Nexus", BehaviorTreeTypes::FriendlyNexus, Agent::AgentModel::Hut);
    auto nexus2 = agents->create_behavior_agent("Enemy Nexus", BehaviorTreeTypes::EnemyNexus, Agent::AgentModel::Hut);

    //position
    Vec3 newsize = { 0.5 ,0.5,0.5 };

    nexus1->set_position(Vec3(5, 0, 0));
    nexus2->set_position(Vec3(95, 0, 90));
    nexus1->set_scaling(newsize);
    nexus2->set_scaling(newsize);



    //minions
    auto Minion1 = agents->create_behavior_agent("FriendlyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
    auto Minion2 = agents->create_behavior_agent("FriendlyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
    auto Minion3 = agents->create_behavior_agent("FriendlyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
    auto Minion4 = agents->create_behavior_agent("FriendlyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);



    auto Minion11 = agents->create_behavior_agent("EnemyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
    auto Minion12 = agents->create_behavior_agent("EnemyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
    auto Minion13 = agents->create_behavior_agent("EnemyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);
    auto Minion14 = agents->create_behavior_agent("EnemyMinion", BehaviorTreeTypes::Minions, Agent::AgentModel::Bird);

    // Minion size
    Vec3 minionSize = { 0.0075f, 0.0075f, 0.0075f };

    // Scale friendly minions
    Minion1->set_scaling(minionSize);
    Minion2->set_scaling(minionSize);
    Minion3->set_scaling(minionSize);
    Minion4->set_scaling(minionSize);

    Minion1->set_position(Vec3(5, 0, 0));
    Minion2->set_position(Vec3(5, 0, 0));
    Minion3->set_position(Vec3(5, 0, 0));
    Minion4->set_position(Vec3(5, 0, 0));


    // Scale enemy minions
    Minion11->set_scaling(minionSize);
    Minion12->set_scaling(minionSize);
    Minion13->set_scaling(minionSize);
    Minion14->set_scaling(minionSize);

    Minion11->set_position(Vec3(95, 0, 90));
    Minion12->set_position(Vec3(95, 0, 90));
    Minion13->set_position(Vec3(95, 0, 90));
    Minion14->set_position(Vec3(95, 0, 90));

    //champion 
    
    
    auto champion = agents->create_behavior_agent("champion", BehaviorTreeTypes::NewChamp, Agent::AgentModel::Man);
    //tower1->set_position(Vec3(10, 0, 30));
    Vec3 championSize = { 2.5f, 2.5f, 2.5f };
    champion->set_color(Vec3(1, 0, 1));


    champion->set_scaling(championSize);
    champion->set_scaling(Vec3(2.25,1.5,1.5));
    champion->set_position(Vec3(20, 0, 50));


    //jungle 
    
    auto jungle = agents->create_behavior_agent("jungle", BehaviorTreeTypes::jungle , Agent::AgentModel::Car);
    jungle->set_position(Vec3(0, 0, 95));
    jungle->set_scaling(Vec3(0.5, 0.5, 0.5));
    champion->set_color(Vec3(0.2, 0, 1));


    

    // You can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code
    // that yourself (that's the realm of project 2)
    terrain->goto_map(0);

    // You can also enable the pathing layer and set grid square colors as you see fit.
    // Works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // Camera position can be modified from this default
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    // Sound control (these sound functions can be kicked off in a behavior tree node - see the example in L_PlaySound.cpp)
    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // Uncomment for example on playing music in the engine (must be .wav)
    // audioManager->PlayMusic(L"Assets\\Audio\\motivate.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
}