#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 18
#define INIT_PLAYER_Y_TILES 9

#define FLAG_X 15
#define FLAG_Y 10

#define FLAG2_X 23
#define FLAG2_Y 17


enum SceneState {
	WON, PLAYING
};


Scene::Scene()
{
	map = NULL;
	player = NULL;
	player2 = NULL;
	flag = NULL;
	flag2 = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (player2 != NULL)
		delete player;
	if (flag != NULL)
		delete flag;
}


void Scene::init()
{
	initShaders();
	SceneState = PLAYING;
	map = TileMap::createTileMap("levels/lvl1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	setPlayers();
	setFlags();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	switch (SceneState) 
	{
	case PLAYING:
		currentTime += deltaTime;
		player->update(deltaTime);
		player2->update(deltaTime);
		glm::vec2 currPos1 = player->getPosition();
		glm::vec2 currPos2 = player2->getPosition();
		if ((currPos1.y > SCREEN_HEIGHT / 2) || (currPos2.y < SCREEN_HEIGHT / 2)) init();
		bool win1 = false;
		if (currPos1.x >= (FLAG_X - 2.5) * map->getTileSize() && 
			currPos1.x <= (FLAG_X + 1) * map->getTileSize() &&
			currPos1.y >= (FLAG_Y) * map->getTileSize() &&
			currPos1.y <= (FLAG_Y + 2) *map->getTileSize())
			win1 = true;
		else
			win1 = false;
		flag->update(deltaTime, win1);
		bool win2 = false;
		if (currPos2.x >= (FLAG2_X - 2.5) * map->getTileSize() &&
			currPos2.x <= (FLAG2_X + 1) * map->getTileSize() &&
			currPos2.y <= (FLAG2_Y)*map->getTileSize() &&
			currPos2.y >= (FLAG2_Y - 2) * map->getTileSize())
			win2 = true;
		else
			win2 = false;
		flag2->update(deltaTime, win2);
		if (Game::instance().getKey(50)) {
			Game::instance().changeState('M');
		}
		break;
	}

}

void Scene::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	player2->render();
	flag->render();
	flag2->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::setPlayers() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player2 = new Player();
	player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, texProgram);
	player2->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), SCREEN_HEIGHT - INIT_PLAYER_Y_TILES * map->getTileSize()));
	player2->setTileMap(map);
}

void Scene::setFlags() {
	flag = new Flag();
	flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	flag->setPosition(glm::vec2(FLAG_X * map->getTileSize(), FLAG_Y * map->getTileSize()));
	flag->setTileMap(map);

	flag2 = new Flag();
	flag2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	flag2->setPosition(glm::vec2(FLAG2_X * map->getTileSize(), FLAG2_Y * map->getTileSize()));
	flag2->setTileMap(map);
}

