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

#define PROPORTION 1.4f


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
	lever = NULL;
	box = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (player2 != NULL)
		delete player2;
	if (flag != NULL)
		delete flag;
	if (lever != NULL)
		delete lever;
	if (box != NULL)
		delete box;
}


void Scene::init()
{
	initShaders();
	SceneState = PLAYING;
	map = TileMap::createTileMap("levels/lvl1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	setSprites();
	camera = WORLD_WIDTH / 2;
	scroll = 0;
	projection = glm::ortho(float(camera - SCREEN_WIDTH/PROPORTION - scroll), float(camera + SCREEN_WIDTH/PROPORTION + scroll), float(WORLD_HEIGHT/2 + SCREEN_HEIGHT/PROPORTION + scroll*PROPORTION), float(WORLD_HEIGHT/2 - SCREEN_HEIGHT/PROPORTION - scroll*PROPORTION));
	currentTime = 0.0f;
	leverActivated = false;
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
		camera = (currPos1.x + currPos2.x)/2;
		if ((currPos1.y > WORLD_HEIGHT / 2) || (currPos2.y < WORLD_HEIGHT / 2)) init();
		bool win1 = false;
		if (abs(currPos1.x - currPos2.x) > SCREEN_WIDTH / 1.2)
			scroll = abs(abs(currPos1.x - currPos2.x) - SCREEN_WIDTH / 1.2) / 2;
		else scroll = 0;
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
		collisions();
		box->update(deltaTime);
		int bpos = box->getPosition().y;
		if ((bpos >= (WORLD_HEIGHT / 2) - 42) && (bpos <= (WORLD_HEIGHT / 2) + 42)) box->setMid();
		break;
	}
	projection = glm::ortho(float(camera - SCREEN_WIDTH / PROPORTION - scroll), float(camera + SCREEN_WIDTH / PROPORTION + scroll), float(WORLD_HEIGHT / 2 + SCREEN_HEIGHT / PROPORTION + scroll / PROPORTION), float(WORLD_HEIGHT / 2 - SCREEN_HEIGHT / PROPORTION - scroll / PROPORTION));
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
	lever->render();
	box->render();
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


void Scene::setSprites() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	player2 = new Player();
	player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, texProgram);
	player2->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), WORLD_HEIGHT - INIT_PLAYER_Y_TILES * map->getTileSize()));
	player2->setTileMap(map);

	flag = new Flag();
	flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	flag->setPosition(glm::vec2(FLAG_X * map->getTileSize(), FLAG_Y * map->getTileSize()));
	flag->setTileMap(map);

	flag2 = new Flag();
	flag2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	flag2->setPosition(glm::vec2(FLAG2_X * map->getTileSize(), FLAG2_Y * map->getTileSize()));
	flag2->setTileMap(map);

	lever = new Lever();
	lever->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	lever->setPosition(glm::vec2(28 * map->getTileSize(), 11 * map->getTileSize()));
	lever->setTileMap(map);

	box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	box->setPosition(glm::vec2(22 * map->getTileSize(), 7 * map->getTileSize()));
	box->setTileMap(map);
}

void Scene::setMap() {
	player->setTileMap(map);
	player2->setTileMap(map);
	flag->setTileMap(map);
	flag2->setTileMap(map);
	lever->setTileMap(map);
	box->setTileMap(map);
}

void Scene::collisions() {
	if (!leverActivated && (abs(player->getPosition().x + 15 - lever->getPosition().x) <= 36) && (abs(player->getPosition().y + 36 - lever->getPosition().y) <= 36))
	{
		lever->activate();
		leverActivated = true;
		map = TileMap::createTileMap("levels/lvl1np.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		setMap();
	}

	int cr = box->getPosition().x - (player->getPosition().x + 36) + 15;
	int cl = player->getPosition().x + 15 - (box->getPosition().x + 36);
	int cri = box->getPosition().x - (player2->getPosition().x + 36) + 15;
	int cli = player2->getPosition().x + 15 - (box->getPosition().x + 36);
	int cu = box->getPosition().y - (player->getPosition().y + 72);
	int cui = player2->getPosition().y - (box->getPosition().y + 72);
	if ((cr >= 0 && cr <= 36) && (abs(player->getPosition().y + 35 - box->getPosition().y) <= 36)) {
		box->setContact(true);
		player->setContact("right");
	}
	else if ((cri >= 0 && cri <= 36) && (abs(player2->getPosition().y + 35 - box->getPosition().y) <= 36)) {
		box->setContact(true);
		player2->setContact("right");
	}
	else if ((cl >= 0 && cl <= 36) && (abs(player->getPosition().y + 35 - box->getPosition().y) <= 36))
	{
		box->setContact(true);
		player->setContact("left");
	}
	else if ((cli >= 0 && cli <= 36) && (abs(player2->getPosition().y + 35 - box->getPosition().y) <= 36))
	{
		box->setContact(true);
		player2->setContact("left");
	}
	else if ((cu <= 6 && cu >= 0) && (player->getPosition().x + 72 - 15 >= box->getPosition().x) && (player->getPosition().x + 15 <= box->getPosition().x + 72)) {
		player->setPosition(glm::vec2(player->getPosition().x, box->getPosition().y-72));
		player->setContact("up");
	}
	else if ((cui <= 6 && cui >= 0) && (player2->getPosition().x + 72 - 15 >= box->getPosition().x) && (player2->getPosition().x + 15 <= box->getPosition().x + 72)) {
		player2->setPosition(glm::vec2(player->getPosition().x, box->getPosition().y-72));
		player2->setContact("up");
	}
	else
	{
		box->setContact(false);
		player->setContact("");
	}
}
