// game.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdexcept>

class SnakeGame {
public:
    SnakeGame();
    void run();

private:
    using Vec2 = sf::Vector2i;
    enum Direction { Up, Right, Down, Left };
    static constexpr int cellSize = 20;
    static constexpr int gridWidth = 40;
    static constexpr int gridHeight = 30;
    static constexpr int windowWidth = cellSize * gridWidth;
    static constexpr int windowHeight = cellSize * gridHeight;

    sf::RenderWindow window;
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text highScoreText;
    sf::Clock clock;
    float timer;
    float delay;

    std::vector<Vec2> snake;
    Vec2 fruit;
    std::vector<Vec2> obstacles;
    Direction dir;
    int score;
    bool gameOver;
    std::vector<int> highScores;

    void loadHighScores();
    void saveHighScores();
    void resetGame();
    bool checkCollision(const Vec2 &pos);
    void placeFruit();
    void placeObstacles();

    void processEvents();
    void update();
    void render();
};
