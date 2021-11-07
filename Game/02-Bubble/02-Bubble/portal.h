#ifndef _PORTAL_INCLUDE
#define _PORTAL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
class Portal
{
public:
	void init(const glm::ivec2& tileMapPos, bool color, bool side, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool getSide() { return side; }
	glm::vec2 getPosition() { return posPortal; }

private:
	bool color, side; //0 = amarillo, izq //1 = azul, derecha
	glm::ivec2 tileMapDispl, posPortal;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif _PORTAL_INCLUDE