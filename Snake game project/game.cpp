// game.cpp
#include "game.h"
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

SnakeGame::SnakeGame()
: window(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Snake Game"),
  scoreText(font, "", 24U),
  gameOverText(font, "", 48U),
  highScoreText(font, "", 24U),
  timer(0.f),
  delay(0.1f)
{
    if (!font.openFromFile("arial.ttf") &&
        !font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    scoreText.setFillColor(sf::Color::White);
    gameOverText.setFillColor(sf::Color::Red);
    highScoreText.setFillColor(sf::Color::White);

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    loadHighScores();
    resetGame();
}

void SnakeGame::run() {
    while (window.isOpen()) {
        processEvents();
        float elapsed = clock.restart().asSeconds();
        timer += elapsed;
        update();
        render();

    }
}

void SnakeGame::processEvents() {
    while (auto evt = window.pollEvent()) {
        const auto &event = *evt;
        if (event.is<sf::Event::Closed>()) window.close();
        else if (auto key = event.getIf<sf::Event::KeyPressed>()) {
            switch (key->scancode) {
                case sf::Keyboard::Scancode::Up:    if (!gameOver && dir != Down)  dir = Up;    break;
                case sf::Keyboard::Scancode::Right: if (!gameOver && dir != Left)  dir = Right; break;
                case sf::Keyboard::Scancode::Down:  if (!gameOver && dir != Up)    dir = Down;  break;
                case sf::Keyboard::Scancode::Left:  if (!gameOver && dir != Right) dir = Left;  break;
                case sf::Keyboard::Scancode::R:     if (gameOver) resetGame();       break;
                default: break;
            }
        }
    }
}

void SnakeGame::update() {
    if (gameOver || timer <= delay) return;
    timer = 0.f;
    Vec2 head = snake.front();
    switch (dir) {
        case Up:    head.y--; break;
        case Right: head.x++; break;
        case Down:  head.y++; break;
        case Left:  head.x--; break;
    }
    if (head.x < 0) head.x = gridWidth - 1;
    if (head.x >= gridWidth) head.x = 0;
    if (head.y < 0) head.y = gridHeight - 1;
    if (head.y >= gridHeight) head.y = 0;

    if (checkCollision(head)) {
        gameOver = true;
        gameOverText.setString(
            "Game Over!\nScore final: " + std::to_string(score) +
            "\nrestart -->tapez R"
        );
        highScores.push_back(score);
        std::sort(highScores.rbegin(), highScores.rend());
        if (highScores.size() > 5) highScores.resize(5);
        saveHighScores();
    } else {
        snake.insert(snake.begin(), head);
        if (head == fruit) {
            score++;
            placeFruit();
            placeObstacles();
        } else {
            snake.pop_back();
        }
    }
}

void SnakeGame::render() {
    window.clear(sf::Color(158, 173, 95));
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1.f, cellSize - 1.f));

    // Draw fruit
    cell.setFillColor(sf::Color::Red);
    cell.setPosition(sf::Vector2f(fruit.x * cellSize, fruit.y * cellSize));
    window.draw(cell);

    // Draw obstacles
    cell.setFillColor(sf::Color(128,128,128));
    for (auto &obs : obstacles) {
        cell.setPosition(sf::Vector2f(obs.x * cellSize, obs.y * cellSize));
        window.draw(cell);
    }

    // Draw snake
    cell.setFillColor(sf::Color::Blue);
    for (auto &pt : snake) {
        cell.setPosition(sf::Vector2f(pt.x * cellSize, pt.y * cellSize));
        window.draw(cell);
    }

    // Draw texts
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(sf::Vector2f(5.f, 5.f));
    window.draw(scoreText);

    std::string hs = "Meilleurs scores:\n";
    for (size_t i = 0; i < highScores.size(); ++i)
        hs += std::to_string(i+1) + ". " + std::to_string(highScores[i]) + "\n";
    highScoreText.setString(hs);
    highScoreText.setPosition(sf::Vector2f(windowWidth - 200.f, 5.f));
    window.draw(highScoreText);

    if (gameOver) {
        gameOverText.setPosition(sf::Vector2f(windowWidth/4.f, windowHeight/2.f - 50.f));
        window.draw(gameOverText);
    }

    window.display();
}

void SnakeGame::loadHighScores() {
    highScores.clear();
    std::ifstream file("scores.txt");
    int s; while (file >> s) highScores.push_back(s);
    std::sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > 5) highScores.resize(5);
}

void SnakeGame::saveHighScores() {
    std::ofstream file("scores.txt");
    for (int s : highScores) file << s << "\n";
}

void SnakeGame::resetGame() {
    gameOver = false;
    score = 0;
    dir = Right;
    snake = {{gridWidth/2, gridHeight/2},
             {gridWidth/2 - 1, gridHeight/2},
             {gridWidth/2 - 2, gridHeight/2}};
    placeFruit();
    obstacles.clear();
    placeObstacles();
    timer = 0.f;
}

bool SnakeGame::checkCollision(const Vec2 &pos) {
    for (size_t i = 1; i < snake.size(); ++i)
        if (snake[i] == pos) return true;
    for (auto &obs : obstacles)
        if (obs == pos) return true;
    return false;
}

void SnakeGame::placeFruit() {
    do {
        fruit.x = std::rand() % gridWidth;
        fruit.y = std::rand() % gridHeight;
    } while (checkCollision(fruit));
}

void SnakeGame::placeObstacles() {
    obstacles.clear();
    for (int i = 0; i < 2; ++i) {
        Vec2 pos;
        do {
            pos.x = std::rand() % gridWidth;
            pos.y = std::rand() % gridHeight;
        } while (checkCollision(pos) || pos == fruit);
        obstacles.push_back(pos);
    }
}
