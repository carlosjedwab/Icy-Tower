#include "Utilities.h"

//Constructors / Destructors
Utilities::Utilities()
{
    
}
Utilities::~Utilities()
{
    
}

//Returns the vector's modulus value.
float Utilities::vectorModulus(sf::Vector2f v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

//Adjusts entity's sprite position so that the camera is independent of the entity.
void Utilities::cameraAdjusting(Global* global, Entity* entity)
{
    entity->getCurrentSprite()->setPosition(entity->getPosition() - global->getCameraOfst());
}

//Adjusts entity's sprite position so that the entity is fixed horizontally in relation to the window.
void Utilities::cameraStickToX(Global *global, Entity* entity)
{
    global->setCameraOfst(sf::Vector2f(entity->getPosition().x - entity->getCurrentSprite()->getPosition().x, global->getCameraOfst().y));
    entity->getCurrentSprite()->setPosition(
        entity->getCurrentSprite()->getPosition().x,
        entity->getPosition().y - global->getCameraOfst().y
    );
}

//Adjusts entity's sprite position so that the entity is fixed vertically in relation to the window.
void Utilities::cameraStickToY(Global *global, Entity* entity)
{
    global->setCameraOfst(sf::Vector2f(global->getCameraOfst().x, entity->getPosition().y - entity->getCurrentSprite()->getPosition().y));
    entity->getCurrentSprite()->setPosition(
        entity->getPosition().x - global->getCameraOfst().x,
        entity->getCurrentSprite()->getPosition().y
    );
}

//Adjusts entity's sprite position so that the entity is fixed horizontally ad vertically in relation to the window.
void Utilities::cameraStickToXandY(Global *global, Entity* entity)
{
    global->setCameraOfst(entity->getPosition() - entity->getCurrentSprite()->getPosition());
}

//Boundary collision check and execution.
//Returns 4 boundary collision detection booleans: right(0), left(1), down(2) and up(3)
std::vector<bool> Utilities::boundaryCollision(Global *global, Entity* entity, bool right, bool left, bool down, bool up)
{
    std::vector<bool> collosionDetectionBoolean;
    for (int i=0; i<4; i++)
        collosionDetectionBoolean.push_back(0);

    sf::FloatRect entityBox = sf::FloatRect(
        entity->getPosition().x - entity->getCurrentSprite()->getOrigin().x,
        entity->getPosition().y - entity->getCurrentSprite()->getOrigin().y,
        entity->getCurrentSprite()->getLocalBounds().width,
        entity->getCurrentSprite()->getLocalBounds().height
    );

    sf::Vector2f newEntityVel = entity->getVelocity();

    if (entityBox.left + entityBox.width > global->getBoundaries().left + global->getBoundaries().width && right){
        newEntityVel.x = -fabs(newEntityVel.x);
        collosionDetectionBoolean[0] = 1; //Right
    }

    if (entityBox.left < global->getBoundaries().left && left){
        newEntityVel.x = fabs(newEntityVel.x);
          collosionDetectionBoolean[1] = 1; //Left
    }

    if (entityBox.top + entityBox.height > global->getBoundaries().top + global->getBoundaries().height && down){
        newEntityVel.y = -fabs(newEntityVel.y);
          collosionDetectionBoolean[2] = 1; //Down
    }

    if (entityBox.top < global->getBoundaries().top && up){
        newEntityVel.y = fabs(newEntityVel.y);
          collosionDetectionBoolean[3] = 1; //Up
    }

    entity->setVelocity(newEntityVel);

    return collosionDetectionBoolean;
}

//Intersection check.
bool Utilities::entitiesAreIntersecting(Entity* entity1, Entity* entity2)
{
    return entity1->getCurrentSprite()->getGlobalBounds().intersects(entity2->getCurrentSprite()->getGlobalBounds());
}

//Returns 1 if it is touching the ground
bool Utilities::isTouchingGround(Entity* entity, sf::FloatRect groundBox, float safezone)
{
    sf::FloatRect entityBox = sf::FloatRect(
        entity->getPosition().x - entity->getCurrentSprite()->getOrigin().x,
        entity->getPosition().y - entity->getCurrentSprite()->getOrigin().y,
        entity->getCurrentSprite()->getLocalBounds().width,
        entity->getCurrentSprite()->getLocalBounds().height
    );
    if (entityBox.left > groundBox.left && entityBox.left + entityBox.width < groundBox.left + groundBox.width)
        return (entityBox.top + entityBox.height <= groundBox.top + safezone) && (entityBox.top + entityBox.height >= groundBox.top);
    else
        return 0;
}

//Returns 1 if it is more to the right then wallPosX
bool Utilities::isTouchingRightWall(Entity* entity, float wallPosX)
{
    return entity->getPosition().x+entity->getCurrentSprite()->getGlobalBounds().width/2.f >= wallPosX;
}

//Returns 1 if it is more to the left then wallPosX
bool Utilities::isTouchingLeftWall(Entity* entity, float wallPosX)
{
    return entity->getPosition().x+entity->getCurrentSprite()->getGlobalBounds().width/2.f <= wallPosX;
}

//Goes well with a deacceleration factor
//Returns a velocity variation.
sf::Vector2f Utilities::simpleArrowKeysControl(float accelValue, float dt)
{
    sf::Vector2f vel = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        vel += sf::Vector2f(1.f,0.f)*accelValue*dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        vel += sf::Vector2f(-1.f,0.f)*accelValue*dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        vel += sf::Vector2f(0.f,-1.f)*accelValue*dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        vel += sf::Vector2f(0.f,1.f)*accelValue*dt;
    return vel;
}

//Can be used for a simple mouse control
//Returns a velocity variation.
sf::Vector2f Utilities::simpleFollowMovement(float accelValue, float dt, sf::Vector2f entityPos, sf::Vector2f destinyPos)
{
    sf::Vector2f accDir = destinyPos - entityPos;
    const float aTot = vectorModulus(accDir);
    sf::Vector2f vel = (accDir/aTot)*accelValue*dt;
    return vel;
}

//Applies a velocity variation.
bool Utilities::simpleMoveAndJumpControl(float accelValue, float dt, Entity* entity, float jumpVelValue, bool isTouchingGround)
{
    bool justJumped = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        entity->setVelocity(entity->getVelocity() + sf::Vector2f(1.f,0.f)*accelValue*dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        entity->setVelocity(entity->getVelocity() + sf::Vector2f(-1.f,0.f)*accelValue*dt);
    if (isTouchingGround && entity->getVelocity().y>=0)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            entity->setVelocity(sf::Vector2f(entity->getVelocity().x, -1.f*jumpVelValue));
            justJumped = 1;
        }
    }
    return justJumped;
}

