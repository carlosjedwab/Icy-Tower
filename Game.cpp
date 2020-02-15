#include "Game.h"

//Private functions
void Game::initWindow()
{
    this->window = new sf::RenderWindow(this->global.getScreenSize(), "Game", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::initEntities()
{
    this->backgroundBack = new BackgroundBack(&global);
    this->backgroundSide = new BackgroundSide(&global);
    this->platforms = new Platforms(&global);
    this->player = new Player(&global);
    //this->newEntity = new NewEntity(&global);/////////////////////////////////////////////////////////////edit
}

//Constructors / Destructors
Game::Game()
{
    this->initWindow();
    this->initEntities();

    music.openFromFile("Sounds/audio1.wav");
    music.setVolume(20);
    music.play();
    music.setLoop(true);

}
Game::~Game()
{
    this->window->~RenderWindow();
    this->backgroundBack->~BackgroundBack();
    this->backgroundSide->~BackgroundSide();
    this->platforms->~Platforms();
    this->player->~Player();
    //delete this->newEntity;/////////////////////////////////////////////////////////////edit
}

//Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

//Functions
void Game::pollEvents()
{
    /*
        @return void

        Event polling (buttons and keys)
    */

    sf::Event event;
    this->global.setEvent(event);
    while (this->window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            this->window->close();
            break;
        }
    }
}

void Game::updateMousePositions()
{
    /*
        @return void

        Updates mouse positions.
        - Mouse position relative to window (Vector2i);
        - Mouse position relative to view (Vector2f).
    */
   
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*this->window);
    sf::Vector2f mousePosView = this->window->mapPixelToCoords(mousePosWindow);
    this->global.setMousePos(mousePosView);
}

void Game::update(float dt)
{
    /*
        @return void

        - Updates the external inputs;
        - Updates the game objects
    */
    this->pollEvents();

    this->updateMousePositions();

    //Update game objects
    this->player->update(dt);
    this->platforms->update(dt);
    //this->newEntity->update(dt);/////////////////////////////////////////////////////////////edit

    //Player/Platform interaction/collision
    int platformNum = 0;
    bool playerIsOnPlatform = 0;
    while (platformNum < this->platforms->getEntities().size()-1)
    {
        if (utilities.isTouchingGround(this->player->getEntity(), this->platforms->getEntities().at(platformNum+1).entityBox,20))
        {
            playerIsOnPlatform = 1;
            platformNum++;
            break;
        }
        platformNum++;
    }
    this->player->setPlayerStatus(playerIsOnPlatform, this->platforms->getEntities().at(platformNum).floorNum);
}

void Game::render()
{
    /*
        @return void

        - Clear old frame;
        - Render objects:
            - Background; //example
            - Player;     //example
            - Enemies.    //example
        - Display frame in window.
    */

    //Clear old frame
    this->window->clear(sf::Color::White);

    //Draw game objects
    this->backgroundBack->render(window);
    this->platforms->render(window);
    this->backgroundSide->render(window);
    this->player->render(window);
    //this->newEntity->render(window);/////////////////////////////////////////////////////////////edit

    //Display window
    this->window->display();
}