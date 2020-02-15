#pragma once

#include <iostream>
#include <vector>

#include "Entity.h"
#include "Global.h"
#include "Utilities.h"

/*
    BACKGROUNDBACK CLASS

    Constructor's parameters: 
    - Global* global

    About this class:
    - It stores, adjust to camera and renders the backgroundBack entity.
*/

class BackgroundBack
{
private:
    //Variables
    //Global variables
    Global* global;

    //Custom constants (maxEntities, maxSize...)
    const float speedFactor = 0.75;

    //Game object
    Entity* entity;
    
    //Private functions
    void initEntityObjects();

public:
    //Constructors / Destructors
    BackgroundBack(Global* global);
    virtual ~BackgroundBack();

    //Accessors
    Utilities utilities;
    Entity* getEntity();

    //Functions
    void renderOne(sf::RenderTarget* target);
    void render(sf::RenderTarget* target);
};