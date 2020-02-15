#include "Platforms.h"
 
//Private functions
void Platforms::initVariables()
{
    //Custom constants
    this->numOfPlatformsTypes = 11;
    this->viewDist = global->getScreenSize().height/3.f;
    this->betweenDist = 100.f;
    this->maxSizeFraction = 2;
    this->minSizeFraction = 4;
    this->safezone = 0.5;
    this->floorNumCheckPoint = 50;
    this->floorNumNextLevel = floorNumCheckPoint*2;

    //Game logic
}

void Platforms::initEntityObjects()
{
    /*
        @return void

        Entity's sprite and initial conditions.
    */

    Entity* modelEntity;
    modelEntity = new Entity;

    //The loading of the texture and the sprites
    char spritesFile[128] = "Images/platforms.png";
    sf::Image image; image.loadFromFile(spritesFile); image.createMaskFromColor(image.getPixel(0,0));
    modelEntity->createTexture(image);
    std::vector<sf::IntRect> spriteBoxes;
    int pixelY = 40, pixelHeight;
    for (int i=0; i<numOfPlatformsTypes; i++)
    {
        while(image.getPixel(10, pixelY) == image.getPixel(0,0))
            pixelY++;
        pixelY += 2;
        pixelHeight = 0;
        while(image.getPixel(10, pixelY+pixelHeight) == image.getPixel(0,0))
            pixelHeight++;
        pixelHeight--;
        spriteBoxes.push_back(sf::IntRect(35,pixelY,29,pixelHeight));
        spriteBoxes.push_back(sf::IntRect(64,pixelY,14,pixelHeight));
        spriteBoxes.push_back(sf::IntRect(81,pixelY,29,pixelHeight));
        pixelY += pixelHeight;
    }
    modelEntity->createSprites(0, spriteBoxes);
    for (int i=0; i<3; i++)
    {
        modelEntity->setCurrentSprite(0, i);
        modelEntity->getCurrentSprite()->setPosition(sf::Vector2f(
            global->getScreenSize().width*0.5,
            global->getScreenSize().height*0.55
        ));
    }
    this->platformPieceWidth  = spriteBoxes[1].width;
    this->platformCornerWidth = spriteBoxes[0].width;
    
    //The initialization of the position
    modelEntity->setPosition(sf::Vector2f(
        global->getScreenSize().width*0.5,
        global->getScreenSize().height*0.55
    ));

    Platform platform;
    platform.entity = modelEntity;
    this->entities.push_back(platform);
}

//Constructors / Destructors
Platforms::Platforms(Global* global)
{
    this->global = global;
    this->initVariables();
    this->initEntityObjects();
    this->newPlatformGenerator(true);
}
Platforms::~Platforms()
{
    while (!entities.empty())
    {
        entities.back().entity->~Entity();
        entities.erase(entities.begin() + entities.size()-1);
    }
}

//Accessors
std::vector<Platform> Platforms::getEntities()
{
    return entities;
}

