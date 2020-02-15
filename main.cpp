#include <chrono>
#include "Game.h"

int main()
{
    //Init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    //Init Game engine
    Game game;

    //Game loop
    while (game.running())
    {
        float dt;
        auto start = std::chrono::steady_clock::now();

        //Update
        game.update(dt);

        //Render
        game.render();

        auto end = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()*1e-9;
    }

    //End of application
    return 0;
}