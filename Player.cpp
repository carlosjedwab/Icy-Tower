#include "Player.h"
 
//Private functions
void Player::initVariables()
{
    //Custom constants
    this->accelValue = 750.f;
    this->deaccelFactor = 1.02f;
    this->minJumpVelValue = 800.f;
    this->minSpriteScreenSpeed = 30;
    this->grav = 2000;

    //Game logic
    this->animationCounter = 0;
    this->comboCounter = 4.0;
    this->prevFloor = 0;
    this->curFloor = 0;
    this->maxFloor = 0;

    this->playerIsOnPlatform = 0;
    this->didJumped = 0;
    this->comboOn = 0;
}

void Player::initEntityObjects()
{
    /*
        @return void

        Entity's sprites, sounds and initial conditions.
    */

    this->entity = new Entity;

    //The loading of the texture and the sprites
    char spritesFile[128] = "Images/PC Computer - Icy Tower - Harold the Homeboy & Disco Dave.png";
    sf::Image image; image.loadFromFile(spritesFile); image.createMaskFromColor(image.getPixel(0,0));
    this->entity->createTexture(image);
    int y = 8, h = 58;
    std::vector<sf::IntRect> spriteBoxes = {
        sf::IntRect(3,y,34,h), sf::IntRect(41,y,34,h), sf::IntRect(77,y,34,h), sf::IntRect(117,y,30,h), sf::IntRect(154,y,30,h),
        sf::IntRect(191,y,30,h), sf::IntRect(229,y,30,h), sf::IntRect(267,y,32,h), sf::IntRect(305,y,34,h), sf::IntRect(346,y,30,h),
        sf::IntRect(384,y,34,h), sf::IntRect(430,y,46,h), sf::IntRect(485,y,41,h), sf::IntRect(533,y,39,h), sf::IntRect(582,y,32,h)
    };
    this->entity->createSprites(0, spriteBoxes);

    for (int i=0; i<15; i++)
    {
        this->entity->setCurrentSprite(0,i);
        this->entity->getCurrentSprite()->setOrigin(
            this->entity->getCurrentSprite()->getLocalBounds().width/2.f,
            this->entity->getCurrentSprite()->getLocalBounds().height/2.f
        );
    }

    //The building of the animation cicles
    std::vector<int> textureNums;
    std::vector<int> spriteNums;
    std::vector<float> durations;

    //Animation cicle 1 - Standing still
    textureNums.push_back(0); spriteNums.push_back(0); durations.push_back(0.2);
    textureNums.push_back(0); spriteNums.push_back(1); durations.push_back(0.2);
    textureNums.push_back(0); spriteNums.push_back(2); durations.push_back(0.2);
    this->entity->createAnimationCicle(textureNums, spriteNums, durations);
    textureNums.clear(); spriteNums.clear(); durations.clear();

    //Animation cicle 2 - Running
    textureNums.push_back(0); spriteNums.push_back(3); durations.push_back(0.1);
    textureNums.push_back(0); spriteNums.push_back(4); durations.push_back(0.1);
    textureNums.push_back(0); spriteNums.push_back(5); durations.push_back(0.1);
    textureNums.push_back(0); spriteNums.push_back(6); durations.push_back(0.1);
    this->entity->createAnimationCicle(textureNums, spriteNums, durations);
    textureNums.clear(); spriteNums.clear(); durations.clear();

    //OBSERVATION: Jumping and combo animations are not frames cicles

    //The loading of the sound buffer and the sound
    char soundFile1[32] = "Sounds/jump.wav";
    this->entity->createBuffer(soundFile1);
    this->entity->createSounds(0);
    this->entity->setCurrentSound(0,0);
    this->entity->getCurrentSound()->setPitch(1.2);
    this->entity->getCurrentSound()->setVolume(400);

    char soundFile2[32] = "Sounds/comboJump.wav";
    this->entity->createBuffer(soundFile2);
    this->entity->createSounds(1);
    this->entity->setCurrentSound(1,0);
    this->entity->getCurrentSound()->setPitch(1.2);
    
    //The initialization of the position
    this->entity->setPosition(sf::Vector2f(
        global->getScreenSize().width*0.5,
        global->getScreenSize().height*0.8
    ));
    utilities.cameraAdjusting(global, entity);
}

