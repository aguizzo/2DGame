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
	PlayerState = NORMAL;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1800.f, 1000.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	bgImage.loadFromFile("images/mega_bg2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	lvl = 1;
	initLvl(1);

	//setSprites();
	camera = WORLD_WIDTH / 2;
	scroll = 0;
	projection = glm::ortho(float(camera - SCREEN_WIDTH/PROPORTION - scroll), float(camera + SCREEN_WIDTH/PROPORTION + scroll), float(WORLD_HEIGHT/2 + SCREEN_HEIGHT/PROPORTION + scroll*PROPORTION), float(WORLD_HEIGHT/2 - SCREEN_HEIGHT/PROPORTION - scroll*PROPORTION));
	currentTime = 0.0f;
	leverActivated = false;
	audio = false;
	count = 0;
	
}

void Scene::initLvl(int lvl) {
	switch (lvl) {
		case 1: 
		{
			map = TileMap::createTileMap("levels/lvl01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(5, 8);
			setPlayer2Sprites(5, 20);
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
			setFlagSprites(6, 14);
			setFlag2Sprites(26, 18);
			lever = NULL;
			box = NULL;
			break;
		}
		case 3:
		{
			map = TileMap::createTileMap("levels/lvl5.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(18, 9);
			setPlayer2Sprites(18, 20);
			setFlagSprites(10, 10);
			setFlag2Sprites(44, 21);
			setLeverSprites(6, 11);
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
			setLeverSprites(6, 11);
			setBoxSprites(21, 20);
			break;
		}
		case 5:
		{
			map = TileMap::createTileMap("levels/lvltest.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			setPlayerSprites(18, 9);
			setPlayer2Sprites(18, 20);
			setFlagSprites(14, 10);
			setFlag2Sprites(23, 17);
			setLeverSprites(28, 11);
			setBoxSprites(0, 0);
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
	camera = WORLD_WIDTH / 2;
	scroll = 0;
	projection = glm::ortho(float(camera - SCREEN_WIDTH / PROPORTION - scroll), float(camera + SCREEN_WIDTH / PROPORTION + scroll), float(WORLD_HEIGHT / 2 + SCREEN_HEIGHT / PROPORTION + scroll * PROPORTION), float(WORLD_HEIGHT / 2 - SCREEN_HEIGHT / PROPORTION - scroll * PROPORTION));
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
			camera = (currPos1.x + currPos2.x) / 2;
			if ((currPos1.y > WORLD_HEIGHT / 2) || (currPos2.y < WORLD_HEIGHT / 2)) reset();
			bool win1 = false;
			if (abs(currPos1.x - currPos2.x) > SCREEN_WIDTH / 1.2)
				scroll = abs(abs(currPos1.x - currPos2.x) - SCREEN_WIDTH / 1.2) / 2;
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
				if ((bpos >= (WORLD_HEIGHT / 2) - 42) && (bpos <= (WORLD_HEIGHT / 2) + 42)) box->setMid();
			}
			if (lever != NULL)
				leverActivate();
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
	if (lever != NULL)
		lever->render();
	if (box != NULL)
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
	

void Scene::setLeverSprites(int x, int y)
{
	lever = new Lever();
	lever->init(glm::ivec2(SCREEN_X, SCREEN_Y), false, texProgram);
	lever->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	lever->setTileMap(map);
}
	
void Scene::setBoxSprites(int x, int y)
{
	box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), true, texProgram);
	box->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	box->setTileMap(map);
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
			player->setPosition(glm::vec2(player->getPosition().x, box->getPosition().y - 72));
			player->setContact("up");
		}
		else if ((cui <= 6 && cui >= 0) && (player2->getPosition().x + 72 - 15 >= box->getPosition().x) && (player2->getPosition().x + 15 <= box->getPosition().x + 72)) {
			player2->setPosition(glm::vec2(player->getPosition().x, box->getPosition().y - 72));
			player2->setContact("up");
		}
		else
		{
			box->setContact(false);
			player->setContact("");
		}
}

void Scene::leverActivate()
{
	if (!leverActivated && (abs(player->getPosition().x + 15 - lever->getPosition().x) <= 36) && (abs(player->getPosition().y + 36 - lever->getPosition().y) <= 36))
		removeBarrier();
}

void Scene::removeBarrier()
{
	lever->activate();
	leverActivated = true;
	if (lvl == 3)
		map = TileMap::createTileMap("levels/lvl5nb.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	setMap();
}
