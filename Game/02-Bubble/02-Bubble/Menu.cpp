#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Menu.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 18
#define INIT_PLAYER_Y_TILES 9

Menu::Menu()
{
	map = NULL;
	background = NULL;
		
}

Menu::~Menu()
{
	if (map != NULL)
		delete map;
}

void Menu::init()
{
	initShaders();
	currentTime = 0.0f;
		
	int option = 0;

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(960.f, 540.f)};
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	bgImage.loadFromFile("images/mega_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);
	cursorImg.loadFromFile("images/Cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (!text.init("fonts/MMRock9.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		Game::instance().setSpecialKey(GLUT_KEY_DOWN);
		option += 1;
		option = option % 3;
		Game::instance().playSoundEffect("sounds/11_Dink.wav");
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		Game::instance().setSpecialKey(GLUT_KEY_UP);
		option += 2;
		option = option % 3;
		Game::instance().playSoundEffect("sounds/11_Dink.wav");
	}
	if (Game::instance().getKey(13)) 
	{	
		switch (option) 
		{
		case 0:
			Game::instance().changeState('S');
			break;
		case 1:
			Game::instance().changeState('H');
			break;
		case 2:
			Game::instance().changeState('C');
			break;
		}		
	}
}

void Menu::render() 
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(bgImage);
	text.render("NAMEGEM", glm::vec2(450, 200), 64, glm::vec4(0, 0, 1, 1));
	updateMenuOptions();
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	cursor->render(cursorImg);
}

void Menu::updateMenuOptions() {
	if (option == 0)
	{
		text.render("Play ", glm::vec2(570, 380), 30, glm::vec4(0, 1, 1, 1));
		text.render("How to play ", glm::vec2(570, 420), 14, glm::vec4(1, 1, 1, 1));
		text.render("Credits ", glm::vec2(570, 460), 14, glm::vec4(1, 1, 1, 1));
		glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(0.f + 64.f, 0.f + 64.f) };
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	}
	else if (option == 1)
	{
		text.render("Play ", glm::vec2(570, 380), 14, glm::vec4(1, 1, 1, 1));
		text.render("How to play ", glm::vec2(570, 420), 30, glm::vec4(0, 1, 1, 1));
		text.render("Credits ", glm::vec2(570, 460), 14, glm::vec4(1, 1, 1, 1));
		glm::vec2 geom2[2] = { glm::vec2(1120.f, 600.f), glm::vec2(1120 + 64.f, 600 + 64.f) };
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	}
	else if (option == 2)
	{
		text.render("Play ", glm::vec2(570, 380), 14, glm::vec4(1, 1, 1, 1));
		text.render("How to play ", glm::vec2(570, 420), 14, glm::vec4(1, 1, 1, 1));
		text.render("Credits ", glm::vec2(570, 460), 30, glm::vec4(0, 1, 1, 1));
		glm::vec2 geom2[2] = { glm::vec2(1010.f, 660.f), glm::vec2(1010 + 64.f, 660 + 64.f) };
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	}
}

void Menu::initShaders()
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