#include "Entity.h"

//Private functions
void Entity::initEntity()
{
    this->pos = sf::Vector2f(0.f, 0.f);
    this->vel = sf::Vector2f(0.f, 0.f);
    this->dir = 0.f;
    currentTextureNum   = 0;
    currentSpriteNum    = 0;
    currentBufferNum    = 0;
    currentSoundNum     = 0;
    currentAnimationNum = 0;
}

//Constructors / Destructors
Entity::Entity()
{
    this->initEntity();
}
Entity::~Entity()
{
    while (!this->allSprites.empty())
        this->eraseTextureAndItsSprites(0);
    while (!this->allSounds.empty())
        this->eraseBufferAndItsSounds(0);
}

//Accessors
//Position, velocity and orientation
sf::Vector2f Entity::getPosition()
{
    return pos;
}

void Entity::setPosition(sf::Vector2f newPos)
{
    pos = newPos;
}

sf::Vector2f Entity::getVelocity()
{
    return vel;
}

void Entity::setVelocity(sf::Vector2f newVel)
{
    vel = newVel;
}

float Entity::getDirection()
{
    return dir;
}

void Entity::setDirection(float newDir)
{
    dir = newDir;
}

//Sprite
sf::Sprite* Entity::getCurrentSprite()
{
    return allSprites.at(currentTextureNum).sprite.at(currentSpriteNum);
}

void Entity::createTexture(sf::Image image)
{
    sf_TextureAndSprites sprites;
    allSprites.push_back(sprites);
    allSprites.back().texture = new sf::Texture;
    allSprites.back().texture->loadFromImage(image);
}

void Entity::createSprites(int textureNum, std::vector<sf::IntRect> spriteBoxes)
{
    if (spriteBoxes == (std::vector<sf::IntRect>)0)
    {
        allSprites.at(textureNum).sprite.push_back(new sf::Sprite);
        allSprites.at(textureNum).sprite.back()->setTexture(*allSprites.at(textureNum).texture);
    }
    else
        for (auto &spriteBox : spriteBoxes)
        {
            allSprites.at(textureNum).sprite.push_back(new sf::Sprite);
            allSprites.at(textureNum).sprite.back()->setTexture(*allSprites.at(textureNum).texture);
            allSprites.at(textureNum).sprite.back()->setTextureRect(spriteBox);
        }
}

void Entity::duplicateSprite(int textureNum, int spriteNum)
{
    allSprites.at(textureNum).sprite.push_back(new sf::Sprite);
    allSprites.at(textureNum).sprite.back()->setTexture(*allSprites.at(textureNum).texture);
    allSprites.at(textureNum).sprite.back()->setTextureRect(allSprites.at(textureNum).sprite.at(spriteNum)->getTextureRect());
    allSprites.at(textureNum).sprite.back()->setOrigin(allSprites.at(textureNum).sprite.at(spriteNum)->getOrigin());
    allSprites.at(textureNum).sprite.back()->setPosition(allSprites.at(textureNum).sprite.at(spriteNum)->getPosition());
    allSprites.at(textureNum).sprite.back()->scale(allSprites.at(textureNum).sprite.at(spriteNum)->getScale());
}

bool Entity::eraseTextureAndItsSprites(int textureNum)
{
    allSprites.at(textureNum).texture->~Texture();
    while (!allSprites.at(textureNum).sprite.empty())
    {
        allSprites.at(textureNum).sprite.back()->~Sprite();
        this->allSprites.at(textureNum).sprite.erase(this->allSprites.at(textureNum).sprite.begin() + textureNum);
    }
    this->allSprites.erase(this->allSprites.begin() + textureNum);
    return !(allSprites.empty());
}

bool Entity::eraseSprite(int textureNum, int spriteNum)
{
    allSprites.at(textureNum).sprite.at(spriteNum)->~Sprite();
    this->allSprites.at(textureNum).sprite.erase(this->allSprites.at(textureNum).sprite.begin() + textureNum);
    return !(allSprites.at(textureNum).sprite.empty());
}

void Entity::setCurrentSprite(int newTextureNum, int newSpriteNum)
{
    this->allSprites.at(newTextureNum).sprite.at(newSpriteNum)->setPosition(
        this->allSprites.at(currentTextureNum).sprite.at(currentSpriteNum)->getPosition());
    this->currentSpriteNum = newSpriteNum;
    this->currentTextureNum = newTextureNum;
}

int Entity::getCurrentTextureNum()
{
    return currentTextureNum;
}

int Entity::getCurrentSpriteNum()
{
    return currentSpriteNum;
}

//Sound
sf::Sound* Entity::getCurrentSound()
{
    return allSounds.at(currentBufferNum).sound.at(currentSoundNum);
}

