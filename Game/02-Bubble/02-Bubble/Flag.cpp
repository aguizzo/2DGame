#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Flag.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum FlagAnims
{
	FLAG_DOWN, FLAG_UP
};


void Flag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/flag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(36,72), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(FLAG_DOWN, 32);
	sprite->addKeyframe(FLAG_DOWN, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(FLAG_UP, 32);
	sprite->addKeyframe(FLAG_UP, glm::vec2(0.5f, 1.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlag.x), float(tileMapDispl.y + posFlag.y)));

}

void Flag::update(int deltaTime, bool win)
{
	sprite->update(deltaTime);
	if (Game::instance().getKey('w'))
	{
		if (sprite->animation() != FLAG_UP)
			sprite->changeAnimation(FLAG_UP);
	}
	else if (Game::instance().getKey('s'))
	{
		if (sprite->animation() != FLAG_DOWN)
			sprite->changeAnimation(FLAG_DOWN);
	}
	else {
		if (sprite->animation() == FLAG_UP)
			sprite->changeAnimation(FLAG_UP);
		else if (sprite->animation() == FLAG_DOWN)
			sprite->changeAnimation(FLAG_DOWN);
	}

	if (win) {
		if (sprite->animation() != FLAG_UP)
			sprite->changeAnimation(FLAG_UP);
	}

	else {
		if (sprite->animation() != FLAG_DOWN)
			sprite->changeAnimation(FLAG_DOWN);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlag.x), float(tileMapDispl.y + posFlag.y)));
}

void Flag::render()
{
	sprite->render();
}

void Flag::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Flag::setPosition(const glm::vec2& pos)
{
	posFlag = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlag.x), float(tileMapDispl.y + posFlag.y)));
}




