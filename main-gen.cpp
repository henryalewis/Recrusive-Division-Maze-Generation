#include "maze.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

int main(int argc, char** argv) { 
    // if argument count is 2 and argument vector 1 is --validate
    if (argc == 2 && std::strcmp(argv[1], "--validate") == 0) {
        // read maze from stdin
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(std::cin, line)) {
            lines.push_back(line);
        }
        if (lines.empty()) {
            std::cerr << "INVALID" << std::endl;
            return 1;
        }
        int h = lines.size();
        int w = lines[0].size();
        // build a temporary maze object to validate
        Maze m(w, h, 0);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < (int)lines[y].size(); x++) {
                m.setCell(x, y, lines[y][x]);
            }
        }
        if (m.validate()) {
            std::cerr << "VALID" << std::endl;
        } else {
            std::cerr << "INVALID" << std::endl;
        }
        return 0;
    } 

    // Check number of arguments
    if (argc != 4) {
        std::cerr << "Error: usage: ./maze-gen <width> <height> <seed>" << std::endl;
        return 1;
    }

    // to integer
    int width = std::atoi(argv[1]);
    int height = std::atoi(argv[2]);
    int seed = std::atoi(argv[3]);

    // validate dimensions
    if (width < 3 || height < 3 || width % 2 == 0 || height % 2 == 0) {
        std::cerr << "Error: width and height must be odd numbers and >= 3\n";
        return 1;
    }

    // Creating maze object
    Maze m(width, height, seed);
    m.generate();
    m.print();
    
    return 0;
}
