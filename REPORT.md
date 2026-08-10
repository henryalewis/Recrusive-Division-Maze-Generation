# Report: Maze Generation

This report documents the maze generation module of the project: the `Maze` class,
the recursive division generator, entrance/exit placement, and the flood-fill
validation mode. This is the portion of the project I implemented myself.

## Overview

`maze-gen` produces a random *perfect* maze using the **recursive division**
algorithm and prints it to `stdout` as an ASCII grid, where `X` is a wall, a space
is a passage, `S` is the entrance and `E` is the exit. A perfect maze has exactly
one path between any two cells and contains no loops. Generation is deterministic:
the same `<width> <height> <seed>` always produces the same maze, which the test
suite relies on.

The program has a second mode, `--validate`, which reads a maze from `stdin` and
runs a flood fill to confirm that every open cell is reachable. This self-check can
be piped directly onto the generator's output.

## The `Maze` class

The maze is stored as a `char**` grid, a pointer to an array of row pointers where
each row is a dynamically allocated `char` array, built and freed with explicit
`new`/`delete`. No STL containers are used for the core grid, in line with the
project's primitive-data-structures constraint.

The public interface is:

- `Maze(int width, int height, int seed)`: allocates the grid and seeds the RNG via `srand(seed)`.
- `Maze(int width, int height)`: an alternate constructor for building a grid from existing data rather than generating one.
- `~Maze()`: frees every row, then the row-pointer array.
- `generate()`: fills the border with walls, opens the interior, runs recursive division, and places `S` and `E`.
- `validate() const`: runs the flood-fill connectivity check.
- `isWall(x, y) const` / `getCell(x, y) const`: read a cell.
- `setCell(x, y, c)`: write a cell (used when loading a maze from text).
- `getWidth()` / `getHeight()` / `getStartX()` / `getStartY()` / `getEndX()` / `getEndY()`: dimensions and endpoint coordinates.
- `print() const`: writes the ASCII grid to `stdout`.

### Memory management

Every heap allocation has a matching free. The destructor releases each row
individually before releasing the array of row pointers, and the flood fill frees
its `visited` grid and its manual stack arrays before returning. The generator was
verified to have zero memory leaks with Valgrind.

## Recursive division algorithm

Generation starts by filling the whole grid with walls, then clearing the interior
so it is one open chamber bounded by a wall border. `divide(x, y, w, h)` then
recursively subdivides that chamber:

1. **Base case.** If the region is smaller than 2 cells in either dimension, it can't be split further, so the call returns.
2. **Orientation.** A wall is drawn horizontally if the region is taller than it is wide, vertically if it is wider than tall, and in a random direction when the region is square. Splitting along the longer axis keeps the sub-regions from becoming long thin corridors.
3. **Wall and gap.** The dividing wall is placed on an even offset within the region and a single gap is carved through it on an odd offset. This parity is what keeps passages one cell wide and guarantees the two halves stay connected by exactly one opening.
4. **Recurse.** The function calls itself on the two sub-regions created by the wall.

Because each split adds exactly one wall with exactly one gap, and every recursive
call operates on a strictly smaller region, the result is a perfect maze: fully
connected, with no loops.

## Entrance and exit placement

After division, `placeEntrance()` scans the left outer wall from top to bottom and
places `S` at the first row whose adjacent interior cell is a passage.
`placeExit()` scans the right outer wall from bottom to top and places `E` the same
way. Because the entrance is taken from the left wall and the exit from the right,
they always sit on opposing sides, so any solver must traverse the full interior.

## Flood-fill validation

In `--validate` mode, `main-gen` reads the maze from `stdin` into a `Maze` object
and calls `validate()`. The check works as follows:

- Allocate a `bool` grid the size of the maze, all cells initially unvisited.
- Locate `S` as the flood-fill origin; if there is no `S`, the maze is invalid.
- Flood fill outward using a manual stack (two `int` arrays sized `width × height`), marking each reachable open cell and counting how many are reached.
- Count the total number of open cells in the grid.
- The maze is **valid** only if the flood fill reached every open cell; otherwise open cells are split into disconnected components and the maze is **invalid**.

It prints exactly `VALID` or `INVALID` to `stderr`. Piping the generator into this
mode (`maze-gen 11 11 99 | maze-gen --validate`) gives a fast correctness check
that any generated maze is fully connected, for any seed and size.

## Complexity and determinism

Both generation and validation are linear in the grid area, `O(width × height)`.
Recursive division writes each wall and gap a bounded number of times, and the
flood fill visits each cell at most once. Because the RNG is seeded once with
`srand(seed)`, the output is a pure function of `(width, height, seed)`, which is
the property the deterministic-seeding tests depend on.

## Testing

The generator is covered by black-box tests that verify `stdout` and `stderr`
separately using `diff`. They target valid generation at the smallest odd size,
non-square generation, deterministic reproduction under a fixed seed, the invariant
that any generated maze passes `--validate`, rejection of even / too-small / mixed
dimensions, and validation of both a deliberately disconnected maze and empty
input. Every case, with its exact reproduction command and expected output, is
documented in [TESTING.md](TESTING.md).