//Functions
void Platforms::newPlatformGenerator(bool first)
{
    /*
        @return void

        Creates new platform:
            - It creates the platform if the calculated position is close enought or if first==true;
            - It creates the first platform non ramdomly;
            - It creates the other platforms randomly:
                - It sets a size acording to the floor level;
                - It sets a random x position;
    */

    float posY;

    if (first)
        posY = global->getBoundaries().top + global->getBoundaries().height;
    else
        posY = this->entities.back().entity->getPosition().y - betweenDist;
         
    if (first || posY >= global->getBoundaries().top + global->getCameraOfst().y - viewDist)
    {
        Platform platform;
        platform.entity = new Entity;
        float posX;
        float size, level;

        //The new platform building
        if (first)
            platform.floorNum = 0;
        else
            platform.floorNum = this->entities.back().floorNum + 1;
        
        //The first or the new level platform
        if (first || platform.floorNum % floorNumCheckPoint == 0)
        {
            size = global->getBoundaries().width/platformPieceWidth;
            posX = global->getBoundaries().left - platformCornerWidth;
        }
        //The others
        else
        {
            float frac;
            if (platform.floorNum < 241)
                frac = rand() % (14-5+1-((14-5)*platform.floorNum)/241) + 5;
            else if (platform.floorNum < 1000)
                frac = 5;
            else if (platform.floorNum < 1500)
                frac = 4;
            else if (platform.floorNum < 2000)
                frac = 3;
            else if (platform.floorNum < 10000)
                frac = 2;
            else
                frac = 1;

            frac /= 28.0;
            size = global->getBoundaries().width/platformPieceWidth*frac;
            posX = global->getBoundaries().left + rand() % (int)(global->getBoundaries().width - size*platformPieceWidth - 2*platformCornerWidth);
        }
        platform.size = size;
        level = int(platform.floorNum / floorNumNextLevel);
        if (level < numOfPlatformsTypes)
            platform.type = level;
        else
            platform.type = numOfPlatformsTypes - 1; //max level
        
        
        //The new platform creation
        platform.entityBox = sf::FloatRect(posX-safezone*platformCornerWidth, posY,
            size*platformPieceWidth + (2+2*safezone)*platformCornerWidth,
            this->entities.at(0).entity->getCurrentSprite()->getLocalBounds().height
        );
        platform.entity->setPosition(sf::Vector2f(posX, posY));
        this->entities.push_back(platform);
    }
}

void Platforms::deleteFarPlatform()
{
    /*
        @return void

        Deletes a platform too far away down;
    */

    if (this->entities.at(1).entity->getPosition().y - global->getCameraOfst().y > viewDist + global->getScreenSize().height)
    {
        this->entities.at(1).entity->~Entity();
        this->entities.erase(entities.begin() + 1);
    }
}

void Platforms::renderPlatform(sf::RenderTarget* target, int platformNum)
{
    /*
        @return void

        Renders one platform:
            - It renders the corner left corner piece;
            - It renders 'size' center pieces offseted of one another;
            - It renders the right piece.
    */

    this->entities.at(0).entity->setPosition(this->entities.at(platformNum+1).entity->getPosition());

    //The left corner piece rendering
    this->entities.at(0).entity->setCurrentSprite(0, this->entities.at(platformNum+1).type*3+0);   //set sprite
    utilities.cameraAdjusting(global, this->entities.at(0).entity);                     //adjust pos
    target->draw(*this->entities.at(0).entity->getCurrentSprite());                     //draw

    //The middle pieces rendering
    this->entities.at(0).entity->setCurrentSprite(0, this->entities.at(platformNum+1).type*3+1);   //set sprite
    this->entities.at(0).entity->setPosition(sf::Vector2f(
        this->entities.at(0).entity->getPosition().x + platformCornerWidth,            //add offset
        this->entities.at(0).entity->getPosition().y
    ));
    for (int i=0; i<this->entities.at(platformNum+1).size; i++)
    {
        utilities.cameraAdjusting(global, this->entities.at(0).entity);                 //adjust pos
        target->draw(*this->entities.at(0).entity->getCurrentSprite());                 //draw
        this->entities.at(0).entity->setPosition(sf::Vector2f(                          //add offset
            this->entities.at(0).entity->getPosition().x + platformPieceWidth,
            this->entities.at(0).entity->getPosition().y
        ));
    }

    //The right corner piece rendering
    this->entities.at(0).entity->setCurrentSprite(0, this->entities.at(platformNum+1).type*3+2);   //set sprite
    utilities.cameraAdjusting(global, this->entities.at(0).entity);                     //adjust pos
    target->draw(*this->entities.at(0).entity->getCurrentSprite());                     //draw
}

void Platforms::update(float dt)
{
    /*
        @return void

        Updates the entity:
            - It generates new platforms acordind to the cameras advancing;
            - It deletes the bottom platform if too far away;
    */

    this->newPlatformGenerator();
    this->deleteFarPlatform();
}

void Platforms::render(sf::RenderTarget* target)
{
    /*
        @return void

        Renders all the entities.
    */

    for (int i=0; i<entities.size()-1; i++)
        this->renderPlatform(target, i);
}