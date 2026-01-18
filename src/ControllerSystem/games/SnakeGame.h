#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <Arduino.h>
#include <Preferences.h>
#include "../Screen.h"
#include "../Keys.h"

class SnakeGame {
public:
    SnakeGame(Screen& screen, Keys& keys);
    ~SnakeGame();
    
    void run();  // 运行游戏主循环
    
private:
    Screen& screen;
    Keys& keys;
    Preferences prefs;
    
    // 游戏配置（适配240×536屏幕）
    static const int GRID_WIDTH = 30;      // 240px / 8px = 30格
    static const int GRID_HEIGHT = 60;     // 480px / 8px = 60格
    static const int CELL_SIZE = 8;        // 每格8像素，蛇更清晰
    static const int MAX_LENGTH = 500;
    static const int GAME_AREA_HEIGHT = 480;  // 游戏区域高度，留56px给信息栏
    
    // 游戏状态
    struct Point {
        int x;
        int y;
    };
    
    Point snake[MAX_LENGTH];
    int snakeLength;
    Point food;
    int direction;  // 0:上 1:右 2:下 3:左
    int score;
    bool gameOver;
    bool paused;
    
    // 历史记录
    int highScore;
    int bestLength;
    int totalGames;
    
    // 私有方法
    void initGame();
    void loadRecords();
    void saveRecords();
    void showWelcome();
    void updateGame();
    void drawGame();
    void generateFood();
    bool checkCollision(int x, int y);
    void handleInput();
};

#endif
