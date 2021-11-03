#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


enum State {
	MENU, SCENE
};

void Game::init()
{
	bPlay = true;
	glClearColor(0.6f, 0.8f, 0.8f, 1.0f);  //gris azulado
	//glClearColor(0.f, 0.8f, 1.f, 1.0f);   //azul cielo
	State = MENU;
	menu.init();
	scene.init();
}

bool Game::update(int deltaTime)
{
	switch (State) 
	{
	case MENU:
		menu.update(deltaTime);
		break;
	case SCENE:
		scene.update(deltaTime);
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
	case SCENE: 
		scene.render();
	}
	
}

void Game::changeState(char c)
{
	switch (c) 
	{
	case 'M': 
		State = MENU;
		break;
	case 'S':
		State = SCENE;
		break;
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 13)
		this->init();
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





