#include "Global.h"
 
//Private functions
void Global::initVariables()
{
    float factorX = 1.0, factorY = 1.0;
    this->wallWidth     = 150;
    this->videoMode     = sf::VideoMode((888              )*factorX, 600*factorY);
    sf::VideoMode bound = sf::VideoMode((888 - 2*wallWidth)*factorX, 600*factorY*0.915);
    this->boundaries    = sf::FloatRect(
        videoMode.width/2.f  - bound.width/2.f,     //.left, aka PosX
        0,                                          //.top,  aka PosY
        bound.width,                                //.width
        bound.height                                //.heght
    );

    this->cameraOfst    = sf::Vector2f(0.f, 0.f);
}

//Constructors / Destructors
Global::Global()
{
    this->initVariables();
}

Global::~Global()
{
    
}

//Accessors
sf::VideoMode Global::getScreenSize()
{
    return this->videoMode;
}

void Global::setScreenSize(sf::VideoMode videoMode)
{
    this->videoMode = videoMode;
}

sf::FloatRect Global::getBoundaries()
{
    return this->boundaries;
}

void Global::setBoundaries(sf::FloatRect boundaries)
{
    this->boundaries = boundaries;
}

sf::Vector2f Global::getMousePos()
{
    return this->mousePosView;
}

void Global::setMousePos(sf::Vector2f mousePosView)
{
    this->mousePosView = mousePosView;
}

sf::Vector2f Global::getCameraOfst()
{
    return this->cameraOfst;
}

void Global::setCameraOfst(sf::Vector2f cameraOfst)
{
    this->cameraOfst = cameraOfst;
}

sf::Event Global::getEvent()
{
    return this->event;
}

void Global::setEvent(sf::Event event)
{
    this->event = event;
}

float Global::getWallWidth()
{
    return this->wallWidth;
}