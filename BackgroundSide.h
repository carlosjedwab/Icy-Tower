#pragma once

#include <iostream>
#include <vector>

#include "Entity.h"
#include "Global.h"
#include "Utilities.h"

/*
    BACKGROUNDSIDE CLASS

    Constructor's parameters: 
    - Global* global

    About this class:
    - It stores, adjust to camera and renders the backgroundSide entity.
*/

class BackgroundSide
{
private:
    //Variables
    //Global variables
    Global* global;

    //Custom constants (maxEntities, maxSize...)
    const float speedFactor = 2.5;

    //Game object
    Entity* entity;
    
    //Private functions
    void initEntityObjects();

public:
    //Constructors / Destructors
    BackgroundSide(Global* global);
    virtual ~BackgroundSide();

    //Accessors
    Utilities utilities;
    Entity* getEntity();

    //Functions
    void renderOne(sf::RenderTarget* target);
    void render(sf::RenderTarget* target);
};