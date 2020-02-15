#include "BackgroundSide.h"
 
//Private functions
void BackgroundSide::initEntityObjects()
{
    /*
        @return void

        Entity's sprite and initial conditions.
    */

    this->entity = new Entity;

    //The loading of the side background
    char backgroundFile[32] = "Images/sideBackground.png";
    sf::Image image; image.loadFromFile(backgroundFile);
    this->entity->createTexture(image);
    this->entity->createSprites(0);
    this->entity->setCurrentSprite(0, 0);
    this->entity->getCurrentSprite()->scale(
        global->getScreenSize().width/this->entity->getCurrentSprite()->getGlobalBounds().width,
        global->getScreenSize().width/this->entity->getCurrentSprite()->getGlobalBounds().width
    );
    this->entity->getCurrentSprite()->setOrigin(this->entity->getCurrentSprite()->getLocalBounds().width/2.f, 0);

    this->entity->setPosition(sf::Vector2f(global->getScreenSize().width/2.f, 0));
}

//Constructors / Destructors
BackgroundSide::BackgroundSide(Global* global)
{
    this->global = global;
    this->initEntityObjects();
}
BackgroundSide::~BackgroundSide()
{
    this->entity->~Entity();
}

//Accessors
Entity* BackgroundSide::getEntity()
{
    return entity;
}

//Functions
void BackgroundSide::renderOne(sf::RenderTarget* target)
{
    /*
        @return void

        Renders the entity's sprite.
    */

    //The special camera ajusting of the sprite's position
    entity->getCurrentSprite()->setPosition(entity->getPosition() - global->getCameraOfst()*speedFactor);

    //The sprite drawing
    target->draw(*this->entity->getCurrentSprite());
}

void BackgroundSide::render(sf::RenderTarget* target)
{
    /*
        @return void

        Renders the entity's sprites.
    */

    if (entity->getCurrentSprite()->getPosition().y > global->getScreenSize().height)
        this->entity->setPosition(sf::Vector2f(
            this->entity->getPosition().x,
            this->entity->getPosition().y - this->entity->getCurrentSprite()->getGlobalBounds().height
        ));
    if (entity->getCurrentSprite()->getPosition().y + entity->getCurrentSprite()->getGlobalBounds().height < 0)
        this->entity->setPosition(sf::Vector2f(
            this->entity->getPosition().x,
            this->entity->getPosition().y + this->entity->getCurrentSprite()->getGlobalBounds().height
        ));

    int plusOrMinus[2] = {-1, 1};
    bool pom = (global->getCameraOfst().y*speedFactor >= this->entity->getPosition().y);


    //Side background
    this->renderOne(target);
    this->entity->setPosition(sf::Vector2f(
        this->entity->getPosition().x,
        this->entity->getPosition().y + plusOrMinus[pom]*this->entity->getCurrentSprite()->getGlobalBounds().height
    ));
    this->renderOne(target);
    this->entity->setPosition(sf::Vector2f(
        this->entity->getPosition().x,
        this->entity->getPosition().y - plusOrMinus[pom]*this->entity->getCurrentSprite()->getGlobalBounds().height
    ));
}