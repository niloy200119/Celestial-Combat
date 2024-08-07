#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
#define MAX_SCORES 5
#define MAX_NAME_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} ScoreEntry;

ScoreEntry highScores[MAX_SCORES];

// Function prototypes
void saveHighScores();
void loadHighScores();
void updateHighScores(int score, const char *name);
void printHighScores();


int board[26][76], planeMove, bombY, bombX, bombOn = 0;
int blockOn = 0, blockY, blockX, score = 0;

void clscreen();
void cursor(int row, int col);
void playBoomSound();
void playBlockHitSound();

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void setCellValue(int planeRow, int col, char val){
    cursor(planeRow, col);
    printf("%c", val);
}

void clearPlane(){
    board[planeMove][3] = 1;

    board[planeMove + 1][3] = 0;
    board[planeMove + 1][4] = 0;

    board[planeMove + 2][3] = 0;
    board[planeMove + 2][4] = 0;
    board[planeMove + 2][5] = 0;

    board[planeMove + 3][3] = 0;
    board[planeMove + 3][4] = 0;

    board[planeMove + 4][3] = 0;

    setCellValue(planeMove, 3, ' ');

    setCellValue(planeMove + 1, 3, ' ');
    setCellValue(planeMove + 1, 4, ' ');

    setCellValue(planeMove + 2, 3, ' ');
    setCellValue(planeMove + 2, 4, ' ');
    setCellValue(planeMove + 2, 5, ' ');

    setCellValue(planeMove + 3, 3, ' ');
    setCellValue(planeMove + 3, 4, ' ');

    setCellValue(planeMove + 4, 3, ' ');
}

void Plane() {
    setConsoleColor(12);
    board[planeMove][3] = 1;

    board[planeMove + 1][3] = 1;
    board[planeMove + 1][4] = 1;

    board[planeMove + 2][3] = 1;
    board[planeMove + 2][4] = 1;
    board[planeMove + 2][5] = 1;

    board[planeMove + 3][3] = 1;
    board[planeMove + 3][4] = 1;

    board[planeMove + 4][3] = 1;

     setCellValue(planeMove, 3, '▲');

    setCellValue(planeMove + 1, 3, '▲');
    setCellValue(planeMove + 1, 4, '▲');

    setCellValue(planeMove + 2, 3, '▲');
    setCellValue(planeMove + 2, 4, '▲');
    setCellValue(planeMove + 2, 5, '▲');

    setCellValue(planeMove + 3, 3, '▲');
    setCellValue(planeMove + 3, 4, '▲');

    setCellValue(planeMove + 4, 3, '▲');



}

void Block() {
    setConsoleColor(10);
    board[blockX][blockY] = 4;
    board[blockX][blockY + 1] = 4;
    board[blockX][blockY + 2] = 4;

    board[blockX + 1][blockY] = 4;
    board[blockX + 1][blockY + 1] = 4;
    board[blockX + 1][blockY + 2] = 4;


    setCellValue(blockX, blockY, '#');
    setCellValue(blockX, blockY + 1, '#');
    setCellValue(blockX, blockY + 2, '#');

    setCellValue(blockX + 1, blockY, '#');
    setCellValue(blockX + 1, blockY + 1, '#');
    setCellValue(blockX + 1, blockY + 2, '#');
}


void clearBlock() {
    setConsoleColor(10);
    board[blockX][blockY] = 0;
    board[blockX][blockY + 1] = 0;
    board[blockX][blockY + 2] = 0;

    board[blockX + 1][blockY] = 0;
    board[blockX + 1][blockY + 1] = 0;
    board[blockX + 1][blockY + 2] = 0;


    setCellValue(blockX, blockY, ' ');
    setCellValue(blockX, blockY + 1, ' ');
    setCellValue(blockX, blockY + 2, ' ');

    setCellValue(blockX + 1, blockY, ' ');
    setCellValue(blockX + 1, blockY + 1, ' ');
    setCellValue(blockX + 1, blockY + 2, ' ');
}

void Bomb() {
    setConsoleColor(9);

    board[bombX][bombY] = 2;
    board[bombX][bombY - 1] = 3;
    board[bombX][bombY - 2] = 3;

    setCellValue(bombX, bombY, '>');
    setCellValue(bombX, bombY - 1, '-');
    setCellValue(bombX, bombY - 2, '-');
}

void clearBomb(){
    board[bombX][bombY] = 0;
    board[bombX][bombY - 1] = 0;
    board[bombX][bombY - 2] = 0;

    setCellValue(bombX, bombY, ' ');
    setCellValue(bombX, bombY - 1, ' ');
    setCellValue(bombX, bombY - 2, ' ');

}

void printBoard(){
    setConsoleColor(14);
    printf("Celestial Combat:\n");

    int i, j;
    printf("\n");
    for (i = 1; i <= 25; i++) {
        for (j = 1; j <= 77; j++) {
            if (i == 1 || i == 25)
                printf("-");
            else if (j == 1 || j == 77)
                printf("|");
            else
                printf(" ");
        }
        printf("\n");
    }

}


