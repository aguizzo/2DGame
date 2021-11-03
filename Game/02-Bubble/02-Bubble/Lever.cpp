#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lever.h"
#include "Game.h"


enum LeverAnim
{
	ACTIVATED, DEACTIVATED, INVACTIVATED, INVDEACTIVATED
};

void Lever::init(const glm::ivec2& tileMapPos, bool inv, ShaderProgram& shaderProgram)
{
	inverted = inv;
	if (inv) spritesheet.loadFromFile("images/leverinv.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/lever.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(DEACTIVATED, 8);
	sprite->addKeyframe(DEACTIVATED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(ACTIVATED, 8);
	sprite->addKeyframe(ACTIVATED, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(INVDEACTIVATED, 8);
	sprite->addKeyframe(INVDEACTIVATED, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(INVACTIVATED, 8);
	sprite->addKeyframe(INVACTIVATED, glm::vec2(0.5f, 0.5f));

	if (inverted) sprite->changeAnimation(INVDEACTIVATED);
	else sprite->changeAnimation(DEACTIVATED);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(tileMapDispl.x + posLever.x, tileMapDispl.y + posLever.y));
}

void Lever::update(int deltaTime){}

void Lever::render()
{
	sprite->render();
}

void Lever::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Lever::setPosition(const glm::vec2& pos) 
{
	posLever = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLever.x), float(tileMapDispl.y + posLever.y)));
}

void Lever::activate()
{
	if (inverted) sprite->changeAnimation(INVACTIVATED);
	else sprite->changeAnimation(ACTIVATED);
}