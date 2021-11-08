#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define PROPORTION 1.4f


enum SceneState {
	WON, PLAYING
};

enum PlayerState {
	NORMAL, GOD_MODE
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
	PlayerState = NORMAL;
	glm::vec2 geom[2] = { glm::vec2(-1000.f, -500.f), glm::vec2(3841.f, 1500.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	space = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	bgImage2.loadFromFile("images/space.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgImage.loadFromFile("images/fondo2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	lvl = 1;
	initLvl(1);

	//setSprites();
	camerax = WORLD_WIDTH / 2;
	cameray = WORLD_HEIGHT / 2;
	scroll = 0;
	projection = glm::ortho(float(camerax - SCREEN_WIDTH / PROPORTION - scroll), float(camerax + SCREEN_WIDTH / PROPORTION + scroll), float(cameray + SCREEN_HEIGHT / PROPORTION + scroll / PROPORTION), float(cameray - SCREEN_HEIGHT / PROPORTION - scroll / PROPORTION));
	currentTime = 0.0f;
	audio = false;
	count = 0;
	
}

void Scene::initLvl(int lvl) {
	switch (lvl) {
		case 1: 
		{
			map = TileMap::createTileMap("levels/lvl01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(5, 10);
			setPlayer2Sprites(5, 19);
			setFlagSprites(26, 11);
			setFlag2Sprites(37, 16);
			lever = NULL;
			box = NULL;
			break;

		}
		case 2:
		{	
			map = TileMap::createTileMap("levels/lvl02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(45, 8);
			setPlayer2Sprites(45, 20);
			setFlagSprites(6, 10);
			setFlag2Sprites(26, 18);
			lever = NULL;
			box = NULL;
			break;
		}
		case 3:
		{
			map = TileMap::createTileMap("levels/lvl03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(18, 9);
			setPlayer2Sprites(18, 20);
			setFlagSprites(10, 10);
			setFlag2Sprites(44, 21);
			setLeverSprites(6, 11,false);
			box = NULL;
			break;
		}
		case 4:
		{
			map = TileMap::createTileMap("levels/lvl04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(18, 9);
			setPlayer2Sprites(18, 20);
			setFlagSprites(10, 10);
			setFlag2Sprites(44, 21);
			setLeverSprites(6, 11,false);
			setBoxSprites(21, 20,true);
			break;
		}
		case 5:
		{
			map = TileMap::createTileMap("levels/lvl05.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(20, 10);
			setPlayer2Sprites(20, 17);
			setFlagSprites(42, 11);
			setFlag2Sprites(42, 16);
			setBoxSprites(12, 9, false);
			setLeverSprites(42, 3, true);
			setPressurePlateSprites(18, 12, false);
			setPortalSprites(38, 23, 1, 1);
			setPortalSprites2(5, 3, 0, 0);
			break;
		}
	}
	
}

void Scene::reset() {
	SceneState = PLAYING;
	//map = TileMap::createTileMap("levels/lvl5.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map = TileMap::createTileMap("levels/lvltest.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//setSprites();
	initLvl(lvl);
	Game::instance().stopSound();
	Game::instance().playMusic("sounds/06_Guts_Man.mp3");
	camerax = WORLD_WIDTH / 2;
	cameray = WORLD_HEIGHT / 2;
	scroll = 0;
	projection = glm::ortho(float(camerax - SCREEN_WIDTH / PROPORTION - scroll), float(camerax + SCREEN_WIDTH / PROPORTION + scroll), float(cameray + SCREEN_HEIGHT / PROPORTION + scroll / PROPORTION), float(cameray - SCREEN_HEIGHT / PROPORTION - scroll / PROPORTION));
	currentTime = 0.0f;
	leverActivated = false;
	audio = false;
	count = 0;
}

void Scene::update(int deltaTime)
{
	switch (SceneState)
	{
		case PLAYING:
		{
			currentTime += deltaTime;
			player->update(deltaTime);
			player2->update(deltaTime);
			glm::vec2 currPos1 = player->getPosition();
			glm::vec2 currPos2 = player2->getPosition();
			int displ = -(WORLD_WIDTH / 2 - currPos1.x);

			camerax = (currPos1.x + currPos2.x) / 2;
			cameray = (currPos1.y + currPos2.y) / 2;
			if (((currPos1.y + 32 > WORLD_HEIGHT / 2 - 3) && (currPos1.y + 32 < WORLD_HEIGHT / 2 + 3)))
				player->setDeath(true);
			if (((currPos2.y + 32 > WORLD_HEIGHT / 2 - 3) && (currPos2.y + 32 < WORLD_HEIGHT / 2 + 3)))
				player2->setDeath(true);
				//reset();
			bool win1 = false;
			if (abs(currPos1.x - currPos2.x) > SCREEN_WIDTH / 1.2)
				scroll = abs(abs(currPos1.x - currPos2.x) - SCREEN_WIDTH / 1.2) / 2;
			else if (abs(currPos1.y - currPos2.y) > SCREEN_HEIGHT / 1.1)
				scroll = abs(abs(currPos1.y - currPos2.y) - SCREEN_HEIGHT / 1.1) / 2;
			else scroll = 0;
			if (currPos1.x >= (flag_x - 2.5) * map->getTileSize() &&
				currPos1.x <= (flag_x + 1) * map->getTileSize() &&
				currPos1.y >= (flag_y)*map->getTileSize() &&
				currPos1.y <= (flag_y + 2) * map->getTileSize())
				win1 = true;
			else
				win1 = false;
			flag->update(deltaTime, win1);
			bool win2 = false;
			if (currPos2.x >= (flag2_x - 2.5) * map->getTileSize() &&
				currPos2.x <= (flag2_x + 1) * map->getTileSize() &&
				currPos2.y <= (flag2_y)*map->getTileSize() &&
				currPos2.y >= (flag2_y - 2) * map->getTileSize())
				win2 = true;
			else
				win2 = false;
			flag2->update(deltaTime, win2);
			if (win1 && win2) {
				Game::instance().stopSound();
				SceneState = WON;
			}
			
			if (box != NULL) 
			{
				collisions();
				box->update(deltaTime);
				int bpos = box->getPosition().y;
				if ((bpos >= (WORLD_HEIGHT / 2) - 36) && (bpos + 72 <= (WORLD_HEIGHT / 2) + 36)) box->setMid();
				
			}
			if (lever != NULL)
				leverActivate();
			if (portal != NULL && portal2 != NULL) {
				portal->update(deltaTime);
				portal2->update(deltaTime);
			}
			switch (PlayerState)
				{
					case NORMAL: 
					{
						if (Game::instance().getKey('g'))
						{
							Game::instance().keyReleased('g');
							PlayerState = GOD_MODE;
							player->setGodMode(true);
							player2->setGodMode(true);
						}
						break;
					}
					case GOD_MODE:
					{
						if (Game::instance().getKey('1'))
						{
							lvl = 1;
							reset();
						}
						if (Game::instance().getKey('2'))
						{
							lvl = 2;
							reset();
						}
						if (Game::instance().getKey('3'))
						{
							lvl = 3;
							reset();
						}
						if (Game::instance().getKey('4'))
						{
							lvl = 4;
							reset();
						}
						if (Game::instance().getKey('5'))
						{
							lvl = 5;
							reset();
						}
						if (Game::instance().getKey('r'))
						{
							Game::instance().keyReleased('r');
							if (lever != NULL)
								removeBarrier();
						}
						if (Game::instance().getKey('g'))
						{
							Game::instance().keyReleased('g');
							PlayerState = NORMAL;
							player->setGodMode(false);
							player2->setGodMode(false);
						}
						break;
					}
				}		
			break;
		}
		case WON:
		{
			currentTime += deltaTime;
			count++;
			//player->update(deltaTime);
			//player2->update(deltaTime);
			if (!audio) 
			{
				Game::instance().playSoundEffect("sounds/10_Victory!.mp3");
				audio = true;
			}
			if (count >= 420) {
				if (lvl == 5)
					Game::instance().changeState('C');
				else
				{
					lvl++;
					initLvl(lvl);
					SceneState = PLAYING;
					Game::instance().playMusic("sounds/06_Guts_Man.mp3");
					audio = false;
					count = 0;
				}	
			}
			break;
		}
	}
	if (Game::instance().getKey('q')) 
	{
		lvl = 1;
		Game::instance().changeState('M');
	}
	projection = glm::ortho(float(camerax - SCREEN_WIDTH / PROPORTION - scroll), float(camerax + SCREEN_WIDTH / PROPORTION + scroll), float(cameray + SCREEN_HEIGHT / PROPORTION + scroll / PROPORTION), float(cameray - SCREEN_HEIGHT / PROPORTION - scroll / PROPORTION));
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
	space->render(bgImage2);
	background->render(bgImage);
	map->render();
	player->render();
	player2->render();
	flag->render();
	flag2->render();
	if (lever != NULL)
		lever->render();
	if (box != NULL)
		box->render();
	if (lvl == 5)
	{
		if (pplateActivated) {
			portal->render();
			portal2->render();
		}
		pplate->render();
	}
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


void Scene::setPlayerSprites(int x, int y)
{
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	player->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	player->setGodMode(PlayerState == GOD_MODE);
	player->setTileMap(map);
}

void Scene::setPlayer2Sprites(int x, int y)
{
	player2 = new Player();
	player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, texProgram);
	player2->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	player2->setGodMode(PlayerState == GOD_MODE);
	player2->setTileMap(map);
}
	
void Scene::setFlagSprites(int x, int y)
{
	flag = new Flag();
	flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, false);
	flag->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	flag->setTileMap(map);
	flag_x = x;
	flag_y = y;
}
	

void Scene::setFlag2Sprites(int x, int y)
{
	flag2 = new Flag();
	flag2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, true);
	flag2->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	flag2->setTileMap(map);
	flag2_x = x;
	flag2_y = y;
}

void Scene::setPressurePlateSprites(int x, int y, bool inv) 
{
	pplate = new PressurePlate();
	pplate->init(glm::ivec2(SCREEN_X, SCREEN_Y), inv, texProgram);
	pplate->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	pplate->setTileMap(map);
}
	

void Scene::setLeverSprites(int x, int y, bool inv)
{
	lever = new Lever();
	lever->init(glm::ivec2(SCREEN_X, SCREEN_Y), inv, texProgram);
	lever->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	lever->setTileMap(map);
}

	
void Scene::setBoxSprites(int x, int y, bool inv)
{
	box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), inv, texProgram);
	box->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	box->setTileMap(map);
}

void Scene::setPortalSprites(int x, int y, bool clr, bool sd) {
	portal = new Portal();
	portal->init(glm::ivec2(SCREEN_X, SCREEN_Y), clr, sd, texProgram);
	portal->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	portal->setTileMap(map);
}

void Scene::setPortalSprites2(int x, int y, bool clr, bool sd) {
	portal2 = new Portal();
	portal2->init(glm::ivec2(SCREEN_X, SCREEN_Y), clr, sd, texProgram);
	portal2->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	portal2->setTileMap(map);
}

void Scene::setMap() {
	player->setTileMap(map);
	player2->setTileMap(map);
	flag->setTileMap(map);
	flag2->setTileMap(map);
	if (lever != NULL)
		lever->setTileMap(map);
	if (box != NULL)
		box->setTileMap(map);
}

void Scene::collisions() {
	glm::ivec2 playerp = { 0,0 };
	glm::ivec2 player2p = { 0,0 };
	glm::ivec2 boxp = { 0,0 };
	glm::ivec2 portalp = { 0,0 };
	glm::ivec2 portal2p = { 0,0 };
	glm::ivec2 pplatep = { 0,0 };
	glm::ivec2 leverp = { 0,0 };
	if (lvl == 5) 
	{
		portalp = portal->getPosition();
		portal2p = portal2->getPosition();
		pplatep = pplate->getPosition();
		
	}
	playerp = player->getPosition();
	player2p = player2->getPosition();
	boxp = box->getPosition();
	
	if(lever!=NULL) leverp = lever->getPosition();

	if (pplate != NULL)
	{
		if (((abs(playerp.x - pplatep.x) <= 36) && (abs(playerp.y + 36 - pplatep.y) <= 36)) || ((abs(boxp.x - pplatep.x) <= 36) && (abs(boxp.y + 36 - pplatep.y) <= 36)))
		{
			pplate->setState(true);
			pplateActivated = true;
		}
		else {
			pplate->setState(false);
			pplateActivated = false;
		}
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
	if (pplateActivated)
	{
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
		if (portal2 != NULL)
		{
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
			else if (portal2 != NULL) {	//entrar al portal 2 por su derecha
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
	}
}

void Scene::leverActivate()
{
	if (!leverActivated && (abs(player->getPosition().x + 15 - lever->getPosition().x) <= 36) && (abs(player->getPosition().y + 36 - lever->getPosition().y) <= 36))
		removeBarrier();
	else if(!leverActivated && (abs(player2->getPosition().x + 15 - lever->getPosition().x) <= 36) && (abs(player2->getPosition().y + 36 - lever->getPosition().y) <= 36))
		removeBarrier();
}

void Scene::removeBarrier()
{
	lever->activate();
	leverActivated = true;
	if (lvl == 3)
		map = TileMap::createTileMap("levels/lvl03nb.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	else if (lvl == 5)
		map = TileMap::createTileMap("levels/lvl05nb.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	setMap();
}