void playBlockHitSound() {
    Beep(1200, 300);
}

void Score(){
    cursor(28, 0);
    printf("Score: %d\n",score);
}

void set() {
    int i, j;
    for (i = 1; i <= 25; i++) {
        for (j = 1; j <= 75; j++) {
            board[i][j] = 0;
        }
    }

    Plane();
    Block();

    if (bombOn) {
        Bomb();
    }

    if (blockOn) {
        Block();
    }
}


void gameOver() {
    cursor(28, 0);
    printf("DYNAMIC_DRILLERS\n");
    printf("Game Over! Your Score: %d\n", score);

}



void playBoomSound() {
    Beep(1000, 500);
}

int main() {
    loadHighScores();
    clscreen();
    printBoard();
    int i, j, k;
    srand(time(NULL));
    planeMove = 12;

    blockX = rand() % 20 + 5;
    blockY = 72;

    set();
    getch();
    clscreen();
    printBoard();

    while (1) {
        set();
        if (GetAsyncKeyState(VK_UP)) {
            if (planeMove == 3)
                continue;
            if (board[planeMove - 1][3] == 0) {
                clearPlane();
                planeMove--;
                Plane();
            }
        } else if (GetAsyncKeyState(VK_DOWN)) {
            if (planeMove + 3 == 25)
                continue;
            if (board[planeMove + 5][3] == 0) {
                clearPlane();
                planeMove++;
                Plane();
            }
        }

        if (bombOn) {
            if (board[bombX][bombY + 1] == 0) {
                clearBomb();
                bombY += 5;
                Bomb();
            }
            if (bombY >= 75) {
                bombOn = 0;
                clearBomb();
                setConsoleColor(14);
                cursor(bombX, 76); printf("|");
            }

            if((bombX == blockX || bombX == blockX + 1) && bombY >= blockY - 2) {
                score += 10;
                bombOn = 0;
                blockOn = 0;
                playBoomSound();
                clearBomb();
                clearBlock();
            }
        }

        if(board[blockX][blockY-1]==1 || board[blockX][blockY-2]==1 || board[blockX+1][blockY-1]==1 || board[blockX+1][blockY-2]==1){
            playBlockHitSound();
            gameOver();
            break;
        }

        if (blockOn) {
            if (board[blockX][blockY - 1] == 0) {
                clearBlock();
                blockY -= 1;
                Block();
            }
            if(blockY <= 3){
                clearBlock();
				blockOn = 0;
			}
        }

        if (blockOn == 0) {
            clearBlock();
            blockX = rand()%18 + 5;
			blockY = 72;
            Block();
			blockOn = 1;
        }

        if (GetAsyncKeyState(VK_SPACE) && bombOn == 0) {
            bombX = planeMove + 2;
            bombY = 6;
            bombOn = 1;
            Plane(); // remove if issue occurs
        }
        Score();
        Sleep(30);
    }
    char playername[MAX_NAME_LENGTH];
    printf("Enter your name : ");
    scanf("%s",playername);
    updateHighScores(score, playername); // Update with the actual score and player name
    saveHighScores();
    printHighScores();

    getchar();
    return 0;

}
void saveHighScores() {
    FILE *file = fopen("highscores.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < MAX_SCORES; i++) {
        fprintf(file, "%s %d\n", highScores[i].name, highScores[i].score);
    }

    fclose(file);
}

void loadHighScores() {
    FILE *file = fopen("highscores.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading. Initializing high scores.\n");
        // Initialize high scores with default values
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(highScores[i].name, "Unknown");
            highScores[i].score = 0;
        }
        return;
    }

    for (int i = 0; i < MAX_SCORES; i++) {
        if (fscanf(file, "%s %d", highScores[i].name, &highScores[i].score) != 2) {
            printf("Error reading high scores from file.\n");
            break;
        }
    }

    fclose(file);
}

void updateHighScores(int score, const char *name) {
    int i, j;
    for (i = 0; i < MAX_SCORES; i++) {
        if (score > highScores[i].score) {
            // Shift lower scores down
            for (j = MAX_SCORES - 1; j > i; j--) {
                strcpy(highScores[j].name, highScores[j - 1].name);
                highScores[j].score = highScores[j - 1].score;
            }
            // Insert new score
            strcpy(highScores[i].name, name);
            highScores[i].score = score;
            break;
        }
    }
}

void printHighScores() {
    printf("\nHigh Scores:\n");
    for (int i = 0; i < MAX_SCORES; i++) {
        printf("%d. %s - %d\n", i + 1, highScores[i].name, highScores[i].score);
    }
}


void clscreen(){
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}


void cursor(int row, int col){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col, row});
}





//Thank_u_everyone!
//DYNAMIC_DRILLERS
//SUST CSE 21
