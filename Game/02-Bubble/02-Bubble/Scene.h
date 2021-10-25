#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Flag.h"


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

private:
	void initShaders();
	void setPlayers();
	void setFlags();

private:
	TileMap *map;
	Player *player;
	Player* player2;
	Flag* flag;
	Flag* flag2;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int SceneState;
};


#endif // _SCENE_INCLUDE