void Entity::createBuffer(char fileName[])
{
    sf_BufferAndSounds sounds;
    allSounds.push_back(sounds);
    allSounds.back().buffer = new sf::SoundBuffer;
    allSounds.back().buffer->loadFromFile(fileName);
}

void Entity::createSounds(int bufferNum)
{
    allSounds.at(bufferNum).sound.push_back(new sf::Sound);
    allSounds.at(bufferNum).sound.back()->setBuffer(*allSounds.at(bufferNum).buffer);
}

void Entity::duplicateSound(int bufferNum, int soundNum)
{
    allSounds.at(bufferNum).sound.push_back(new sf::Sound);
    allSounds.at(bufferNum).sound.back()->setBuffer(*allSounds.at(bufferNum).buffer);
}

bool Entity::eraseBufferAndItsSounds(int bufferNum)
{
    delete allSounds.at(bufferNum).buffer;
    while (!allSounds.at(bufferNum).sound.empty())
    {
        delete allSounds.at(bufferNum).sound.back();
        this->allSounds.at(bufferNum).sound.erase(this->allSounds.at(bufferNum).sound.begin() + bufferNum);
    }
    this->allSounds.erase(this->allSounds.begin() + bufferNum);
    return !(this->allSounds.empty());
}

bool Entity::eraseSound(int bufferNum, int soundNum)
{
    delete allSounds.at(bufferNum).sound.at(soundNum);
    this->allSounds.at(bufferNum).sound.erase(this->allSounds.at(bufferNum).sound.begin() + bufferNum);
    return !(allSounds.at(bufferNum).sound.empty());
}

void Entity::setCurrentSound(int newBufferNum, int newSoundNum)
{
    this->currentSoundNum = newSoundNum;
    this->currentBufferNum = newBufferNum;
}

int Entity::getCurrentBufferNum()
{
    return currentBufferNum;
}

int Entity::getCurrentSoundNum()
{
    return currentSoundNum;
}

//Animation
void Entity::createAnimationCicle(std::vector<int> textureNums, std::vector<int> spriteNums, std::vector<float> durations)
{
    sf_Frame* firstFrame;
    sf_Frame* frame = (sf_Frame*)malloc(sizeof(sf_Frame));
    firstFrame = frame;
    for (int i=0; i<textureNums.size()-1; i++)
    {
        sf_Frame* nextFrame = (sf_Frame*)malloc(sizeof(sf_Frame));
        frame->frameNum = i;
        frame->textureNum = textureNums[i];
        frame->spriteNum = spriteNums[i];
        frame->duration = durations[i];
        frame->next = nextFrame;
        frame = nextFrame;
    }
    frame->frameNum = textureNums.size()-1;
    frame->textureNum = textureNums.back();
    frame->spriteNum = spriteNums.back();
    frame->next = firstFrame;
    allAnimations.push_back(firstFrame);
}

void Entity::insertNewAnimationFrame(int textureNum, int spriteNum, float duration)
{
    sf_Frame* currentAnimationsClone = allAnimations.at(currentAnimationNum);
    while (currentAnimationsClone->next->frameNum != 0);
    sf_Frame* frame = (sf_Frame*)malloc(sizeof(sf_Frame));
    frame->frameNum = currentAnimationsClone->frameNum+1;
    frame->textureNum = textureNum;
    frame->spriteNum = spriteNum;
    frame->duration = duration;
    frame->next = currentAnimationsClone->next;
    currentAnimationsClone->next = frame;
}

void Entity::nextAnimationFrame()
{
    this->allAnimations.at(currentAnimationNum) = allAnimations.at(currentAnimationNum)->next;
    this->setCurrentSprite(
        allAnimations.at(currentAnimationNum)->textureNum,
        allAnimations.at(currentAnimationNum)->spriteNum
    );
}

void Entity::setCurrentAnimationFrame(int animationFrameNum)
{
    while (allAnimations.at(currentAnimationNum)->frameNum != animationFrameNum)
        this->allAnimations.at(currentAnimationNum) = allAnimations.at(currentAnimationNum)->next;
    this->setCurrentSprite(
        allAnimations.at(currentAnimationNum)->textureNum,
        allAnimations.at(currentAnimationNum)->spriteNum
    );
}

void Entity::setAnimationNum(int newAnimationNum)
{
    this->currentAnimationNum = newAnimationNum;
    this->setCurrentAnimationFrame(0);
}

int Entity::getCurrentAnimationFrame()
{
    return this->allAnimations.at(currentAnimationNum)->frameNum;
}

int Entity::getCurrentAnimationNum()
{
    return this->currentAnimationNum;
}

float Entity::getCurrentFrameDuration()
{
    return this->allAnimations.at(currentAnimationNum)->duration;
}
