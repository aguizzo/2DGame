#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, bool inverted, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setContact(string side);
	glm::vec2 getPosition() { return posPlayer; }
	
private:
	bool bJumping;
	bool fstjump = true;
	bool inverted;
	bool cl, cr, cu;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, jumpHeight, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


