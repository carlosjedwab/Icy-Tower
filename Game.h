#pragma once

#include <iostream>
#include <vector>

#include "BackgroundBack.h"
#include "BackgroundSide.h"
#include "Platforms.h"
#include "Player.h"
//#include "NewEntity.h"/////////////////////////////////////////////////////////////edit

/*
    GAME CLASS

    Constructor's parameters: none;

    About this class:
    - It acts as the game engine/
      Wrapper class.
*/

class Game
{
private:
    //Variables
    Global global;

    //Window
    sf::RenderWindow* window;

    //Music
    sf::Music music;

    //Game objects
    BackgroundBack* backgroundBack;
    BackgroundSide* backgroundSide;
    Platforms* platforms;
    Player* player;
    //NewEntity* newEntity;/////////////////////////////////////////////////////////////edit
    
    //Private functions
    void initWindow();
    void initEntities();

public:
    //Constructors / Destructors
    Game();
    virtual ~Game();

    //Accessors
    Utilities utilities;
    const bool running() const;

    //Functions
    void pollEvents();
    void updateMousePositions();
    void update(float dt);
    void render();
};