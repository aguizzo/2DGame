#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "portal.h"
#include "Game.h"



enum PortalAnims
{
	IZQ, DER
};


void Portal::init(const glm::ivec2& tileMapPos, bool clr, bool s, ShaderProgram& shaderProgram)
{
	color = clr;
	side = s;
	if (clr) spritesheet.loadFromFile("images/portalazul.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/portalamarillo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(36, 72), glm::vec2(1./3.f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(DER, 4);
	sprite->addKeyframe(DER, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DER, glm::vec2(1./3.f, 0.f));
	sprite->addKeyframe(DER, glm::vec2(2./3.f, 0.f));
	sprite->addKeyframe(DER, glm::vec2(1. / 3.f, 0.f));

	sprite->setAnimationSpeed(IZQ, 4);
	sprite->addKeyframe(IZQ, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(IZQ, glm::vec2(1. / 3.f, 0.5f));
	sprite->addKeyframe(IZQ, glm::vec2(2. / 3.f, 0.5f));
	sprite->addKeyframe(IZQ, glm::vec2(1. / 3.f, 0.5f));

	if(s) sprite->changeAnimation(DER);
	else sprite->changeAnimation(IZQ);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));

}

void Portal::update(int deltaTime){
	sprite->update(deltaTime);
}

void Portal::render()
{
	sprite->render();
}

void Portal::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Portal::setPosition(const glm::vec2& pos)
{
	posPortal = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));
}



