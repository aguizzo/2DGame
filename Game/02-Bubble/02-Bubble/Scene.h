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
	void setLeverSprites(int x, int y, bool inv);
	void setBoxSprites(int x, int y, bool inv);
	void setMap();
	void initLvl(int lvl);
	void leverActivate();
	void removeBarrier();
	void setPressurePlateSprites(int x, int y, bool inv);
	void setPortalSprites(int x, int y, bool inv, bool side);
	void setPortalSprites2(int x, int y, bool inv, bool side);
	
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
	int SceneState, PlayerState, camerax, cameray, scroll;
	bool leverActivated, touchingPortal, pplateActivated;
	TexturedQuad *background, *space;
	Texture bgImage, bgImage2;
	bool audio;
	int count;
	int lvl;
};


#endif // _SCENE_INCLUDE

