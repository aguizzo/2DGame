#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Flag.h"
#include "Lever.h"
#include "box.h"
#include "portal.h"
#include "TexturedQuad.h"
#include "PressurePlate.h"

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
	Portal* portal;
	Portal* portal2;
	PressurePlate* pplate;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int SceneState, camera, scroll;
	bool leverActivated, touchingPortal, pplateActivated;
	TexturedQuad* background;
	Texture bgImage;
};


#endif // _SCENE_INCLUDE

