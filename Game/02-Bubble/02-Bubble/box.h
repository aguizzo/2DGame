
#ifndef _BOX_INCLUDE
#define _BOX_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Box
{

public:
	void init(const glm::ivec2& tileMapPos, bool inverted, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setContact(bool c);
	void setMid();
	glm::vec2 getPosition() { return posBox; }

private:
	bool mid, inverted, contact;
	glm::ivec2 tileMapDispl, posBox;
	int jumpAngle, jumpHeight, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _BOX_INCLUDE
