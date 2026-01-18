#include "SnakeGame.h"

SnakeGame::SnakeGame(Screen& screen, Keys& keys) 
    : screen(screen), keys(keys), 
      snakeLength(3), direction(1), score(0), 
      gameOver(false), paused(false),
      highScore(0), bestLength(3), totalGames(0) {
    
    // 初始化蛇的位置
    snake[0] = {12, 25};  // 蛇头
    snake[1] = {11, 25};
    snake[2] = {10, 25};  // 蛇尾
}

SnakeGame::~SnakeGame() {
    prefs.end();
}

void SnakeGame::run() {
    Serial.println("2.1 贪吃蛇");
    
    // 加载历史记录并显示欢迎界面
    loadRecords();
    showWelcome();
    
    // 等待开始或退出
    bool gameStarted = false;
    while (!gameStarted) {
        keys.kvs_update();
        if (keys.o.pressed()) {
            gameStarted = true;
        }
        if (keys.x.pressed()) {
            screen.spr.fillSprite(TFT_BLACK);
            screen.spr.setTextDatum(TC_DATUM);
            return;
        }
        delay(10);
    }
    
    // 初始化游戏
    initGame();
    
    // 游戏主循环
    uint32_t lastMoveTime = millis();
    const int moveDelay = 150;
    
    while (!gameOver) {
        handleInput();
        
        // 退出游戏
        if (keys.x.pressed()) {
            screen.spr.fillSprite(TFT_BLACK);
            break;
        }
        
        // 如果暂停，跳过游戏逻辑
        if (paused) {
            delay(10);
            continue;
        }
        
        // 按固定时间间隔更新游戏
        if (millis() - lastMoveTime >= moveDelay) {
            lastMoveTime = millis();
            updateGame();
            drawGame();
        }
        
        delay(10);
    }
    
    // 游戏结束处理
    if (gameOver) {
        totalGames++;
        bool newRecord = false;
        
        // 更新最高分
        if (score > highScore) {
            highScore = score;
            prefs.putInt("highScore", highScore);
            newRecord = true;
            Serial.println("🎉 新纪录！最高分: " + String(highScore));
        }
        
        // 更新最长蛇身
        if (snakeLength > bestLength) {
            bestLength = snakeLength;
            prefs.putInt("bestLength", bestLength);
            newRecord = true;
            Serial.println("🎉 新纪录！最长蛇身: " + String(bestLength));
        }
        
        // 更新总游戏次数
        prefs.putInt("totalGames", totalGames);
        
        Serial.println("=== 保存游戏记录 ===");
        Serial.println("当前分数: " + String(score));
        Serial.println("当前长度: " + String(snakeLength));
        Serial.println("最高分: " + String(highScore));
        Serial.println("最长蛇身: " + String(bestLength));
        Serial.println("总游戏次数: " + String(totalGames));
        
        // 游戏结束画面
        screen.spr.fillRect(30, 180, 180, 180, TFT_BLACK);
        screen.spr.drawRect(30, 180, 180, 180, TFT_RED);
        screen.spr.fillRect(32, 182, 176, 176, TFT_NAVY);
        
        screen.spr.setTextColor(TFT_RED);
        screen.spr.setTextDatum(MC_DATUM);
        screen.spr.drawString("GAME OVER!", 120, 205, 4);
        
        if (newRecord) {
            screen.spr.setTextColor(TFT_GOLD);
            screen.spr.drawString("*** NEW RECORD! ***", 120, 235, 2);
        }
        
        screen.spr.setTextColor(TFT_WHITE);
        screen.spr.drawString("Score: " + String(score), 120, 260, 2);
        screen.spr.drawString("Length: " + String(snakeLength), 120, 280, 2);
        
        screen.spr.setTextColor(TFT_CYAN);
        screen.spr.drawString("High Score: " + String(highScore), 120, 305, 2);
        screen.spr.drawString("Best Length: " + String(bestLength), 120, 325, 2);
        
        screen.spr.setTextColor(TFT_YELLOW);
        screen.spr.drawString("Press X to exit", 120, 345, 2);
        
        lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
        
        // 等待退出
        while (!keys.x.pressed()) {
            keys.kvs_update();
            delay(10);
        }
        screen.spr.fillSprite(TFT_BLACK);
    }
    
    // 恢复文本对齐方式
    screen.spr.setTextDatum(TC_DATUM);
}

void SnakeGame::initGame() {
    // 重置游戏状态
    snakeLength = 3;
    direction = 1;
    score = 0;
    gameOver = false;
    paused = false;
    
    // 初始化蛇的位置
    snake[0] = {12, 25};
    snake[1] = {11, 25};
    snake[2] = {10, 25};
    
    // 生成第一个食物
    generateFood();
}

void SnakeGame::loadRecords() {
    prefs.begin("snake_game", false);
    highScore = prefs.getInt("highScore", 0);
    bestLength = prefs.getInt("bestLength", 3);
    totalGames = prefs.getInt("totalGames", 0);
    
    Serial.println("=== 读取游戏存档 ===");
    Serial.println("最高分: " + String(highScore));
    Serial.println("最长蛇身: " + String(bestLength));
    Serial.println("总游戏次数: " + String(totalGames));
}

void SnakeGame::saveRecords() {
    prefs.putInt("highScore", highScore);
    prefs.putInt("bestLength", bestLength);
    prefs.putInt("totalGames", totalGames);
}

