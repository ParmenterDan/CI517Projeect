#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), gameWon(false), box(325, 400, 150, 20), paddleBox(350, 200, 10, 10) {
	TTF_Font* font = ResourceManager::loadFont("res/fonts/tahoma.ttf", 24);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	// Setup Game
	setupGame();
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	int speed = 3;

	if (eventSystem->isPressed(Key::A)) {
		velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::D)) {
		velocity.x = speed;
	}

	if (eventSystem->isPressed(Key::E)) {
		if (gameWon == true || gameLost == true) {
			setupGame();
		}
	}
}

void MyGame::update() {
	// Store variable to handle level up
	lastScore = score;

	// Handle Player Paddle
	handlePlayer();

	// Handle Ball Physics
	HandleBall();

	checkScore();

	if (checkGameLost() == true) {
		gameLost = true;
	}

	if (gameWon == true || gameLost == true) {
		gameOver();
	}
}

void MyGame::render() {
	if (gameWon == false && gameLost == false) {
		// Draw background
		gfx->drawTexture(Background, backgroundTexture, SDL_FLIP_NONE);

		gfx->setDrawColor(SDL_COLOR_RED);

		// Player
		gfx->drawTexture(Paddle, boxTexture, SDL_FLIP_NONE);

		// Draw Ball(s)
		gfx->setDrawColor(SDL_COLOR_GRAY);


		for (int i = 0; i < 10; i++) {
			if (balls[i].isActive == true) {
				gfx->drawTexture(ballTexture, ballPosition[i], SDL_FLIP_NONE);
			}
		}

		gfx->setDrawColor(SDL_COLOR_GRAY);

		// Draw Bricks
		gfx->setDrawColor(SDL_COLOR_BLUE);
		for (int i = 0; i < sizeof(bricks) / sizeof(bricks[0]); i++) {
			if (bricks[i].isActive == true) {
				int x = bricks[i].x;
				int y = bricks[i].y;
				int w = bricks[i].w;
				int h = bricks[i].h;

				gfx->fillRect(x, y, w, h);
				gfx->drawTexture(brickTexture, bricks[i].texturePosition, SDL_FLIP_NONE);
			}
		}
	} else if (gameWon == true) {
		gfx->drawTexture(gameOverWinTexture, gameOverWinPosition, SDL_FLIP_NONE);
	}
	else if (gameLost == true) {
		gfx->drawTexture(gameOverLostTexture, gameOverWinPosition, SDL_FLIP_NONE);
	}
}

void MyGame::renderUI() {
	/*gfx->drawTexture(Background, backgroundTexture, SDL_FLIP_NONE);*/

	if (gameWon == false && gameLost == false) {
		gfx->setDrawColor(SDL_COLOR_BLACK);
		std::string scoreStr = "Score: " + std::to_string(score);
		gfx->drawText(scoreStr, 55, 10);

		gfx->drawText("Level: " + std::to_string(level), 615, 10);

		// Easter Egg
		if (easterEggActive == true) {
			gfx->drawText(easterEgg, 240, 9);
		}
	}
}

