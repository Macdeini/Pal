#include <Windows.h>
#include <iostream>
#include <string>

// Dimensions of the screen
const int width = 120, height = 40;
const int displacement = 120;      // takes characters down a level. x * displacement is x levels down
const int numOfEnemies = 5;

class playerChar {

public:
    int position[3];
    wchar_t model[3] = { L'U', L'O', L'Y' };
    bool isDead = false;

    playerChar(int startingPosition) {
        position[0] = startingPosition;
        position[1] = startingPosition - displacement;
        position[2] = startingPosition - 2 * displacement;
    }

    void moveRight() {
        position[0] += 1;
        position[1] += 1;
        position[2] += 1;
    }

    void moveLeft() {
        position[0] -= 1;
        position[1] -= 1;
        position[2] -= 1;
    }
};

class badGuy {
public:
    int position[3];
    wchar_t model[3] = { L'D', L'A', L'B' };
    bool isDead = false;

    badGuy(int startingPosition) {
        position[0] = startingPosition;
        position[1] = startingPosition - displacement;
        position[2] = startingPosition - 2 * displacement;
    }

    void moveRight() {
        position[0] += 1;
        position[1] += 1;
        position[2] += 1;
    }

    void moveLeft() {
        position[0] -= 1;
        position[1] -= 1;
        position[2] -= 1;
    }

    void checkDeath(int swordPosition[5]) {
        for (int i = 0; i < 5; i++) {
            if (swordPosition[i] == position[0] || swordPosition[i] == position[1] || swordPosition[i] == position[2]) {
                isDead = true;
                position[0] = -1;
                position[1] = -2;
                position[2] = -3;
            }
        }
    }

    // True for player is to the right false for player to the left
    void trackPlayer(int playerPosition[3]) {
        if (abs(playerPosition[1] - position[1]) > 5) {
            if (position[1] - playerPosition[1] > 0) {
                moveLeft();
            }
            else if (position[1] - playerPosition[1] < 0) {
                moveRight();
            }
        }
    }
};

class swordItem {
public:
    int position[5] = {};
    std::wstring swordModel = L"SWORD";

    void setPositionRight(int positionToSet) {
        for (int i = 0; i < 5; i++) {
            position[i] = positionToSet + i + 1;
        }
    }

    void setPositionLeft(int positionToSet) {
        for (int i = 0; i < 5; i++) {
            position[4 - i] = positionToSet - i - 1;
        }
    }
};

class friendlyNPC {
public:
    int position[3];
    wchar_t model[3] = { L'L', L'A', L'P' };

    friendlyNPC(int startingPosition) {
        position[0] = startingPosition;
        position[1] = startingPosition - displacement;
        position[2] = startingPosition - 2 * displacement;
    }

    void moveRight() {
        position[0] += 1;
        position[1] += 1;
        position[2] += 1;
    }

    void moveLeft() {
        position[0] -= 1;
        position[1] -= 1;
        position[2] -= 1;
    }

    int findClosestEnemy(int enemyPositions[numOfEnemies]) {
        int closestPosition = -1;
        int closestDistance = 10000000;
        for (int i = 0; i < numOfEnemies; i++) {
            if (enemyPositions[i] >= 0 && (abs(enemyPositions[i] - position[1]) < closestDistance)) {
                closestPosition = enemyPositions[i];
                closestDistance = abs(enemyPositions[i] - position[1]);
            }
        }
        if (closestPosition == -1) {
            return position[1];
        }
        return closestPosition;
    }

    // Return True when at enemy, false when not
    bool goToEnemy(int enemyPosition) {
        if (enemyPosition == position[1]) 
            return false;
            if (enemyPosition - position[1] > 3) {
                moveRight();
                return false;
            }
            else if (enemyPosition - position[1] < -3) {
                moveLeft();
                return false;
            }
            return true;
    }

    bool stab(int enemyPosition, swordItem& sword) {
        if (enemyPosition - position[1] > 0) {
            //sword.setPositionRight(position[1]);
            return true;
        }
        else {
            //sword.setPositionLeft(position[1]);
            return false;
        }
    }

};



// Checks if position is at the end of the screen
bool endOfScreen(int position) {
    int leftEndCheck = 0;
    int rightEndCheck = width - 1;

    while (leftEndCheck < width * height || rightEndCheck < width * height) {
        if (position == rightEndCheck) {      // || position == leftEndCheck) {
            return true;
        }
        leftEndCheck += width;
        rightEndCheck += width;
    }
    return false;
}


int main() {

    // Make a text buffer
    char* screen = new char[width * height];
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    friendlyNPC pal(displacement * 17 + 60);
    swordItem sword;
    badGuy enemy(displacement * 17 + 20);
    badGuy enemy2(displacement * 17 + 70);
    badGuy enemy3(displacement * 17 + 90);
    badGuy enemy4(displacement * 17 + 50);
    badGuy enemy5(displacement * 17 + 100);
    badGuy enemies[numOfEnemies] = { enemy, enemy2, enemy3, enemy4, enemy5 };
    //int enemyPosition[numOfEnemies] = { enemy.position[1], enemy2.position[1], enemy3.position[1], enemy4.position[1] };
    int enemyPosition[numOfEnemies] = { enemies[0].position[1], enemies[1].position[1], enemies[2].position[1], enemies[3].position[1], enemies[4].position[1] };


    while (true) {

        Sleep(50);

        // Reset Screen Display
        for (int i = 0; i < width * height; i++) {
            if (displacement * 18 <= i && i <= displacement * 19 - 1) {
                screen[i] = L'=';
            }
            else {
                screen[i] = L' ';
            }

        }

        for (int i = 0; i < 3; i++) {
            screen[pal.position[i]] = pal.model[i];
        }

        for (int x = 0; x < numOfEnemies; x++) {
            for (int i = 0; i < 3; i++) {
                screen[enemies[x].position[i]] = enemies[x].model[i];
            }
        }


        if (pal.goToEnemy(pal.findClosestEnemy(enemyPosition))) {
            if (pal.stab(pal.findClosestEnemy(enemyPosition), sword)) {
                sword.setPositionRight(pal.position[1]);
            }
            else {
                sword.setPositionLeft(pal.position[1]);
            }
            for (int i = 0; i < 5; i++) {
                screen[sword.position[i]] = sword.swordModel[i];
            }
            for (int x = 0; x < numOfEnemies; x++) {
                enemies[x].checkDeath(sword.position);
            }
        }
        for (int x = 0; x < numOfEnemies; x++) {
            enemyPosition[x] = enemies[x].position[1];
        }

        
        // Draw the actual screen
        WriteConsoleOutputCharacterA(console, screen, width * height, { 0, 0 }, &bytesWritten);

    }

    return 0;
}