void SnakeGame::showWelcome() {
    screen.spr.fillSprite(TFT_BLACK);
    screen.spr.setTextColor(TFT_CYAN);
    screen.spr.setTextDatum(MC_DATUM);
    screen.spr.drawString("SNAKE GAME", 120, 200, 4);
    
    screen.spr.setTextColor(TFT_YELLOW);
    screen.spr.drawString("=== Best Records ===", 120, 240, 2);
    screen.spr.setTextColor(TFT_WHITE);
    screen.spr.drawString("High Score: " + String(highScore), 120, 265, 2);
    screen.spr.drawString("Best Length: " + String(bestLength), 120, 285, 2);
    screen.spr.drawString("Total Games: " + String(totalGames), 120, 305, 2);
    
    screen.spr.setTextColor(TFT_GREEN);
    screen.spr.drawString("Press O to Start", 120, 340, 2);
    screen.spr.setTextColor(TFT_RED);
    screen.spr.drawString("Press X to Exit", 120, 365, 2);
    
    lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
}

void SnakeGame::handleInput() {
    keys.kvs_update();
    
    // 暂停控制
    if (keys.o.pressed()) {
        paused = !paused;
        if (paused) {
            screen.spr.setTextColor(TFT_YELLOW);
            screen.spr.setTextDatum(MC_DATUM);
            screen.spr.drawString("PAUSED", 120, 260, 4);
            lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
        }
    }
    
    // 方向控制（防止180度转向）
    if (keys.up.pressed() && direction != 2) direction = 0;
    if (keys.right.pressed() && direction != 3) direction = 1;
    if (keys.down.pressed() && direction != 0) direction = 2;
    if (keys.left.pressed() && direction != 1) direction = 3;
}

void SnakeGame::updateGame() {
    // 移动蛇身（从尾到头）
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    
    // 移动蛇头
    switch (direction) {
        case 0: snake[0].y--; break;  // 上
        case 1: snake[0].x++; break;  // 右
        case 2: snake[0].y++; break;  // 下
        case 3: snake[0].x--; break;  // 左
    }
    
    // 检测撞墙
    if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || 
        snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
        gameOver = true;
        return;
    }
    
    // 检测撞到自己
    for (int i = 1; i < snakeLength; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            gameOver = true;
            return;
        }
    }
    
    // 检测吃到食物
    if (snake[0].x == food.x && snake[0].y == food.y) {
        if (snakeLength < MAX_LENGTH) {
            snakeLength++;
            score += 10;
        }
        generateFood();
    }
}

void SnakeGame::drawGame() {
    screen.spr.fillSprite(TFT_BLACK);
    
    // 绘制边框
    screen.spr.drawRect(0, 0, GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, TFT_WHITE);
    
    // 绘制蛇身
    for (int i = 0; i < snakeLength; i++) {
        uint32_t color = (i == 0) ? TFT_GREENYELLOW : TFT_GREEN;
        screen.spr.fillRect(snake[i].x * CELL_SIZE + 1, 
                          snake[i].y * CELL_SIZE + 1, 
                          CELL_SIZE - 2, 
                          CELL_SIZE - 2, 
                          color);
        // 蛇头加个小圆点
        if (i == 0) {
            screen.spr.fillCircle(snake[i].x * CELL_SIZE + CELL_SIZE/2, 
                                snake[i].y * CELL_SIZE + CELL_SIZE/2, 
                                2, TFT_YELLOW);
        }
    }
    
    // 绘制食物（闪烁效果）
    uint32_t foodColor = (millis() / 200) % 2 ? TFT_RED : TFT_ORANGE;
    screen.spr.fillCircle(food.x * CELL_SIZE + CELL_SIZE/2, 
                        food.y * CELL_SIZE + CELL_SIZE/2, 
                        CELL_SIZE/2 - 1, 
                        foodColor);
    
    // 显示分数和长度
    screen.spr.setTextColor(TFT_CYAN);
    screen.spr.setTextDatum(TL_DATUM);
    screen.spr.drawString("Score:" + String(score), 5, GRID_HEIGHT * CELL_SIZE + 5, 2);
    screen.spr.drawString("Len:" + String(snakeLength), 5, GRID_HEIGHT * CELL_SIZE + 25, 2);
    
    // 显示最高分
    if (score >= highScore) {
        screen.spr.setTextColor(TFT_GOLD);
        screen.spr.drawString("NEW!", 90, GRID_HEIGHT * CELL_SIZE + 5, 2);
    } else {
        screen.spr.setTextColor(TFT_SILVER);
        screen.spr.drawString("Best:" + String(highScore), 90, GRID_HEIGHT * CELL_SIZE + 5, 2);
    }
    
    // 显示控制提示
    screen.spr.setTextColor(TFT_YELLOW);
    screen.spr.setTextDatum(TR_DATUM);
    screen.spr.drawString("O:Pause X:Quit", 235, GRID_HEIGHT * CELL_SIZE + 5, 2);
    
    // 推送到屏幕
    lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
}

void SnakeGame::generateFood() {
    bool validFood = false;
    while (!validFood) {
        food.x = random(1, GRID_WIDTH - 1);
        food.y = random(1, GRID_HEIGHT - 1);
        validFood = true;
        
        // 确保食物不在蛇身上
        for (int i = 0; i < snakeLength; i++) {
            if (food.x == snake[i].x && food.y == snake[i].y) {
                validFood = false;
                break;
            }
        }
    }
}

bool SnakeGame::checkCollision(int x, int y) {
    // 检查是否与蛇身碰撞
    for (int i = 0; i < snakeLength; i++) {
        if (x == snake[i].x && y == snake[i].y) {
            return true;
        }
    }
    return false;
}
