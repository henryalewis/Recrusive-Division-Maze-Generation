# Team Contributions

## Members

| Name | Responsibilities |
|---|---|---|---|
| Henry Lewis | Maze Generation |

## Individual Contributions

### Student 1 — Henry Lewis

I implemented the maze generator (maze-gen) and the Maze class. The maze generation was completed using a recursive division algorithm; the grid starts as an open interior boxed by walls, and divide() recursively splits each region by drawing a wall and carving a single gap, choosing wall orientation by region shape (and at random when square) so that every recursive call produces a smaller perfect sub-maze. The Maze class manages its 2D grid with raw char** arrays and explicit new/delete, with a destructor that frees every row. 

I also implemented entrance/exit placement, scanning the left wall top to bottom for S and the right wall bottom to top for E so they always sit on opposing sides. The --validate flood-fill, confirms a maze is fully connected. The process required drafting black box tests in TESTING.md that could give a clear target for the correct output. The main challenge was orientation: the generated ASCII was correct, but the Minecraft render came out mirrored relative to the grid. I resolved this by flipping the row index when placing blocks in main-view, so the 3D maze matches the ASCII output correctly.