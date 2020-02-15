#pragma once

#include <iostream>
#include <vector>

#include "Global.h"
#include "Entity.h"

/*
    UTILITIES CLASS

    Constructor's parameters: none;

    About this class:
    - It contains some useful functions.
*/

class Utilities
{
public:
    //Constructors / Destructors
    Utilities();
    virtual ~Utilities();

    float               vectorModulus               (sf::Vector2f v);
    void                cameraAdjusting             (Global* global, Entity* entity);
    void                cameraStickToX              (Global* global, Entity* entity);
    void                cameraStickToY              (Global* global, Entity* entity);
    void                cameraStickToXandY          (Global* global, Entity* entity);
    std::vector<bool>   boundaryCollision           (Global* global, Entity* entity, bool right, bool left, bool down, bool up);
    bool                entitiesAreIntersecting     (Entity* entity1, Entity* entity2);
    bool                isTouchingGround            (Entity* entity, sf::FloatRect groundBox, float safezone);
    bool                isTouchingRightWall         (Entity* entity, float wallPosX);
    bool                isTouchingLeftWall          (Entity* entity, float wallPosX);
    sf::Vector2f        simpleArrowKeysControl      (float accValue, float dt);
    sf::Vector2f        simpleFollowMovement        (float accelValue, float dt, sf::Vector2f entityPos, sf::Vector2f destinyPos);
    bool                simpleMoveAndJumpControl    (float accelValue, float dt, Entity* entity, float jumpVelValue, bool isTouchingGround);
    void                steadyAtGround              (Entity* entity, bool isTouchingGround);
    void                simplePlatformer            (float accelValue, float dt, Entity* entity, float jumpVelValue, bool isTouchingGround);
    sf::Vector2f        getRandomPosInRectArea      (sf::FloatRect rect);
    sf::Vector2f        getRandomPosInCircArea      (sf::CircleShape circ);
    sf::Vector2f        getRandomPosInRingArea      (sf::CircleShape inCirc, sf::CircleShape outCirc);
    void                spawn                       (std::vector<Entity*> entities, Entity* modelEntity, sf::Vector2f pos);
    void                despawn                     (std::vector<Entity*> entities, int entityNum);
};