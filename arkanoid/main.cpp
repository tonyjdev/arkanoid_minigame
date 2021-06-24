#include <SFML/Graphics.hpp>

using namespace sf;

// Map
int blocksX = 10;
int blocksY = 10;
int offsetX = 10;
int offsetY = 50;
int mapWidth = 520;
int mapHeight = 600;

// Blocks
int blockWidth = 50;
int blockHeight = 20;
int n = 0;

// Ball
int ballX = 0;
int ballY = 0;
int ballSize = 16;
int dirX = 1;
int dirY = -1;
int changeDir = false;
int bounces = 0;
int bouncesMax = 10;
int vel = 3;
int maxVel = 8;

// Paddle
int paddleX = 250;
int paddleY = 500;
int paddleVel = 5;
int paddleWidth = 100;

// Textures & Sprites
Texture tBlock, tBall, tPaddle;
Sprite sBlock[300];

// Player
int score = 0;
int highscore = 0;

// UI
Font font;
Text textScore;
Text textHighscore;



void ResetBall()
{
	ballX = paddleX + (paddleWidth / 2);
	ballY = paddleY - ballSize;
	dirY = -1;
	dirX = 1;
	vel = 3;
	bounces = 0;

	score = 0;
}

void DrawLevel()
{
	n = 0;
	for (int i = 0; i < blocksX; i++) {
		for (int j = 0; j < blocksY; j++) {
			sBlock[n].setTexture(tBlock);
			sBlock[n].setPosition(i * blockWidth + offsetX, j * blockHeight + offsetY);
			n++;
		}
	}
}


int main()
{
	RenderWindow app(VideoMode(mapWidth, mapHeight), "Arkanoid Game!");
	app.setFramerateLimit(60);

	// Initialize font and text
	if (!font.loadFromFile("./fonts/Minercraftory.ttf")) {
		throw("NO SE HA PODIDO CARGAR LA FUENTE");
	}
	// Setup score text
	textScore.setFont(font);
	textScore.setCharacterSize(16);
	textScore.setFillColor(Color::White);
	textScore.setPosition(offsetX, mapHeight - offsetY + offsetX);
	// Setup highscore text
	textHighscore.setFont(font);
	textHighscore.setCharacterSize(16);
	textHighscore.setPosition(mapWidth - 200, mapHeight - offsetY + offsetX);


	tBlock.loadFromFile("./images/block.png");
	tBall.loadFromFile("./images/ball.png");
	tPaddle.loadFromFile("./images/paddle.png");

	Sprite sBall(tBall);
	Sprite sPaddle(tPaddle);

	DrawLevel();
	ResetBall();

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e)) {
			if (e.type == Event::Closed) {
				app.close();
			}
		}

		// Move paddle
		if (Keyboard::isKeyPressed(Keyboard::Left)) { paddleX -= paddleVel; }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { paddleX += paddleVel; }
		if (paddleX < offsetX) { paddleX = offsetX; }
		if (paddleX > (blocksX * blockWidth) + offsetX - paddleWidth) { paddleX = (blocksX * blockWidth) + offsetX - paddleWidth; }

		// Move Ball
		ballX += dirX * vel;
		changeDir = false;
		for (int i = 0; i < n; i++) {
			if (FloatRect(ballX, ballY, ballSize, ballSize).intersects(sBlock[i].getGlobalBounds())) {
				changeDir = true;
				sBlock[i].setPosition(-1000, 0);
				score += 10;
				break;
			}
		}
		if (changeDir == true) { dirX = -dirX; bounces++; }

		ballY += dirY * vel;
		changeDir = false;
		for (int i = 0; i < n; i++) {
			if (FloatRect(ballX, ballY, ballSize, ballSize).intersects(sBlock[i].getGlobalBounds())) {
				changeDir = true;
				sBlock[i].setPosition(-1000, 0);
				score += 10;
				break;
			}
		}
		if (changeDir == true) { dirY = -dirY; bounces++; }



		if ((ballX > mapWidth - offsetX - ballSize) || (ballX < offsetX)) { dirX = -dirX; bounces++; }
		if (ballY < offsetX) { dirY = -dirY; bounces++; }


		if (FloatRect(ballX, ballY, ballSize, ballSize).intersects(sPaddle.getGlobalBounds())) {
			bounces++;
			dirY = -dirY;
		}


		if (bounces > bouncesMax) {
			if (vel < maxVel) {
				vel++;
			}
			bounces -= bouncesMax;
		}

		// Check Reset ball
		if (ballY > mapHeight - offsetY - ballSize) {
			// Reset Ball
			ResetBall();
			DrawLevel();
		}

		app.clear();
		
		// Draw map
		for (int i = 0; i < n; i++) {
			app.draw(sBlock[i]);
		}

		// Draw ball
		sBall.setPosition(ballX, ballY);
		app.draw(sBall);

		// Draw Paddle
		sPaddle.setPosition(paddleX, paddleY);
		app.draw(sPaddle);

		// Show Score
		textScore.setString("Score..." + std::to_string(score));
		app.draw(textScore);

		// Show Highscore
		textHighscore.setString("Highscore..." + std::to_string(highscore));
		if (score >= highscore) {
			textHighscore.setFillColor(Color::Red);
			highscore = score;
		}
		else {
			textHighscore.setFillColor(Color::White);
		}
		app.draw(textHighscore);

		app.display();
	}

	return 0;

}