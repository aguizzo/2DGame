#ifndef _HOWTOPLAY_INCLUDE
#define _HOWTOPLAY_INCLUDE
#include "TexturedQuad.h"
#include "Text.h"

class HowToPlay
{
public:
	HowToPlay();
	~HowToPlay();

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