void MyGame::HandleBall() {
	// Handle ball movement and collision
	for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++) {
		if (balls[i].isActive == true) {
			// Save last position & speed
			balls[i].lastX = balls[i].x;
			balls[i].lastY = balls[i].y;

			// Save last speed for each ball
			for (int c = 0; c < sizeof(balls) / sizeof(balls[0]); c++) {
				balls[c].lastSpeedX = balls[c].speedX;
				balls[c].lastSpeedY = balls[c].speedY;
			}

			// Handle Movement
			balls[i].x += balls[i].speedX;
			balls[i].y += balls[i].speedY;

			ballPosition[i]->x = balls[i].x;
			ballPosition[i]->y = balls[i].y;

			// Balls collide with eachother
			if (i < sizeof(balls) / sizeof(balls[0])) {
				for (int c = i + 1; c < sizeof(balls) / sizeof(balls[0]) - 1; c++) {
					if (balls[c].isActive == true) {
						if (mySystem->boxCollisionPhysics(balls[i], balls[c]) == true) {
							sfx->playSound(ballCollision);
							if (mySystem->boxCollisionSide(balls[i], balls[c]) == "Top") {
								balls[i].side = "top";
								balls[i].speedX = mySystem->calcVelocityX(balls[i], balls[c]);
								balls[c].speedX = mySystem->calcVelocityX(balls[c], balls[i]);
								balls[i].speedY = mySystem->calcVelocityY(balls[i], balls[c]);
								balls[c].speedY = mySystem->calcVelocityY(balls[c], balls[i]);
							}
							else if (mySystem->boxCollisionSide(balls[i], balls[c]) == "Bottom") {
								balls[i].speedX = mySystem->calcVelocityX(balls[i], balls[c]);
								balls[c].speedX = mySystem->calcVelocityX(balls[c], balls[i]);
								balls[i].speedY = mySystem->calcVelocityY(balls[i], balls[c]);
								balls[c].speedY = mySystem->calcVelocityY(balls[c], balls[i]);
							}
							else if (mySystem->boxCollisionSide(balls[i], balls[c]) == "Left") {
								balls[i].speedX = mySystem->calcVelocityX(balls[i], balls[c]);
								balls[c].speedX = mySystem->calcVelocityX(balls[c], balls[i]);
								balls[i].speedY = mySystem->calcVelocityY(balls[i], balls[c]);
								balls[c].speedY = mySystem->calcVelocityY(balls[c], balls[i]);
							}
							else if (mySystem->boxCollisionSide(balls[i], balls[c]) == "Right") {
								balls[i].speedX = mySystem->calcVelocityX(balls[i], balls[c]);
								balls[c].speedX = mySystem->calcVelocityX(balls[c], balls[i]);
								balls[i].speedY = mySystem->calcVelocityY(balls[i], balls[c]);
								balls[c].speedY = mySystem->calcVelocityY(balls[c], balls[i]);
							}
						}
					}
				}
			}

			// Ball collides with screen border
			if (mySystem->borderCollideX(balls[i], DEFAULT_WINDOW_WIDTH, 10, 10) == "Left") {
				balls[i].x = 10;
				balls[i].speedX *= -1;
				sfx->playSound(ballCollision);
			}
			else if (mySystem->borderCollideX(balls[i], DEFAULT_WINDOW_WIDTH , 10, 10) == "Right") {
				balls[i].x = DEFAULT_WINDOW_WIDTH - 10 - balls[i].w;
				balls[i].speedX *= -1;
				sfx->playSound(ballCollision);
			}
			else if (mySystem->borderCollideY(balls[i], DEFAULT_WINDOW_HEIGHT, 50, 10) == "Top") {
				balls[i].y = 51;
				balls[i].speedY *= -1;
				sfx->playSound(ballCollision);
			}
			else if (mySystem->borderCollideY(balls[i], DEFAULT_WINDOW_HEIGHT, 50, 10) == "Bottom") {
				balls[i].y = 51;
			}

			// Ball collides with Paddle
			if (mySystem->boxCollisionPhysics(balls[i], playerController) == true) {
				sfx->playSound(paddleCollision);
				if (mySystem->boxCollisionSide(balls[i], playerController) == "Top") {
					balls[i].speedY *= -1.05;
					balls[i].speedX = rand() % 10 - 5;
					score += 200 * balls[0].multiplier;
					balls[i].multiplier += 0.05;
				}
				else if (mySystem->boxCollisionSide(balls[i], playerController) == "Right") {
					balls[i].speedX *= -1;
					balls[i].x += 5;
				}
				else if (mySystem->boxCollisionSide(balls[i], playerController) == "Left") {
					balls[i].x -= 5;
					balls[i].speedX *= -1;
				}

				if (easterEggActive == false) {
					easterEggActive = true;
				}
				else if (easterEggActive == true) {
					easterEggActive = false;
				}
			}
			

			// Ball collides with Brick
			for (int ii = 0; ii < sizeof(bricks) / sizeof(bricks[0]); ii++) {
				if (bricks[ii].isActive == true) {
					if (balls[i].x >= bricks[ii].x && balls[i].x <= bricks[ii].x + bricks[ii].w && balls[i].y + balls[i].h >= bricks[ii].y && balls[i].y <= bricks[ii].y + bricks[ii].h) {
						sfx->playSound(boxCollision);
						bricks[ii].isActive = mySystem->disableObject();
						balls[i].x = 50 * i + 1;
						balls[i].y = 51;
						balls[i].speedX = rand() % 10 - 5;
						if (balls[i].speedX == 0) {
							balls[1].speedX = 1;
						}
						balls[i].speedY = 1;
					}
				}
			}
		
		}
	}
}

bool MyGame::checkGameLost() {
	int active = sizeof(bricks) / sizeof(bricks[0]);
	for (int i = 0; i < sizeof(bricks) / sizeof(bricks[0]); i++) {
		if (bricks[i].isActive == false) {
			active--;
		}
	}
	if (active == 0) {
		return true;
	}
	else {
		return false;
	}
}

