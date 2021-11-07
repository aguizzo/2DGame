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
	void reset();
	

private:
	void initShaders();
	void collisions();
	void setPlayerSprites(int x, int y);
	void setPlayer2Sprites(int x, int y);
	void setFlagSprites(int x, int y);
	void setFlag2Sprites(int x, int y);
	void setLeverSprites(int x, int y);
	void setBoxSprites(int x, int y);
	void setMap();
	void initLvl(int lvl);
	void leverActivate();
	void removeBarrier();
	void setPressurePlateSprites();
	void setPortalSprites();
	void setPortalSprites2();
	
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
	int flag_x, flag2_x, flag_y, flag2_y;
	int SceneState, PlayerState, camera, scroll;
	bool leverActivated, touchingPortal, pplateActivated;
	TexturedQuad* background;
	Texture bgImage;
	bool audio;
	int count;
	int lvl;
};


#endif // _SCENE_INCLUDE

