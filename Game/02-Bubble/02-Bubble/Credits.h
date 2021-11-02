#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE
#include "TexturedQuad.h"
#include "Text.h"

class Credits
{
public:
	Credits();
	~Credits();

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
	Texture image;
	Text text;
};


#endif


