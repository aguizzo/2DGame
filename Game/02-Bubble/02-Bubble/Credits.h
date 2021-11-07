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
	void initImages();

private:
	ShaderProgram texProgram;
	float currentTime;
	TexturedQuad* background, *logo, *logo2;
	glm::mat4 projection;
	Texture bgImage, logoImg, logoImg2;
	Text text;
};


#endif


