#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>

#include "Entity.h"
#include "Global.h"
#include "Utilities.h"

/*
    PLATFORMS CLASS

    Constructor's parameters: 
    - Global* global

    About this class:
    - It generates new platforms acordind to the cameras advancing;
    - It deletes the bottom platform if too far away;
*/

struct Platform_Reg
{
    Entity* entity;
    int type;
    int size;
    int floorNum;
    sf::FloatRect entityBox;

}; typedef struct Platform_Reg Platform;

class Platforms
{
private:
    //Variables
    //Global variables
    Global* global;

    //Custom constants (maxEntities, maxSize...)
    int numOfPlatformsTypes;
    int platformPieceWidth, platformCornerWidth;
    float viewDist, betweenDist;
    int maxSizeFraction, minSizeFraction;
    float safezone;
    int floorNumCheckPoint;
    int floorNumNextLevel;

    //Game logic (counters, nEntities...)

    //Game objects
    std::vector<Platform> entities;
    
    //Private functions
    void initVariables();
    void initEntityObjects();

public:
    //Constructors / Destructors
    Platforms(Global* global);
    virtual ~Platforms();

    //Accessors
    Utilities utilities;
    std::vector<Platform> getEntities();

    //Functions
    void newPlatformGenerator(bool first = false);
    void deleteFarPlatform();
    void renderPlatform(sf::RenderTarget* target, int platformNum);

    //Main functions
    void update(float dt);
    void render(sf::RenderTarget* target);
};