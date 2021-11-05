#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


enum State {
	MENU, HOW_TO, CREDITS, SCENE
};

void Game::init()
{
	bPlay = true;
	glClearColor(0.6f, 0.8f, 0.8f, 1.0f);  //gris azulado
	//glClearColor(0.f, 0.8f, 1.f, 1.0f);   //azul cielo
	State = MENU;
	menu.init();
	howTo.init();
	credits.init();
	scene.init();
	engine = irrklang::createIrrKlangDevice();
	engine->setSoundVolume(0.5f);
	if (!engine)
	{
		printf("Could not startup engine\n");
	}
	engine->play2D("sounds/01_Stage_Select.mp3", true);
}

bool Game::update(int deltaTime)
{
	switch (State) 
	{
	case MENU:
		menu.update(deltaTime);
		break;
	case HOW_TO:
		howTo.update(deltaTime);
		break;
	case CREDITS:
		credits.update(deltaTime);
		break;
	case SCENE:
		scene.update(deltaTime);
		break;
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (State)
	{
	case MENU:
		menu.render();
		break;
	case HOW_TO:
		howTo.render();
		break;
	case CREDITS:
		credits.render();
		break;
	case SCENE: 
		scene.render();
	}
	
}

void Game::changeState(char c)
{
	switch (c) 
	{
		case 'M': 
		{
			State = MENU;
			engine->stopAllSounds();
			engine->play2D("sounds/01_Stage_Select.mp3", true);
			break;
		}
		case 'H': 
		{
			State = HOW_TO;
			break;
		}
		case 'C': 
		{
			State = CREDITS;
			engine->stopAllSounds();
			engine->play2D("sounds/15_Epilogue.mp3", true);
			break;
		}
		case 'S':
		{
			State = SCENE;
			scene.reset();
			engine->stopAllSounds();
			engine->play2D("sounds/06_Guts_Man.mp3", true);
			break;
		}
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 'm')
		engine->stopAllSounds();
	
	//if (key == 13)
		
	//this->init();
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setSpecialKey(int key) {
	specialKeyReleased(key);
}

void Game::playSoundEffect(string effect) {
	char const* c = effect.data();
	engine->play2D(c, false);
}

void Game::stopSound() {
	engine->stopAllSounds();
}