void MyGame::setupGame() {
	score = 0;
	level = 1;

	gameWon = false;
	gameLost = false;

	// Setup Paddle
	playerController.x = 325;
	playerController.y = 400;
	playerController.w = 100;
	playerController.h = 15;
	playerController.setImmovable(true);

	boxTexture = new SDL_Rect{ (int)playerController.x, (int)playerController.y, (int)playerController.w, (int)playerController.h };
	backgroundTexture = new SDL_Rect{ 0, 0, 800, 600 };
	gameOverWinPosition = new SDL_Rect{ 0, 0, 800, 600 };
	for (int i = 0; i < 10; i++) {
		ballPosition[i] = new SDL_Rect{ 100,100,10,10 };
	}


	// Textures
	Paddle = ResourceManager::loadTexture("res/Images/paddle.png", { 0,0,0 });
	Background = ResourceManager::loadTexture("res/Images/backgroundFinal2.png", { 0,0,0 });
	brickTexture = ResourceManager::loadTexture("res/Images/brickTexture.png", { 0,0,0 });
	ballTexture = ResourceManager::loadTexture("res/Images/ballTexture.jpg", { 0,0,0 });
	gameOverWinTexture = ResourceManager::loadTexture("res/Images/gameOverWin.png", { 0,0,0 });
	gameOverLostTexture = ResourceManager::loadTexture("res/Images/gameOverLose.png", { 0,0,0 });

	// Audio
	ballCollision = ResourceManager::loadSound("res/Audio/ballsCollide.wav");
	boxCollision = ResourceManager::loadSound("res/Audio/boxCollide.wav");
	paddleCollision = ResourceManager::loadSound("res/Audio/paddleCollide.wav");
	
	// Setup Bricks
	for (int i = 0; i < sizeof(bricks) / sizeof(bricks[0]); i++) {
		if (i < 7) {
			bricks[i].y = 450;
			bricks[i].x = 14 + 112 * i;
		}
		else if (i < 14) {
			bricks[i].y = 480;
			bricks[i].x = 14 + 112 * (i - 7);
		}
		else if (i < 21) {
			bricks[i].y = 510;
			bricks[i].x = 14 + 112 * (i - 14);
		}
		else if (i < 28) {
			bricks[i].y = 540;
			bricks[i].x = 14 + 112 * (i - 21);
		}

		bricks[i].w = 100;
		bricks[i].h = 20;
		bricks[i].isActive = true;
		bricks[i].texture = brickTexture;

		bricks[i].texturePosition = new SDL_Rect{ (int)bricks[i].x,(int)bricks[i].y,(int)bricks[i].w,(int)bricks[i].h };
	}


	// Setup Ball Positions
	for (int i = 0; i < 10; i++) {
		balls[i].setImmovable(false);
		balls[i].w = 10;
		balls[i].h = 10;
		balls[i].speedX = rand() % 10 - 5;


		if (i == 0) {
			balls[i].x = 35;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 1.2;
		}
		else if (i == 1) {
			balls[i].x = 115;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 0.4;
		}
		else if (i == 2) {
			balls[i].x = 195;
			balls[i].y = 50;
			balls[i].isActive = true;
			balls[i].speedY = 1;
		}
		else if (i == 3) {
			balls[i].x = 275;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 0.1;
		}
		else if (i == 4) {
			balls[i].x = 355;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 0.9;
		}
		else if (i == 5) {
			balls[i].x = 435;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 1.5;
		}
		else if (i == 6) {
			balls[i].x = 515;
			balls[i].y = 50;
			balls[i].isActive = true;
			balls[i].speedY = 0.7;
		}
		else if (i == 7) {
			balls[i].x = 595;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 1.3;
		}
		else if (i == 8) {
			balls[i].x = 675;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 1.1;
		}
		else if (i == 9) {
			balls[i].x = 755;
			balls[i].y = 50;
			balls[i].isActive = false;
			balls[i].speedY = 1;
		}
	}

	// Seed Random
	srand(time(0));
}

void MyGame::handlePlayer() {
	playerController.x += velocity.x;
	playerController.y += velocity.y;

	velocity.x = 0;
	velocity.y = 0;

	boxTexture->x = playerController.x;
	boxTexture->y = playerController.y;

	if (mySystem->borderCollideX(playerController, DEFAULT_WINDOW_WIDTH, 10, 10) == "Left") {
		playerController.x = 10;
	}
	else if (mySystem->borderCollideX(playerController, DEFAULT_WINDOW_WIDTH, 10, 10) == "Right") {
		playerController.x = DEFAULT_WINDOW_WIDTH - 10 - playerController.w;
	}
}

void MyGame::checkScore() {
	if (score >= 1000 && score < 2000) {
		level = 2;
		if (lastScore < 1000) {
			levelUp();
		}
	}
	else if (score >= 2000 && score < 3000) {
		level = 3;
		if (lastScore < 2000) {
			levelUp();
		}
	}
	else if (score >= 3000 && score < 4000) {
		level = 4;
		if (lastScore < 3000) {
			levelUp();
		}
	}
	else if (score >= 4000 && score < 6000) {
		level = 5;
		if (lastScore < 4000) {
			levelUp();
		}
	}
	else if (score >= 6000) {
		if (lastScore < 6000) {
			gameWon = true;
		}
	}
}

void MyGame::levelUp() {
	if (level == 2) {
		balls[1].isActive = true;
		balls[9].isActive = true;
	}
	else if (level == 3) {
		balls[4].isActive = true;
		balls[5].isActive = true;
	}
	else if (level == 4) {
		balls[3].isActive = true;
		balls[7].isActive = true;
	}
	else if (level == 5) {
		balls[0].isActive = true;
		balls[8].isActive = true;
	}
}

void MyGame::gameOver() {
	for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++) {
		balls[i].isActive = false;
	}
}