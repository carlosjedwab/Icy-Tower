#pragma once

#include "Global.h"

struct sfTextureAndSprites_Reg
{
    sf::Texture* texture;
    std::vector<sf::Sprite*> sprite;

};typedef struct sfTextureAndSprites_Reg sf_TextureAndSprites;

struct sfBufferAndSounds_Reg
{
    sf::SoundBuffer* buffer;
    std::vector<sf::Sound*> sound;

};typedef struct sfBufferAndSounds_Reg sf_BufferAndSounds;

struct sfFrames_Reg
{
    int frameNum;
    int textureNum;
    int spriteNum;
    float duration;
    struct sfFrames_Reg* next;

};typedef struct sfFrames_Reg sf_Frame;

/*
    ENTITY CLASS

    Constructor's parameters: none;

    About this class:
    - This class acts as the general entity type;
    - It stores data such as:
        - Global position;
        - Global velocity;
        - Direction;
        - Textures and sprites;
        - SoundBuffers and sounds;
        - Animation sprite cicles.
*/

class Entity
{
private:
    //Variables
    sf::Vector2f pos;
    sf::Vector2f vel;
    float dir;
    int currentTextureNum, currentSpriteNum;
    int currentBufferNum, currentSoundNum;
    int currentAnimationNum;
    
    //Game objects
    std::vector<sf_TextureAndSprites> allSprites;
    std::vector<sf_BufferAndSounds> allSounds;
    std::vector<sf_Frame*> allAnimations;

    //Private functions
    void initEntity();

public:
    //Constructors / Destructors
    Entity();
    virtual ~Entity();


    //Accessors
    //Position, velocity and orientation
    sf::Vector2f getPosition();
    void         setPosition(sf::Vector2f newPos);
    sf::Vector2f getVelocity();
    void         setVelocity(sf::Vector2f newVel);
    float        getDirection();
    void         setDirection(float newDir);
    
    //Sprite
    sf::Sprite*  getCurrentSprite();            
    void         createTexture(sf::Image image);
    void         createSprites(int textureNum, std::vector<sf::IntRect> spriteBoxes = (std::vector<sf::IntRect>)0);
    void         duplicateSprite(int textureNum, int spriteNum);
    bool         eraseTextureAndItsSprites(int textureNum);
    bool         eraseSprite(int textureNum, int spriteNum);
    void         setCurrentSprite(int newTextureNum, int newSpriteNum);
    int          getCurrentTextureNum();
    int          getCurrentSpriteNum();

    //Sound
    sf::Sound*   getCurrentSound();            
    void         createBuffer(char fileName[]);
    void         createSounds(int bufferNum);
    void         duplicateSound(int bufferNum, int soundNum);
    bool         eraseBufferAndItsSounds(int bufferNum);
    bool         eraseSound(int bufferNum, int soundNum);
    void         setCurrentSound(int newBufferNum, int newSoundNum);
    int          getCurrentBufferNum();
    int          getCurrentSoundNum();

    //Animation
    void         createAnimationCicle(std::vector<int> textureNums, std::vector<int> spriteNums, std::vector<float> durations);
    void         insertNewAnimationFrame(int textureNum, int spriteNum, float duration);
    void         nextAnimationFrame();
    void         setCurrentAnimationFrame(int animationFrameNum);
    void         setAnimationNum(int newAnimationNum);
    int          getCurrentAnimationFrame();
    int          getCurrentAnimationNum();
    float        getCurrentFrameDuration();
};