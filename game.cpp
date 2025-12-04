#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// --- Global Variables ---
int width = 30, height = 20;
int gameOver, paused;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
// 1=Up (W), 2=Down (S), 3=Left (A), 4=Right (D), 0=None
int dir; 
int speed; // game speed (delay in ms) based on difficulty

// --- Function to Initialize Game State ---
void setup() {
    gameOver = 0;
    paused = 0;
    x = width / 2;
    y = height / 2;
    // Generate initial fruit position
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    dir = 0; // no movement until key pressed
}

// --- Function to Draw the Game Screen ---
void draw() {
    system("cls"); // Clear the console screen

    // Top border
    for (int i = 0; i < width + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) printf("#"); // Left border

            if (i == y && j == x)
                printf("O");  // Snake head
            else if (i == fruitY && j == fruitX)
                printf("F");  // Fruit
            else {
                int print = 0;
                // Check if current coordinate is part of the tail
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o"); // Snake tail segment
                        print = 1;
                    }
                }
                if (!print) printf(" "); // Empty space
            }

            if (j == width - 1) printf("#"); // Right border
        }
        printf("\n");
    }

    // Bottom border
    for (int i = 0; i < width + 2; i++) printf("#");
    printf("\n");

    printf("Score : %d\n", score);
    printf("Controls: W/A/S/D = Move | P = Pause | R = Resume | X = Exit\n");
}

// --- Function to Handle User Input ---
void input() {
    if (_kbhit()) { // Check if a key has been pressed
        switch (_getch()) { // Get the character without waiting
            case 'w': dir = 1; break; // Up
            case 's': dir = 2; break; // Down
            case 'a': dir = 3; break; // Left
            case 'd': dir = 4; break; // Right
            case 'x': gameOver = 1; break; // Exit game
            case 'p': paused = 1; break; // Pause
            case 'r': paused = 0; break; // Resume
        }
    }
}

// --- Function to Handle Game Logic (Movement, Collisions, Eating) ---
void logic() {
    if (paused) return; // Skip movement and collision checks if paused

    // 1. Move the tail: Each segment takes the position of the one in front of it.
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    // The first segment takes the head's previous position
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // 2. Move the head based on current direction
    switch (dir) {
        case 1: y--; break; // Up
        case 2: y++; break; // Down
        case 3: x--; break; // Left
        case 4: x++; break; // Right
    }

    // 3. Check for boundary collision
    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = 1;

    // 4. Check for self-collision (head hits tail)
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    // 5. Check for fruit consumption
    if (x == fruitX && y == fruitY) {
        score += 10;
        // Generate new fruit position
        fruitX = rand() % width;
        fruitY = rand() % height;
        // Increase tail length (if not maxed out)
        if (nTail < 100) nTail++; 
    }
}

// --- Main Game Execution Function ---
int main() {
    // Seed the random number generator
    srand(time(NULL)); 

    int choice;
    printf("Choose Difficulty:\n1. Easy\n2. Medium\n3. Hard\nEnter choice: ");
    scanf("%d", &choice);

    // Set game speed based on difficulty choice (Sleep delay in milliseconds)
    switch (choice) {
        case 1: speed = 150; break;
        case 2: speed = 100; break;
        case 3: speed = 50; break;
        default: speed = 100; // Default to Medium
    }

    // Main Game Loop (for playing multiple times)
    do {
        setup(); // Initialize game for a new round

        // Game Round Loop (runs until gameOver is true)
        while (!gameOver) {
            draw();
            input();
            logic();
            // Control game speed
            Sleep(speed); 
        }

        printf("Game Over! Final Score: %d\n", score);
        printf("Play Again? (y/n): ");
        // Clear the input buffer from previous scanf and final score screen
        fflush(stdin); 
        char again = _getch(); 

        if (again == 'y' || again == 'Y') {
            gameOver = 0; // Reset for a new game
        } else {
            break; // Exit the do-while loop
        }

    } while (1);

    return 0;
}