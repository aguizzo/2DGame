#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"

class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void updateMenuOptions();

private:
	ShaderProgram texProgram;
	float currentTime;
	TexturedQuad* background, *cursor;
	glm::mat4 projection;
	Texture bgImage, cursorImg;
	TileMap *map;
	Text text;
	int option;
};


#endif


