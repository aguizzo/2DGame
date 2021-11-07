#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "HowToPlay.h"
#include "Credits.h"
#include <irrKlang.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

#define WORLD_WIDTH 1800
#define WORLD_HEIGHT 1044


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void changeState(char c);
	void playSoundEffect(string effect);
	void playMusic(string track);
	void stopSound();
	void resetLvl();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void setSpecialKey(int key);

private:
	int State;
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	Menu menu;
	HowToPlay howTo;
	Credits credits;
	irrklang::ISoundEngine* engine;
};


#endif // _GAME_INCLUDE


