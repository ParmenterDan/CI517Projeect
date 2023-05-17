#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	Point2 pos;
	bool isAlive;
};

class Bricks {
public:
	int example = 50;
	int x, y, w, h;
	bool isActive;
	SDL_Rect* texturePosition;
	SDL_Texture* texture;
};

class Paddle {
public:
	Vector2f position;
	Vector2f size;
	int x, y, w, h;
	float speed = 1.5;
};


class MyGame : public AbstractGame {
private:
	// Testing

	Vector2i velocity;

	std::vector<std::shared_ptr<GameKey>> gameKeys;

	/* GAMEPLAY */
	int score, level = 1, lastScore;
	bool gameWon, gameLost;

	// Easter Egg
	std::string easterEgg = "Daniel_Parmenter-CI517-2022";
	bool easterEggActive = false;

	void checkScore();
	void levelUp();

	void handleKeyEvents();
	void update();
	void render();
	void renderUI();

public:
	Rect box;
	Rect paddleBox;
	SDL_Rect* boxTexture;
	SDL_Rect* backgroundTexture;
	SDL_Rect* gameOverWinPosition;
	Paddle playerPaddle;

	Bricks bricks[28];

	// Physics Objects
	PhysicsObjects balls[10];
	PhysicsObjects playerController;

	// Textures
	SDL_Texture* Paddle;
	SDL_Texture* Background;
	SDL_Texture* brickTexture;
	SDL_Texture* gameOverWinTexture;
	SDL_Texture* gameOverLostTexture;

	// Temp
	SDL_Texture* ballTexture;
	SDL_Rect* ballPosition[10];

	//Audio
	Mix_Chunk* ballCollision;
	Mix_Chunk* paddleCollision;
	Mix_Chunk* boxCollision;

	// Handle game start
	bool gameStarted = false;

	// Functions
	void HandleBall();
	bool checkGameLost();
	void setupGame();
	void startScreen();
	void restart();
	void gameOver();
	void handlePlayer();

	MyGame();
	~MyGame();
};

#endif