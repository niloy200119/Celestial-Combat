#include<stdio.h>
#include<time.h>
#include<windows.h>

int G[26][76], planeMove, bombY, bombX, bombOn = 0;
int blockOn = 0, blockY, blockX, score = 0;

// Function declarations for sound effects

void playBoomSound();

void Plane() {
    G[planeMove][3] = 1;

    G[planeMove + 1][3] = 1;
    G[planeMove + 1][4] = 1;

    G[planeMove + 2][3] = 1;
    G[planeMove + 2][4] = 1;
    G[planeMove + 2][5] = 1;

    G[planeMove + 3][3] = 1;
    G[planeMove + 3][4] = 1;

    G[planeMove + 4][3] = 1;
}

void Block() {
    G[blockX][blockY] = 4;
    G[blockX][blockY + 1] = 4;
    G[blockX][blockY + 2] = 4;

    G[blockX + 1][blockY] = 4;
    G[blockX + 1][blockY + 1] = 4;
    G[blockX + 1][blockY + 2] = 4;
}

void Bomb() {
    G[bombX][bombY] = 2;
    G[bombX][bombY - 1] = 3;
    G[bombX][bombY - 2] = 3;
}

void print() {
    int i, j;
    printf("Celestial Combat:\n");
    for (i = 1; i <= 25; i++) {
        for (j = 1; j <= 75; j++) {
            if (i == 1 || i == 25)
                printf("-");
            else if (j == 1 || j == 75)
                printf("|");
            else if (G[i][j] == 1)
                printf("*");
            else if (G[i][j] == 2)
                printf(">");
            else if (G[i][j] == 3)
                printf("-");
            else if (G[i][j] == 4)
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void Score(){
    printf("Score: %d\n",score);
}

void set() {
    int i, j;
    for (i = 1; i <= 25; i++) {
        for (j = 1; j <= 75; j++) {
            G[i][j] = 0;
        }
    }

    Plane();

    if (bombOn) {
        Bomb();
    }

    if (blockOn) {
        Block();
    }
}

void gameOver() {
    printf("Game Over! Your Score: %d\n", score);

}


// Function to play boom sound effect
void playBoomSound() {
    Beep(1000, 500);
}

void main() {
    int i, j, k;
    srand(time(NULL));
    planeMove = 12;

    blockX = rand() % 20 + 2;
    blockY = 72;

    set();
    print();
    Score();
    getch();
    system("CLS");

    while (1) {
        set();
        print();
        if (GetAsyncKeyState(VK_UP)) {
            if (planeMove == 2)
                continue;
            if (G[planeMove - 1][3] == 0) {
                planeMove--;
            }
        } else if (GetAsyncKeyState(VK_DOWN)) {
            if (planeMove + 5 == 25)
                continue;
            if (G[planeMove + 5][3] == 0) {
                planeMove++;
            }
        }

        if (bombOn) {
            if (G[bombX][bombY + 1] == 0) {
                bombY += 3;
            }
            if (bombY >= 75) {
                bombOn = 0;
            }

            // Check if the bomb hits the block
            if (G[bombX][bombY] == 4 || G[bombX][bombY + 1] == 4 || G[bombX][bombY + 2] == 4) {
                score += 10;
                bombOn = 0;
                blockOn = 0;
                playBoomSound();
            }
        }

        if (blockOn) {
            if (G[blockX][blockY - 1] == 0) {
                blockY -= 2;
            } else if(G[blockX][blockY-1]==1 || G[blockX][blockY-2]==1){
                // Game over if the block hits the plane
                gameOver();
                break;
            }
            if(blockY <= 3){
				blockOn = 0;
			}
        }

        if (blockOn == 0) {
            blockX = rand()%20 + 2;
			blockY = 72;
			blockOn = 1;
        }

        if (GetAsyncKeyState(VK_SPACE) && bombOn == 0) {
            bombX = planeMove + 2;
            bombY = 5;
            bombOn = 1;

        }
        Score();
        Sleep(18);
        system("CLS");
    }
    getchar();
}
