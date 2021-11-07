#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "HowToPlay.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 18
#define INIT_PLAYER_Y_TILES 9

HowToPlay::HowToPlay()
{
	background = NULL;
		
}

HowToPlay::~HowToPlay()
{

}

void HowToPlay::init()
{
	initShaders();
	currentTime = 0.0f;

	int option = 0;
	initImages();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (!text.init("fonts/MMRock9.ttf"))
		cout << "Could not load font!!!" << endl;
}

void HowToPlay::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getKey('b'))
	{
		Game::instance().changeState('M');
	}
}

void HowToPlay::render() 
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(bgImage);
	arrows->render(arrowsImg);
	up->render(upImg);
	text.render("How to Play ", glm::vec2(320, 80), 30, glm::vec4(1, 1, 1, 1));
	text.render("Move right or left", glm::vec2(400, 200), 14, glm::vec4(1, 1, 1, 1));
	text.render("Jump", glm::vec2(400, 260), 14, glm::vec4(1, 1, 1, 1));
	text.render("Press b to go back", glm::vec2(340, 400), 14, glm::vec4(1, 1, 1, 1));
}


void HowToPlay::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void HowToPlay::initImages()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(960.f, 540.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	bgImage.loadFromFile("images/mega_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom2[2] = { glm::vec2(300.f, 175.f), glm::vec2(80.f + 300.f, 40.f + 175.f) };
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	arrows = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	arrowsImg.loadFromFile("images/leftright.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom3[2] = { glm::vec2(325.f, 240.f), glm::vec2(40.f + 325.f, 40.f + 240.f) };
	glm::vec2 texCoords3[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	up = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);
	upImg.loadFromFile("images/up.png", TEXTURE_PIXEL_FORMAT_RGBA);
}