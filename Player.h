#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>

#include "Entity.h"
#include "Global.h"
#include "Utilities.h"

/*
    PLAYER CLASS

    Constructor's parameters: 
    - Global* global

    About this class:
    - The player suffers by drag;
    - The player accelerates due to the external inputs;
    - The player has a combo mode;
    - The player makes sounds when jumping;
    - The player bounces off walls and platforms;
    - The player changes sprites so it looks like it's running;
    - The player interacts with the players camera.
*/

class Player
{
private:
    //Variables
    //Global variables
    Global* global;

    //Custom constants (maxEntities, maxSize...)
    float accelValue, deaccelFactor;
    float minJumpVelValue;
    float minSpriteScreenSpeed;
    float grav;

    //Game logic (counters, nEntities...)
    float animationCounter, comboCounter, comboCounterAux;
    float jumpVelValue;
    float spriteScreenSpeed;
    int prevFloor, curFloor, maxFloor;
    
    bool playerIsOnPlatform;
    bool justJumped, didJumped, justLanded;
    bool comboOn, comboSide;

    //Game objects
    Entity* entity;
    
    //Private functions
    void initVariables();
    void initEntityObjects();

public:
    //Constructors / Destructors
    Player(Global* global);
    virtual ~Player();

    //Accessors
    Utilities utilities;
    Entity* getEntity();
    void setPlayerStatus(bool playerIsOnPlatform, int newCurFloor);

    //Functions
    void gravity(float dt);
    void movementComboAndSounds(float dt);
    void interactions();
    void animationEffects(float dt);
    void playersCameraSetting(float dt);

    //Main functions
    void update(float dt);
    void render(sf::RenderTarget* target);
};