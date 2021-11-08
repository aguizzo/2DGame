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
	void initImages();

private:
	ShaderProgram texProgram;
	float currentTime;
	TexturedQuad* background, *arrows, *up, *flag, *flag2;
	glm::mat4 projection;
	Texture bgImage, arrowsImg, upImg, flagImg;
	Text text;
};


#endif


