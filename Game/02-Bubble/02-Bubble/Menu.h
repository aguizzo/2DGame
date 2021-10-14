#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE
#include "TexturedQuad.h"

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

private:
	ShaderProgram texProgram;
	float currentTime;
	TexturedQuad* background;
	glm::mat4 projection;
	Texture bgImage;
};


#endif


