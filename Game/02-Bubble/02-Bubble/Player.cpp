#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define MAX_JUMP_HEIGHT 126
#define FALL_STEP 6


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, bool inv, ShaderProgram &shaderProgram)
{
	inverted = inv;
	bJumping = false;
	jumpHeight = 0;
	god = false;
	cl = false, cr = false, cu = false;
	if(inv) spritesheet.loadFromFile("images/megamaninv.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/megaman.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(72, 72), glm::vec2(1/8.f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.5f));

		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.125f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125*3.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125*4.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125*5.f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125*4.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125*3.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.125*6.f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.125*6.f, 0.5f));



		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	if (!fstjump && !Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		fstjump = true;
	}
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		cr = false;
		if ((sprite->animation() == JUMP_LEFT) || (sprite->animation() == JUMP_RIGHT))
			sprite->changeAnimation(JUMP_LEFT);
		else if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 6;
		if (cl)
		{
			if (map->collisionMoveLeft(posPlayer - glm::ivec2(58, 0), glm::ivec2(72, 72)))
			{
				posPlayer.x += 6;
				sprite->changeAnimation(STAND_LEFT);
			}
			else posPlayer.x += 3;
		}
		else 
		{
			if (map->collisionMoveLeft(posPlayer + glm::ivec2(14, 0), glm::ivec2(72, 72)))
			{
				posPlayer.x += 6;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		cl = false;
		if ((sprite->animation() == JUMP_RIGHT) || (sprite->animation() == JUMP_LEFT))
			sprite->changeAnimation(JUMP_RIGHT);
		else if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 6;
		if (cr)
		{
			if (map->collisionMoveRight(posPlayer + glm::ivec2(58, 0), glm::ivec2(72, 72)))
			{
				posPlayer.x -= 6;
				sprite->changeAnimation(STAND_RIGHT);
			}
			else posPlayer.x -= 3;
		}
		else
		{
			if (map->collisionMoveRight(posPlayer - glm::ivec2(14, 0), glm::ivec2(72, 72)))
			{
				posPlayer.x -= 6;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		if (inverted) {
			if ((sprite->animation() == MOVE_LEFT) || (sprite->animation() == STAND_LEFT))
				sprite->changeAnimation(JUMP_LEFT);
			else if ((sprite->animation() == MOVE_RIGHT) || (sprite->animation() == STAND_RIGHT))
				sprite->changeAnimation(JUMP_RIGHT);
			jumpAngle += JUMP_ANGLE_STEP;
			int colup = map->collisionMoveUpInv(posPlayer, glm::ivec2(72, 72), &posPlayer.y);
			if (jumpAngle < 90 && colup == 1) {
				jumpAngle = 180 - jumpAngle;
			}
			else if (colup == 2 && !god)
			{
				Game::instance().stopSound();
				Game::instance().playSoundEffect("sounds/08_MegamanDefeat.wav");
			}
			else if (jumpAngle == 180)
			{

				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY + MAX_JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					int coldown = map->collisionMoveDownInv(posPlayer, glm::ivec2(72, 72), &posPlayer.y);
					if (coldown == 2 && !god)
					{
						Game::instance().stopSound();
						Game::instance().playSoundEffect("sounds/08_MegamanDefeat.wav");
					}
					else bJumping = coldown == 0;
				}
			}
		}
		else {
			if ((sprite->animation() == MOVE_LEFT) || (sprite->animation() == STAND_LEFT))
				sprite->changeAnimation(JUMP_LEFT);
			else if ((sprite->animation() == MOVE_RIGHT) || (sprite->animation() == STAND_RIGHT))
				sprite->changeAnimation(JUMP_RIGHT);
			jumpAngle += JUMP_ANGLE_STEP;
			int colup = map->collisionMoveUp(posPlayer, glm::ivec2(72, 72), &posPlayer.y);
			if (jumpAngle < 90 && colup == 1) {
				jumpAngle = 180 - jumpAngle;

			}
			else if (colup == 2 && !god)
			{
				Game::instance().stopSound();
				Game::instance().playSoundEffect("sounds/08_MegamanDefeat.wav");
			}
			else if (jumpAngle == 180)
			{

				bJumping = false;
				posPlayer.y = startY;
				
			}
			else
			{
				posPlayer.y = int(startY - MAX_JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					int coldown = map->collisionMoveDown(posPlayer, glm::ivec2(72, 72), &posPlayer.y);
					if (coldown == 2 && !god) 
					{
						Game::instance().stopSound();
						Game::instance().playSoundEffect("sounds/08_MegamanDefeat.wav");
					}
						
					else bJumping = coldown == 0;					
				}
			}
		}
		if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && jumpAngle > 30) 
			bJumping = false;
		if (jumpAngle < 5)
			Game::instance().playSoundEffect("sounds/06_MegamanLand.wav");
	}
	else
	{
		//if (!cu) {
			if (inverted) {
				if(!cu)posPlayer.y -= FALL_STEP;
				int coldown = map->collisionMoveDownInv(posPlayer, glm::ivec2(72, 72), &posPlayer.y);
				if (coldown == 1 || cu)
				{
					if (sprite->animation() == JUMP_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == JUMP_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
					jumpHeight = 0;
					if (Game::instance().getSpecialKey(GLUT_KEY_UP) && fstjump)
					{
						bJumping = true;
						fstjump = false;
						jumpAngle = 0;
						startY = posPlayer.y;
					}
				}
				else if (coldown == 2 && !god) {
					Game::instance().stopSound();
					Game::instance().playSoundEffect("sounds/08_MegamanDefeat.wav");
				}
				else {
					if (sprite->animation() == STAND_LEFT)
						sprite->changeAnimation(JUMP_LEFT);
					else if (sprite->animation() == STAND_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);
				}
			}
			else {
				if(!cu)posPlayer.y += FALL_STEP;
				int coldown = map->collisionMoveDown(posPlayer, glm::ivec2(72, 72), &posPlayer.y);
				if (coldown == 1 || cu)
				{
					if (sprite->animation() == JUMP_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == JUMP_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
					jumpHeight = 0;
					if (Game::instance().getSpecialKey(GLUT_KEY_UP) && fstjump)
					{
						bJumping = true;
						fstjump = false;
						jumpAngle = 0;
						startY = posPlayer.y;
					}
				}
				else if (coldown == 2 && !god)
				{
					Game::instance().stopSound();
					Game::instance().playSoundEffect("sounds/08_MegamanDefeat.wav");
				}
				else {
					if (sprite->animation() == STAND_LEFT)
						sprite->changeAnimation(JUMP_LEFT);
					else if (sprite->animation() == STAND_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);
				}
			}
		//}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::setContact(string s) 
{
	if (s == "right") {
		cr = true;
		cl = false;
		cu = false;
	}
	else if (s =="left"){
		cl = true;
		cr = false;
		cu = false;
	}
	else if (s == "up") {
		cl = false;
		cr = false;
		cu = true;
	}
	else {
		cl = false;
		cr = false;
		cu = false;
	}
}

void Player::setGodMode(bool b) {
	god = b;
}