//Constructors / Destructors
Player::Player(Global* global)
{
    this->global = global;
    this->initVariables();
    this->initEntityObjects();
}
Player::~Player()
{
    this->entity->~Entity();
}

//Accessors
Entity* Player::getEntity()
{
    return entity;
}

void Player::setPlayerStatus(bool playerIsOnPlatform, int newCurFloor)
{
    //Player's landing status
    if (this->playerIsOnPlatform == 0 && playerIsOnPlatform == 1 && this->entity->getVelocity().y >= 0)
        justLanded = 1;
    else
        justLanded = 0;

    //Player's previous, current and maximum floor status
    if (justLanded)
    {
        this->prevFloor = this->curFloor;
    
        this->curFloor = newCurFloor;
        if (newCurFloor > maxFloor)
            this->maxFloor = newCurFloor;
    }
    
    this->playerIsOnPlatform = playerIsOnPlatform;
}

//Functions
void Player::gravity(float dt)
{
    //The gravity
    this->entity->setVelocity(entity->getVelocity() + sf::Vector2f(0.f, grav*dt));
}

void Player::movementComboAndSounds(float dt)
{
    //The deceleration due to drag
    this->entity->setVelocity(this->entity->getVelocity()/deaccelFactor);

    //Platform like controls
    if (comboOn)
        jumpVelValue = 4.2*pow(fabs(this->entity->getVelocity().x),0.9) + minJumpVelValue;
    else
        jumpVelValue = 2.4*pow(fabs(this->entity->getVelocity().x),0.9) + minJumpVelValue;
    //The controls and ground interation
    this->gravity(dt);
    didJumped = justJumped;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        entity->setVelocity(entity->getVelocity() + sf::Vector2f(1.f,0.f)*accelValue*dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        entity->setVelocity(entity->getVelocity() + sf::Vector2f(-1.f,0.f)*accelValue*dt);
    if (playerIsOnPlatform && entity->getVelocity().y>=0)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (comboCounter <= 3.0 && curFloor >= prevFloor + 2)
            {
                comboOn = 1;
                this->entity->setCurrentSound(1,0);
            }
            else 
            {
                comboOn = 0;
                this->entity->setCurrentSound(0,0);
            }

            comboCounter = comboCounterAux;

            this->entity->getCurrentSound()->play();
            
            entity->setVelocity(sf::Vector2f(entity->getVelocity().x, -1.f*jumpVelValue));
            justJumped = 1;
        }
    }
}

void Player::interactions()
{
    //The boundary bouncing. Boundaries collision detection booleans: right(0), left(1), down(2) and up(3)
    if (playerIsOnPlatform)
    {
        if (entity->getVelocity().y > 0)
            entity->setVelocity(sf::Vector2f(entity->getVelocity().x, 0));
    }
    utilities.boundaryCollision(global, entity, 1,1,0,0);
    utilities.steadyAtGround(entity, playerIsOnPlatform);
}

