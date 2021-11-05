#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Flag.h"
#include "Lever.h"
#include "box.h"
#include "TexturedQuad.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void reset();

private:
	void initShaders();
	void collisions();
	void setSprites();
	void setMap();

private:
	TileMap *map;
	Player *player;
	Player* player2;
	Flag* flag;
	Flag* flag2;
	Lever* lever;
	Box* box;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int SceneState;
	int camera, scroll;
	bool leverActivated;
	TexturedQuad* background;
	Texture bgImage;
	bool audio;
	int count;
};


#endif // _SCENE_INCLUDE

