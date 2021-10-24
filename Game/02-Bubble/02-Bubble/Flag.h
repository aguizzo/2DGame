#pragma once
#ifndef _FLAG_INCLUDE
#define _FLAG_INCLUDE


#include "Sprite.h"
#include "TileMap.h"



class Flag
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, bool win);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, posFlag;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _FLAG_INCLUDE

