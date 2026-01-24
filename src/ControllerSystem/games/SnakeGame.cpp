#include "SnakeGame.h"

// Chinese font is already included in ControllerSystem.ino

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
    Serial.println("2.1 Snake Game");
    
    // Load records and show welcome screen
    loadRecords();
    showWelcome();
    
    // Wait for start or exit
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
    
    // Initialize game
    initGame();
    
    // Main game loop
    uint32_t lastMoveTime = millis();
    const int moveDelay = 150;
    
    while (!gameOver) {
        handleInput();
        
        // Exit game
        if (keys.x.pressed()) {
            screen.spr.fillSprite(TFT_BLACK);
            break;
        }
        
        // Skip game logic if paused
        if (paused) {
            delay(10);
            continue;
        }
        
        // Update game at fixed intervals
        if (millis() - lastMoveTime >= moveDelay) {
            lastMoveTime = millis();
            updateGame();
            drawGame();
        }
        
        delay(10);
    }
    
    // Game over handling
    if (gameOver) {
        totalGames++;
        bool newRecord = false;
        
        // Update high score
        if (score > highScore) {
            highScore = score;
            prefs.putInt("highScore", highScore);
            newRecord = true;
            Serial.println("New Record! High Score: " + String(highScore));
        }
        
        // Update best length
        if (snakeLength > bestLength) {
            bestLength = snakeLength;
            prefs.putInt("bestLength", bestLength);
            newRecord = true;
            Serial.println("New Record! Best Length: " + String(bestLength));
        }
        
        // Update total games
        prefs.putInt("totalGames", totalGames);
        
        Serial.println("=== Saving Game Records ===");
        Serial.println("Current Score: " + String(score));
        Serial.println("Current Length: " + String(snakeLength));
        Serial.println("High Score: " + String(highScore));
        Serial.println("Best Length: " + String(bestLength));
        Serial.println("Total Games: " + String(totalGames));
        
        // Game over screen
        screen.spr.fillRect(20, 150, 200, 250, TFT_BLACK);
        screen.spr.drawRect(20, 150, 200, 250, TFT_RED);
        screen.spr.fillRect(22, 152, 196, 246, TFT_NAVY);
        
        screen.spr.setTextColor(TFT_RED);
        screen.spr.setTextDatum(MC_DATUM);
        screen.spr.drawString("GAME OVER", 120, 180, 4);
        
        if (newRecord) {
            screen.spr.setTextColor(TFT_GOLD);
            screen.spr.drawString("NEW RECORD!", 120, 215, 2);
        }
        
        screen.spr.setTextColor(TFT_WHITE);
        screen.spr.drawString("Score: " + String(score), 120, 250, 2);
        screen.spr.drawString("Length: " + String(snakeLength), 120, 280, 2);
        
        screen.spr.setTextColor(TFT_CYAN);
        screen.spr.drawString("Best: " + String(highScore), 120, 320, 2);
        
        screen.spr.setTextColor(TFT_YELLOW);
        screen.spr.drawString("Press X to Exit", 120, 360, 2);
        
        lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
        
        // Wait for exit
        while (!keys.x.pressed()) {
            keys.kvs_update();
            delay(10);
        }
        screen.spr.fillSprite(TFT_BLACK);
    }
    
    // Restore text alignment
    screen.spr.setTextDatum(TC_DATUM);
}

void SnakeGame::initGame() {
    // Reset game state
    snakeLength = 3;
    direction = 1;
    score = 0;
    gameOver = false;
    paused = false;
    
    // Initialize snake position (center-top)
    snake[0] = {12, 24};
    snake[1] = {11, 24};
    snake[2] = {10, 24};
    
    // Generate first food
    generateFood();
}

void SnakeGame::loadRecords() {
    prefs.begin("snake_game", false);
    highScore = prefs.getInt("highScore", 0);
    bestLength = prefs.getInt("bestLength", 3);
    totalGames = prefs.getInt("totalGames", 0);
    
    Serial.println("=== Loading Game Records ===");
    Serial.println("High Score: " + String(highScore));
    Serial.println("Best Length: " + String(bestLength));
    Serial.println("Total Games: " + String(totalGames));
}

void SnakeGame::saveRecords() {
    prefs.putInt("highScore", highScore);
    prefs.putInt("bestLength", bestLength);
    prefs.putInt("totalGames", totalGames);
}

void SnakeGame::showWelcome() {
    screen.spr.fillSprite(TFT_BLACK);
    
    // Title
    screen.spr.setTextColor(TFT_CYAN);
    screen.spr.setTextDatum(MC_DATUM);
    screen.spr.drawString("SNAKE GAME", 120, 140, 4);
    
    // Records title
    screen.spr.setTextColor(TFT_YELLOW);
    screen.spr.drawString("- RECORDS -", 120, 190, 2);
    
    // Records data
    screen.spr.setTextColor(TFT_WHITE);
    screen.spr.setTextDatum(TL_DATUM);
    screen.spr.drawString("High Score:", 40, 240, 2);
    screen.spr.setTextColor(TFT_GOLD);
    screen.spr.setTextDatum(TR_DATUM);
    screen.spr.drawString(String(highScore), 200, 240, 4);
    
    screen.spr.setTextColor(TFT_WHITE);
    screen.spr.setTextDatum(TL_DATUM);
    screen.spr.drawString("Best Length:", 40, 285, 2);
    screen.spr.setTextColor(TFT_GREENYELLOW);
    screen.spr.setTextDatum(TR_DATUM);
    screen.spr.drawString(String(bestLength), 200, 285, 4);
    
    screen.spr.setTextColor(TFT_WHITE);
    screen.spr.setTextDatum(TL_DATUM);
    screen.spr.drawString("Total Games:", 40, 330, 2);
    screen.spr.setTextColor(TFT_CYAN);
    screen.spr.setTextDatum(TR_DATUM);
    screen.spr.drawString(String(totalGames), 200, 330, 4);
    
    // Instructions
    screen.spr.setTextColor(TFT_GREEN);
    screen.spr.setTextDatum(MC_DATUM);
    screen.spr.drawString("Press O to Start", 120, 400, 2);
    screen.spr.setTextColor(TFT_RED);
    screen.spr.drawString("Press X to Exit", 120, 440, 2);
    
    lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
}

