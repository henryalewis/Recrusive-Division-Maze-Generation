#ifndef MAZE_H
#define MAZE_H

// Maze — design and implement this class yourself.
class Maze {
    // TODO
    public:
        // Constructor for main-solve
        Maze(int width, int height);
        // Constructor
        Maze(int width, int height, int seed);
        // Destructor
        ~Maze();

        // Getters
        int getWidth() const;
        int getHeight() const;

        // Returns true if x,y is a wall
        bool isWall(int x, int y) const;

        // Returns the cell character ('X', or 'S', or 'E')
        char getCell(int x, int y) const;

        // Start and end coordinates
        int getStartX() const;
        int getStartY() const;
        int getEndX() const;
        int getEndY() const;

        // Print maze grid
        void print() const;

        // Recursive division on the maze (creates the pathways)
        void generate();

        // validate
        bool validate() const;
        void setCell(int x, int y, char c);

        // 2D array of chars (pointer to a collection of pointers that points to row of chars)
        // grid -> ptr -> row of characters
        char** grid;

    private:
        int width;
        int height;
        int seed;

        // Start and End coordinates
        int startX;
        int startY;
        int endX;
        int endY;

        // Helper, the recursive division algorithm
        void divide(int x, int y, int w, int h);

        // Helper, places S and E
        void placeEntrance();
        void placeExit();

        void fillWalls();
};

#endif
