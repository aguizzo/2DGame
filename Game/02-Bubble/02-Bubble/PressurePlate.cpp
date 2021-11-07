#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "PressurePlate.h"
#include "Game.h"


enum PressurePlateAnim
{
	ACTIVATED, DEACTIVATED, INVACTIVATED, INVDEACTIVATED
};

void PressurePlate::init(const glm::ivec2& tileMapPos, bool inv, ShaderProgram& shaderProgram)
{
	inverted = inv;
	if (inv) spritesheet.loadFromFile("images/pplateinv.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/pplate.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(72, 36), glm::vec2(0.5f, 1.f), &spritesheet, &shaderProgram);

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
	sprite->setPosition(glm::vec2(tileMapDispl.x + posPlate.x, tileMapDispl.y + posPlate.y));
}

void PressurePlate::update(int deltaTime) {
	sprite->update(deltaTime);
}

void PressurePlate::render()
{
	sprite->render();
}

void PressurePlate::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void PressurePlate::setPosition(const glm::vec2& pos)
{
	posPlate = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlate.x), float(tileMapDispl.y + posPlate.y)));
}

void PressurePlate::setState(bool st)
{
	if (inverted) {
		if (st) sprite->changeAnimation(INVACTIVATED);
		else sprite->changeAnimation(INVDEACTIVATED);
	}
	else {
		if (st) sprite->changeAnimation(ACTIVATED);
		else sprite->changeAnimation(DEACTIVATED);
	}
}