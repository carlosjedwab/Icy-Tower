#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
    GLOBAL CLASS

    Constructor's parameters: none;

    About this class:
    - It stores globaly used variables 
      and constants for easy access.
*/

class Global
{
private:
    //Variables
    //Window
    sf::VideoMode videoMode;
    sf::FloatRect boundaries;
    float wallWidth;

    //Mouse position
    sf::Vector2f mousePosView;

    //Poll event
    sf::Event event;

    //Global logic
    sf::Vector2f cameraOfst;

    //Private functiones
    void initVariables();
    
public:
    //Constructors / Destructors
    Global();
    virtual ~Global();

    //Accessors
    sf::VideoMode getScreenSize();
    void          setScreenSize(sf::VideoMode videoMode);
    sf::FloatRect getBoundaries();
    void          setBoundaries(sf::FloatRect boundaries);
    sf::Vector2f  getMousePos();
    void          setMousePos(sf::Vector2f mousePosView);
    sf::Vector2f  getCameraOfst();
    void          setCameraOfst(sf::Vector2f cameraOfst);
    sf::Event     getEvent();
    void          setEvent(sf::Event event);

    float getWallWidth();
};