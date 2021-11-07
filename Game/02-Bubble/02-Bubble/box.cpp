#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Box.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define MAX_JUMP_HEIGHT 126
#define FALL_STEP 6




void Box::init(const glm::ivec2& tileMapPos, bool inv, ShaderProgram& shaderProgram)
{
	mid = false;
	jumpHeight = 0;
	if (inv) spritesheet.loadFromFile("images/boxinv.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/box.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(72, 72), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
	inverted = inv;

}

void Box::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!mid) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (contact) {
				if (map->collisionMoveLeft(posBox - glm::ivec2(5, 0), glm::ivec2(72, 72)) == 0)
					posBox.x -= 3;
			}

		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (contact) {
				if (map->collisionMoveRight(posBox + glm::ivec2(5, 0), glm::ivec2(72, 72)) == 0)
					posBox.x += 3;
			}
		}
		if (inverted) {
			posBox.y -= FALL_STEP;
			int coldown = map->collisionMoveDownBoxInv(posBox, glm::ivec2(72, 72), &posBox.y);
			if (coldown == 2) mid = true;
		}
		else {
			posBox.y += FALL_STEP;
			int coldown = map->collisionMoveDownBox(posBox, glm::ivec2(72, 72), &posBox.y);
			if (coldown == 2) mid = true;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
}

void Box::render()
{
	sprite->render();
}

void Box::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Box::setPosition(const glm::vec2& pos)
{
	posBox = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
}

void Box::setContact(bool c) { contact = c; }
void Box::setMid() { mid = true; }