void SnakeGame::handleInput() {
    keys.kvs_update();
    
    // Pause control
    if (keys.o.pressed()) {
        paused = !paused;
        if (paused) {
            screen.spr.setTextColor(TFT_YELLOW);
            screen.spr.setTextDatum(MC_DATUM);
            screen.spr.drawString("PAUSED", 120, 240, 4);
            lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
        }
    }
    
    // Direction control (prevent 180-degree turns)
    if (keys.up.pressed() && direction != 2) direction = 0;
    if (keys.right.pressed() && direction != 3) direction = 1;
    if (keys.down.pressed() && direction != 0) direction = 2;
    if (keys.left.pressed() && direction != 1) direction = 3;
}

void SnakeGame::updateGame() {
    // Move snake body (from tail to head)
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    
    // Move snake head
    switch (direction) {
        case 0: snake[0].y--; break;  // Up
        case 1: snake[0].x++; break;  // Right
        case 2: snake[0].y++; break;  // Down
        case 3: snake[0].x--; break;  // Left
    }
    
    // Check wall collision
    if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || 
        snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
        gameOver = true;
        return;
    }
    
    // Check self collision
    for (int i = 1; i < snakeLength; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            gameOver = true;
            return;
        }
    }
    
    // Check if food is eaten
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
    
    // Draw game area border
    screen.spr.drawRect(0, 0, 240, GAME_AREA_HEIGHT, TFT_DARKGREY);
    
    // Draw snake body
    for (int i = 0; i < snakeLength; i++) {
        uint32_t color = (i == 0) ? TFT_GREENYELLOW : TFT_GREEN;
        screen.spr.fillRect(snake[i].x * CELL_SIZE, 
                          snake[i].y * CELL_SIZE, 
                          CELL_SIZE - 1, 
                          CELL_SIZE - 1, 
                          color);
        // Add small dot to snake head
        if (i == 0) {
            screen.spr.fillCircle(snake[i].x * CELL_SIZE + CELL_SIZE/2, 
                                snake[i].y * CELL_SIZE + CELL_SIZE/2, 
                                2, TFT_YELLOW);
        }
    }
    
    // Draw food (blinking effect)
    uint32_t foodColor = (millis() / 200) % 2 ? TFT_RED : TFT_ORANGE;
    screen.spr.fillCircle(food.x * CELL_SIZE + CELL_SIZE/2, 
                        food.y * CELL_SIZE + CELL_SIZE/2, 
                        CELL_SIZE/2 - 1, 
                        foodColor);
    
    // Info bar separator line
    screen.spr.drawFastHLine(0, GAME_AREA_HEIGHT, 240, TFT_DARKGREY);
    
    // Display info without loading font
    int infoY = GAME_AREA_HEIGHT + 10;
    
    // First line: Score and Length
    screen.spr.setTextColor(TFT_CYAN);
    screen.spr.setTextDatum(TL_DATUM);
    screen.spr.drawString("Score:" + String(score), 5, infoY, 2);
    
    screen.spr.setTextColor(TFT_YELLOW);
    screen.spr.drawString("Len:" + String(snakeLength), 100, infoY, 2);
    
    // Display high score or new record
    if (score >= highScore && score > 0) {
        screen.spr.setTextColor(TFT_GOLD);
        screen.spr.setTextDatum(TR_DATUM);
        screen.spr.drawString("NEW!", 235, infoY, 2);
    } else if (highScore > 0) {
        screen.spr.setTextColor(TFT_SILVER);
        screen.spr.setTextDatum(TR_DATUM);
        screen.spr.drawString("Best:" + String(highScore), 235, infoY, 2);
    }
    
    // 推送到屏幕
    lcd_PushColors(0, 0, 240, 536, (uint16_t*)screen.spr.getPointer());
}

void SnakeGame::generateFood() {
    bool validFood = false;
    while (!validFood) {
        food.x = random(1, GRID_WIDTH - 1);
        food.y = random(1, GRID_HEIGHT - 1);
        validFood = true;
        
        // Ensure food is not on snake body
        for (int i = 0; i < snakeLength; i++) {
            if (food.x == snake[i].x && food.y == snake[i].y) {
                validFood = false;
                break;
            }
        }
    }
}

bool SnakeGame::checkCollision(int x, int y) {
    // Check collision with snake body
    for (int i = 0; i < snakeLength; i++) {
        if (x == snake[i].x && y == snake[i].y) {
            return true;
        }
    }
    return false;
}
