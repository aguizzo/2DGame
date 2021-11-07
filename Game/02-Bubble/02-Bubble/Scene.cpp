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
	pplate = NULL;
	portal = NULL;
	portal2 = NULL;
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
	if (pplate != NULL)
		delete pplate;
	if (portal != NULL)
		delete portal;
	if (portal2 != NULL)
		delete portal2;
}


void Scene::init()
{
	initShaders();
	leverActivated = false;
	touchingPortal = false;
	pplateActivated = false;
	SceneState = PLAYING;
	map = TileMap::createTileMap("levels/lvl1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1800.f, 1000.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	bgImage.loadFromFile("images/mega_bg2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	setSprites();
	camera = WORLD_WIDTH / 2;
	scroll = 0;
	projection = glm::ortho(float(camera - SCREEN_WIDTH/PROPORTION - scroll), float(camera + SCREEN_WIDTH/PROPORTION + scroll), float(WORLD_HEIGHT/2 + SCREEN_HEIGHT/PROPORTION + scroll*PROPORTION), float(WORLD_HEIGHT/2 - SCREEN_HEIGHT/PROPORTION - scroll*PROPORTION));
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	switch (SceneState) 
	{
	case PLAYING:
		currentTime += deltaTime;
		portal->update(deltaTime);
		portal2->update(deltaTime);
		player->update(deltaTime);
		player2->update(deltaTime);
		glm::vec2 currPos1 = player->getPosition();
		glm::vec2 currPos2 = player2->getPosition();
		camera = (currPos1.x + currPos2.x)/2;
		if (((currPos1.y + 32 > WORLD_HEIGHT / 2 - 3)  && (currPos1.y + 32 < WORLD_HEIGHT / 2 + 3)) || ((currPos2.y + 32 > WORLD_HEIGHT / 2 - 3) && (currPos2.y + 32 < WORLD_HEIGHT / 2 + 3))) init();
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
		if ((bpos >= (WORLD_HEIGHT / 2) - 36) && (bpos + 72 <= (WORLD_HEIGHT / 2) + 36)) box->setMid();
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
	background->render(bgImage);
	map->render();
	player->render();
	player2->render();
	flag->render();
	flag2->render();
	lever->render();
	box->render();
	if (pplateActivated) portal->render();
	portal2->render();
	pplate->render();
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

	pplate = new PressurePlate();
	pplate->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	pplate->setPosition(glm::vec2(25 * map->getTileSize(), 11 * map->getTileSize()));
	pplate->setTileMap(map);

	box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	box->setPosition(glm::vec2(22 * map->getTileSize(), 7 * map->getTileSize()));
	box->setTileMap(map);

	portal = new Portal();
	portal->init(glm::ivec2(SCREEN_X, SCREEN_Y), 1, 1, texProgram);
	portal->setPosition(glm::vec2(33 * map->getTileSize(), 19 * map->getTileSize()));
	portal->setTileMap(map);

	portal2 = new Portal();
	portal2->init(glm::ivec2(SCREEN_X, SCREEN_Y), 0, 0, texProgram);
	portal2->setPosition(glm::vec2(12 * map->getTileSize(), 10 * map->getTileSize()));
	portal2->setTileMap(map);
}

void Scene::setMap() {
	player->setTileMap(map);
	player2->setTileMap(map);
	flag->setTileMap(map);
	flag2->setTileMap(map);
	lever->setTileMap(map);
	box->setTileMap(map);
	portal->setTileMap(map);
}

void Scene::collisions() {
	glm::ivec2 playerp = player->getPosition();
	glm::ivec2 player2p = player2->getPosition();
	glm::ivec2 boxp = box->getPosition();
	glm::ivec2 portalp = portal->getPosition();
	glm::ivec2 portal2p = portal2->getPosition();
	glm::ivec2 leverp = lever->getPosition();
	glm::ivec2 pplatep = pplate->getPosition();

	if (!leverActivated && (abs(playerp.x + 15 - leverp.x) <= 36) && (abs(playerp.y + 36 - leverp.y) <= 36))
	{
		lever->activate();
		leverActivated = true;
		map = TileMap::createTileMap("levels/lvl1np.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		setMap();
	}

	if (((abs(playerp.x - pplatep.x) <= 36) && (abs(playerp.y + 36 - pplatep.y) <= 36)) || ((abs(boxp.x - pplatep.x) <= 36) && (abs(boxp.y + 36 - pplatep.y) <= 36)))
	{
		pplate->setState(true);
		pplateActivated = true;
	}
	else {
		pplate->setState(false);
		pplateActivated = false;
	}

	int cr = boxp.x - (playerp.x + 36) + 15;
	int cl = playerp.x + 15 - (boxp.x + 36);
	int cri = boxp.x - (player2p.x + 36) + 15;
	int cli = player2p.x + 15 - (boxp.x + 36);
	int cu = boxp.y - (playerp.y + 72);
	int cui = player2p.y - (boxp.y + 72);

	if ((cr >= 0 && cr <= 36) && (abs(playerp.y + 35 - boxp.y) <= 36)) {
		box->setContact(true);
		player->setContact("right");
	}
	else if ((cl >= 0 && cl <= 36) && (abs(playerp.y + 35 - boxp.y) <= 36))
	{
		box->setContact(true);
		player->setContact("left");
	}
	else if ((cri >= 0 && cri <= 36) && (abs(player2p.y - 35 - boxp.y) <= 36)) {
		box->setContact(true);
		player2->setContact("right");
	}
	else if ((cli >= 0 && cli <= 36) && (abs(player2p.y - 35 - boxp.y) <= 36))
	{
		box->setContact(true);
		player2->setContact("left");
	}
	else
	{
		box->setContact(false);
		player->setContact("");
		player2->setContact("");
	}


	if ((cu <= 6 && cu >= 0) && (playerp.x + 72 - 15 >= boxp.x) && (playerp.x + 15 <= boxp.x + 72)) {
		player->setPosition(glm::vec2(playerp.x, boxp.y - 72));
		player->setContact("up");
		box->setContact(false);
	}

	if ((cui <= 6 && cui >= 0) && (player2p.x + 72 - 15 >= boxp.x) && (player2p.x + 15 <= boxp.x + 72)) {
		player2->setPosition(glm::vec2(player2p.x, boxp.y + 72));
		player2->setContact("up");
		box->setContact(false);
	}
	if (pplateActivated) {
		if (portal->getSide()) {  //entrar al portal 1 por su izquierda
			int distr1 = portalp.x + 36 - (playerp.x + 57);
			int distr2 = portalp.x + 36 - (player2p.x + 57);
			if (!touchingPortal && distr1 <= 6 && distr1 >= -6 && (playerp.y + 36 >= portalp.y) && (playerp.y + 36 <= portalp.y + 72)) {
				touchingPortal = true;
				if (portal2->getSide()) player->setPosition(glm::ivec2(portal2p.x - 36, portal2p.y));
				else player->setPosition(portal2p);
				player->setJump(false);
			}
			else if (!touchingPortal && distr2 <= 6 && distr2 >= -6 && (player2p.y + 36 >= portalp.y) && (player2p.y + 36 <= portalp.y + 72)) {
				touchingPortal = true;
				if (portal2->getSide()) player2->setPosition(glm::ivec2(portal2p.x - 36, portal2p.y));
				else player2->setPosition(portal2p);
				player2->setJump(false);
			}
			else touchingPortal = false;
		}
		else {	//entrar al portal 1 por su derecha
			int distl1 = playerp.x + 15 - portalp.x;
			int distl2 = player2p.x + 15 - portalp.x;
			if (!touchingPortal && distl1 <= 6 && distl1 >= -6 && (playerp.y + 36 >= portalp.y) && (playerp.y + 36 <= portalp.y + 72)) {
				touchingPortal = true;
				if (portal2->getSide()) player->setPosition(glm::ivec2(portal2p.x - 36, portal2p.y));
				else player->setPosition(portal2p);
				player->setJump(false);
			}
			else if (!touchingPortal && distl2 <= 6 && distl2 >= -6 && (player2p.y + 36 >= portalp.y) && (player2p.y + 36 <= portalp.y + 72)) {
				touchingPortal = true;
				if (portal2->getSide()) player2->setPosition(glm::ivec2(portal2p.x - 36, portal2p.y));
				else player2->setPosition(portal2p);
				player2->setJump(false);
			}
			else touchingPortal = false;
		}
	}
	if (portal2->getSide()) {	//entrar al portal 2 por su izquierda
		int distr1 = portal2p.x + 36 - (playerp.x + 57);
		int distr2 = portal2p.x + 36 - (player2p.x + 57);
		if (!touchingPortal && distr1 <= 6 && distr1 >= -6 && (playerp.y + 36 >= portal2p.y) && (playerp.y + 36 <= portal2p.y + 72)) {
			touchingPortal = true;
			if (portal->getSide()) player->setPosition(glm::ivec2(portalp.x - 36, portalp.y));
			else player->setPosition(portalp);
			player->setJump(false);
		}
		else if (!touchingPortal && distr2 <= 6 && distr2 >= -6 && (player2p.y + 36 >= portal2p.y) && (player2p.y + 36 <= portal2p.y + 72)) {
			touchingPortal = true;
			if (portal->getSide()) player2->setPosition(glm::ivec2(portalp.x - 36, portalp.y));
			else player2->setPosition(portalp);
			player2->setJump(false);
		}
		else touchingPortal = false;
	}
	else {	//entrar al portal 2 por su derecha
		int distl1 = playerp.x + 15 - portal2p.x;
		int distl2 = player2p.x + 15 - portal2p.x;
		if (!touchingPortal && distl1 <= 6 && distl1 >= -6 && (playerp.y + 36 >= portal2p.y) && (playerp.y + 36 <= portal2p.y + 72)) {
			touchingPortal = true;
			if (portal->getSide()) player->setPosition(glm::ivec2(portalp.x - 36, portalp.y));
			else player->setPosition(portalp);
			player->setJump(false);
		}
		else if (!touchingPortal && distl2 <= 6 && distl2 >= -6 && (player2p.y + 36 >= portal2p.y) && (player2p.y + 36 <= portal2p.y + 72)) {
			touchingPortal = true;
			if (portal->getSide()) player2->setPosition(glm::ivec2(portalp.x - 36, portalp.y));
			else player2->setPosition(portalp);
			player2->setJump(false);
		}
		else touchingPortal = false;
	}
}
