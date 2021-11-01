#ifndef _LEVER_INCLUDE
#define _LEVER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
class Lever
{
public:
	void init(const glm::ivec2& tileMapPos, bool inverted, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void activate();
	glm::vec2 getPosition() { return posLever; }

private:
	bool inverted;
	glm::ivec2 tileMapDispl, posLever;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif // _LEVER_INCLUDE