void Player::animationEffects(float dt)
{
    /*
        @return void

        Animates the entity's sprite:
            - It cycles through the running animation frames;
            - It has a not moving sprite;
            - It faces different sides due to the x velocity direction
              and the arrow keys control.
    */

    //if in the air
    if (!playerIsOnPlatform || this->entity->getVelocity().y < 0)
    {
        if (comboOn)
        {
            if (this->entity->getCurrentSpriteNum() != 11)
            {
                this->entity->setCurrentSprite(0, 11);
                this->entity->getCurrentSprite()->setRotation(0);
                if (this->entity->getVelocity().x <= 0)
                    comboSide = 1;
                else
                    comboSide = 0;             
            }
            if (comboSide == 1)
                this->entity->getCurrentSprite()->rotate(10);
            else if (comboSide == 0)
                this->entity->getCurrentSprite()->rotate(-10);
        }
        else
        {
            //side jumping frames
            if (fabs(this->entity->getVelocity().x) > 25.f)
            {
                if (this->entity->getVelocity().y < 0)
                    this->entity->setCurrentSprite(0, 8);
                else if (this->entity->getVelocity().y > 0)
                    this->entity->setCurrentSprite(0, 10);
                if (fabs(this->entity->getVelocity().y) < 15)
                    this->entity->setCurrentSprite(0, 9);
            }
            //front jumping frames
            else
                this->entity->setCurrentSprite(0, 7);

            //face the side of the velocity X (unless a pressed arrow key say otherwise)
            if (this->entity->getVelocity().x >= 0)
                this->entity->getCurrentSprite()->setScale(1.f,1.f);
            else
                this->entity->getCurrentSprite()->setScale(-1.f,1.f);

            //face the side os the pressed arrow key (priority over the last setScale pair)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                this->entity->getCurrentSprite()->setScale(1.f,1.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                this->entity->getCurrentSprite()->setScale(-1.f,1.f);
        }
    }
    //if running
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ^ sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //if this animation wasn't active just before, set it active
        if (this->entity->getCurrentAnimationNum() != 1)
        {
            animationCounter = 0;
            this->entity->setAnimationNum(1);
        }
        //The timer activation and the animation cicle advance
        if (animationCounter >= this->entity->getCurrentFrameDuration())
        {
            animationCounter = 0;
            this->entity->nextAnimationFrame();
        }

        //The side facing
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            this->entity->getCurrentSprite()->setScale(1.f,1.f);
        else
            this->entity->getCurrentSprite()->setScale(-1.f,1.f);
    }
    //if steady
    else
    {
        //if this animation wasn't active just before, set it active
        if (this->entity->getCurrentAnimationNum() != 0)
        {
            animationCounter = 0;
            this->entity->setAnimationNum(0);
            this->entity->getCurrentSprite()->setScale(1.f,1.f);
        }
        //The timer activation and the animation cicle advance
        if (animationCounter >= this->entity->getCurrentFrameDuration())
        {
            animationCounter = 0;
            this->entity->nextAnimationFrame();
        }
    }
}

void Player::playersCameraSetting(float dt)
{
    //The camera following the player with a speed offset depending on the sprite's position
    if (this->entity->getVelocity().y < 0)
    {
        if (this->entity->getPosition().y - global->getCameraOfst().y > global->getBoundaries().height/2.f)
            spriteScreenSpeed = this->entity->getVelocity().y;
        else
            spriteScreenSpeed = this->entity->getVelocity().y*(this->entity->getCurrentSprite()->getPosition().y)/global->getScreenSize().height;
    }
    else
        spriteScreenSpeed = this->entity->getVelocity().y;

    //The minimal camera scrolling speed (active only after a certain height in game)
    if (maxFloor > 5)
        spriteScreenSpeed += minSpriteScreenSpeed;
    this->entity->getCurrentSprite()->setPosition(this->entity->getCurrentSprite()->getPosition() + sf::Vector2f(0.f, spriteScreenSpeed*dt));

    //The fixation of the camera on this entity Y position
    utilities.cameraStickToY(global, entity);
}

void Player::update(float dt)
{
    /*
        @return void

        Updates the entity:
            - It suffers by drag;
            - It accelerates due to the external inputs;
            - It has a combo mode;
            - It makes sounds when jumping;
            - It bounces off walls and platforms;
            - It changes sprites so it looks like it's running;
            - It interacts with the players camera.
    */

    if (justLanded)
    {
        comboCounterAux = 0;
    }

    comboCounter += dt;
    comboCounterAux += dt;
    animationCounter += dt;

    this->movementComboAndSounds(dt);
    this->interactions();

    //The velocity aplication (pos = pos + vel*dt)
    this->entity->setPosition(this->entity->getPosition() + this->entity->getVelocity() * dt);

    this->animationEffects(dt);
    this->playersCameraSetting(dt);
}

void Player::render(sf::RenderTarget* target)
{
    /*
        @return void

        Renders the entity.
    */

    //The camera ajusting of the sprite's position
    utilities.cameraAdjusting(global, entity);

    //The sprite drawing
    target->draw(*this->entity->getCurrentSprite());
}