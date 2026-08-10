#include "maze.h"
#include <iostream>
#include <cstdlib>

// Constructor without seed for main-solve
Maze::Maze(int width, int height) : grid(nullptr), seed(0) {
    this->width = width;
    this->height = height;

    startX = -1;
    startY = -1;
    endX = -1;
    endY = -1;

    // grid of char pointers height tall and width wide
    grid = new char*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width + 1];
    }
}

Maze::Maze(int width, int height, int seed) {
    this->width = width;
    this->height = height;
    this->seed = seed;

    startX = 0;
    startY = 0;
    endX = 0;
    endY = 0;

    // grid of char pointers height tall and width wide
    grid = new char*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
    }

    srand(seed);
}

// deconstructor
Maze::~Maze() {
    for (int i = 0; i < height; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

int Maze::getWidth() const {
    return width;
}

int Maze::getHeight() const {
    return height;
}

int Maze::getStartX() const {
    return startX;
}

int Maze::getStartY() const {
    return startY;
}

int Maze::getEndX() const {
    return endX;
}

int Maze::getEndY() const {
    return endY;
}

char Maze::getCell(int x, int y) const {
    return grid[y][x]; // height and width
}

bool Maze::isWall(int x, int y) const {
    return grid[y][x] == 'X';
}

void Maze::print() const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            std::cout << grid[y][x];
        }
        std::cout << '\n';
    }
}

void Maze::fillWalls() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = 'X';
        }
    }
}

void Maze::generate() {
    fillWalls();
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            grid[y][x] = ' ';
        }
    }

    // recursive function
    divide(1, 1, width - 2, height - 2);
    placeEntrance();
    placeExit();

}

void Maze::divide(int x, int y, int w, int h) {
    // base case
    if (w < 2 || h < 2) {
        return;
    }

    // decide orientation
    bool horizontal;
    if (w < h) {
        horizontal = true;   // height bigger than width, draw horizontal wall
    } else if (w > h) {
        horizontal = false;  // width bigger than height, draw vertical wall
    } else {
        horizontal = (rand() % 2 == 0);  // square, pick at random
    }

    if (horizontal) {
        // pick random even row inside region for wall
        int wallY = y + 1 + (rand() % ((h - 1) / 2)) * 2;
        // pick random odd column inside region for gap
        int gapX = x + (rand() % ((w + 1) / 2)) * 2;

        // draw horizontal wall
        for (int i = x; i < x + w; i++) {
            grid[wallY][i] = 'X';
        }
        // carve gap
        grid[wallY][gapX] = ' ';

        // recurse on top and bottom regions
        divide(x, y, w, wallY - y);
        divide(x, wallY + 1, w, y + h - wallY - 1);

    } else {
        // pick random even column inside region for wall
        int wallX = x + 1 + (rand() % ((w - 1) / 2)) * 2;
        // pick random odd row inside region for gap
        int gapY = y + (rand() % ((h + 1) / 2)) * 2;

        // draw vertical wall
        for (int i = y; i < y + h; i++) {
            grid[i][wallX] = 'X';
        }
        // carve gap
        grid[gapY][wallX] = ' ';

        // recurse on left and right regions
        divide(x, y, wallX - x, h);
        divide(wallX + 1, y, x + w - wallX - 1, h);
    }
}

void Maze::placeEntrance() {
    // scans left wall from the top to bottom
    for (int y = 1; y < height - 1; y++) {
        if (grid[y][1] == ' ') {  // checks if cell adjacent to left wall is a gap
            grid[y][0] = 'S';
            startX = 0;
            startY = y;
            return;
        }
    }
}

void Maze::placeExit() {
    // scans right wall bottom to top
    for (int y = height - 2; y >= 1; y--) {
        if (grid[y][width - 2] == ' ') {  // checks if cell adjacent to right wall is a gap
            grid[y][width - 1] = 'E';
            endX = width - 1;
            endY = y;
            return;
        }
    }
}

bool Maze::validate() const {
    // allocate a 2d array the same size as the grid
    bool** visited = new bool*[height]; // every cell is false until visited
    for (int i = 0; i < height; i++) {
        visited[i] = new bool[width];
        for (int j = 0; j < width; j++) {
            visited[i][j] = false;
        }
    }

    // find S to start the flood fill
    int startFillX = -1, startFillY = -1;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == 'S') {
                startFillX = x;
                startFillY = y;
            }
        }
    }

    // if no S found, invalid
    if (startFillX == -1) {
        // free visited
        for (int i = 0; i < height; i++) delete[] visited[i];
        delete[] visited;
        return false;
    }

    // flood fill using a manual stack
    // stack stores x,y pairs and max size is width*height
    int* stackX = new int[width * height];
    int* stackY = new int[width * height];
    int top = 0;

    stackX[top] = startFillX;
    stackY[top] = startFillY;
    top++;
    visited[startFillY][startFillX] = true;

    int visitedCount = 1;

    while (top > 0) {
        top--;
        int cx = stackX[top];
        int cy = stackY[top];

        // check all 4 neighbours
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];

            // bounds check
            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
            // skip walls and already visited
            if (grid[ny][nx] == 'X') continue;
            if (visited[ny][nx]) continue;

            visited[ny][nx] = true;
            visitedCount++;
            stackX[top] = nx;
            stackY[top] = ny;
            top++;
        }
    }

    // count total open cells
    int totalOpen = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] != 'X') totalOpen++;
        }
    }

    // free everything
    for (int i = 0; i < height; i++) delete[] visited[i];
    delete[] visited;
    delete[] stackX;
    delete[] stackY;

    // if flood fill reached all open cells, maze is valid
    return visitedCount == totalOpen;
}

void Maze::setCell(int x, int y, char c) {
    grid[y][x] = c;
    if (c == 'S') {
        startX = x; startY = y;
    }
    if (c == 'E') {
        endX = x; endY = y;
    }
}