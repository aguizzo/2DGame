#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Credits.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 18
#define INIT_PLAYER_Y_TILES 9

Credits::Credits()
{
	background = NULL;
		
}

Credits::~Credits()
{

}

void Credits::init()
{
	initShaders();
	currentTime = 0.0f;
	int option = 0;
	initImages();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (!text.init("fonts/MMRock9.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Credits::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getKey('b'))
	{
		Game::instance().changeState('M');
	}
}

void Credits::render() 
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(bgImage);
	logo->render(logoImg);
	logo2->render(logoImg2);
	text.render("NAMEGEM", glm::vec2(260, 100), 64, glm::vec4(0, 0, 1, 1));
	text.render("A GAME INSPIRED IN SISAO, DEVELOPED BY", glm::vec2(200, 170), 14, glm::vec4(1, 1, 1, 1));
	text.render("Havana 24", glm::vec2(390, 200), 14, glm::vec4(1, 1, 1, 1));
	text.render("DEVELOPER TEAM STAFF", glm::vec2(310, 270), 14, glm::vec4(1, 1, 1, 1));
	text.render("Oscar Polonio", glm::vec2(360, 300), 14, glm::vec4(1, 1, 1, 1));
	text.render("Andres Guizzo", glm::vec2(360, 330), 14, glm::vec4(1, 1, 1, 1));
	text.render("MUSIC BY", glm::vec2(390, 370), 14, glm::vec4(1, 1, 1, 1));
	text.render("Manami Matsumae", glm::vec2(360, 400), 14, glm::vec4(1, 1, 1, 1));
	text.render("GAME AESTHETICS INSPIRED IN", glm::vec2(280, 440), 14, glm::vec4(1, 1, 1, 1));
	
}


void Credits::initShaders()
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

void Credits::initImages()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(960.f, 540.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	bgImage.loadFromFile("images/mega_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom2[2] = { glm::vec2(420.f, 210.f), glm::vec2(50.f + 420.f, 40.f + 210.f) };
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	logo = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	logoImg.loadFromFile("images/havana.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom3[2] = { glm::vec2(380.f, 470.f), glm::vec2(150.f + 380.f, 35.f + 470.f) };
	glm::vec2 texCoords3[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	logo2 = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);
	logoImg2.loadFromFile("images/megamanlogo.png", TEXTURE_PIXEL_FORMAT_RGBA);
}