//Avoids continuous bouncing at the ground
void Utilities::steadyAtGround(Entity* entity, bool isTouchingGround)
{
    if (isTouchingGround)
    {
        if (entity->getVelocity().y > 0)
            entity->setVelocity(sf::Vector2f(entity->getVelocity().x, 0));
    }
}

//Combines utilities functions for a simple platform movement mechanic
void Utilities::simplePlatformer(float accelValue, float dt, Entity* entity, float jumpVelValue, bool isTouchingGround)
{
    //The gravity
    entity->setVelocity(entity->getVelocity() + sf::Vector2f(0.f, 1000.f*dt));
    //The controls and ground interation
    this->simpleMoveAndJumpControl(accelValue, dt, entity, jumpVelValue, isTouchingGround);
    this->steadyAtGround(entity, isTouchingGround);
}

//Returns random position inside one rectangular area
sf::Vector2f Utilities::getRandomPosInRectArea(sf::FloatRect rect)
{
    sf::Vector2f randomPos = sf::Vector2f(
        rand() % (int)(rect.width+1) + (int)rect.left,
        rand() % (int)(rect.height+1) + (int)rect.top
    );
    return randomPos;
}

//Returns random position inside one circular area
sf::Vector2f Utilities::getRandomPosInCircArea(sf::CircleShape circ)
{
    float angle = rand() % 360 * M_PI/180;
    float rad = rand() % (int)(circ.getRadius()+1);
    float cs = cos(angle); float sn = sin(angle); float tg = sn/cs; float coTg = cs/sn;
    sf::Vector2f randomPos = sf::Vector2f(
        cs/fabs(cs)*rad/sqrt(1+tg*tg)     + circ.getPosition().x,
        sn/fabs(sn)*rad/sqrt(1+coTg*coTg) + circ.getPosition().y
    );
    return randomPos;
}

//Returns random position between two concentric circular areas
sf::Vector2f Utilities::getRandomPosInRingArea(sf::CircleShape inCirc, sf::CircleShape outCirc)
{
    float angle = rand() % 360 * M_PI/180;
    float rad = rand() % (int)(outCirc.getRadius()+1) + (int)inCirc.getRadius();
    float cs = cos(angle); float sn = sin(angle); float tg = sn/cs; float coTg = cs/sn;
    sf::Vector2f randomPos = sf::Vector2f(
        cs/fabs(cs)*rad/sqrt(1+tg*tg)     + inCirc.getPosition().x,
        sn/fabs(sn)*rad/sqrt(1+coTg*coTg) + inCirc.getPosition().y
    );
    return randomPos;
}

//Adds new entity to the entities vector
void Utilities::spawn(std::vector<Entity*> entities, Entity* modelEntity, sf::Vector2f pos)
{
    entities.push_back(new Entity);
    entities.back() = modelEntity;
    entities.back()->setPosition(pos);
}

//Removes entity from the entities vector
void Utilities::despawn(std::vector<Entity*> entities, int entityNum)
{
    entities.erase(entities.begin() + entityNum);
}