#ifndef _PRESSUREPLATE_INCLUDE
#define _PRESSUREPLATE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
class PressurePlate
{
public:
	void init(const glm::ivec2& tileMapPos, bool inverted, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setState(bool st);
	glm::vec2 getPosition() { return posPlate; }

private:
	bool inverted;
	glm::ivec2 tileMapDispl, posPlate;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif // _PRESSUREPLATE_INCLUDE

