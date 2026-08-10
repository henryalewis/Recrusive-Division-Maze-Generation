# Recursive Division Maze Generator (C++)

A command-line maze generator written in modern C++. It builds random *perfect*
mazes using the recursive division algorithm, prints them as an ASCII grid, and can
validate that a maze is fully connected with a flood fill. It is built entirely on
primitive data structures, using raw pointers, dynamic arrays, and manual
`new`/`delete`, with zero memory leaks confirmed by Valgrind.

> **Context.** This repository is the maze *generation* module I wrote for a
> two-person university project. The original project also included a maze solver
> and a 3D Minecraft renderer, which were my partner's and shared work respectively
> and are not part of this repository. What you see here is the portion I designed
> and implemented myself.

## What it does

`maze-gen` has two modes:

- **Generate:** produces a random perfect maze and prints it to `stdout` as an ASCII grid.
- **Validate** (`--validate`): reads a maze from `stdin` and runs a flood fill to confirm every open cell is reachable, printing `VALID` or `INVALID`.

A perfect maze has exactly one path between any two cells and contains no loops.
In the grid, `X` is a wall, a space is a passage, `S` is the entrance on the left
wall and `E` is the exit on the right wall. Generation is deterministic: the same
width, height and seed always produce the same maze.

## Example

```text
$ ./bin/maze-gen 5 5 1
XXXXX
S X X
X X X
X   E
XXXXX

# Confirm any generated maze is fully connected (VALID is printed to stderr)
$ ./bin/maze-gen 11 11 99 | ./bin/maze-gen --validate
VALID
```

## Build

Requires `g++` with C++17 support. There are no external dependencies.

```bash
make          # builds bin/maze-gen
make test     # generates a maze and checks it validates as connected
make clean    # removes bin/ and obj/
```

Compiler flags: `-Wall -Werror -std=c++17 -g -O` (warnings are treated as errors).

## Run

```bash
# Generate a maze:  ./bin/maze-gen <width> <height> <seed>
./bin/maze-gen 21 21 99

# Validate a maze read from stdin
./bin/maze-gen 21 21 99 | ./bin/maze-gen --validate
```

Width and height must be **odd** and `>= 3`. Invalid dimensions are rejected with an
error on `stderr` and a non-zero exit code. All diagnostic output goes to `stderr`,
so `stdout` only ever carries the maze grid and stays clean for piping.

## Design highlights

**Recursive division generator.** The grid starts as an open interior boxed by
walls, and `divide()` recursively splits each region by drawing a wall and carving a
single gap. Wall orientation is chosen by region shape, and at random when the
region is square, so every recursive call produces a smaller perfect sub-maze.
Placing walls on even offsets and gaps on odd offsets keeps every passage one cell
wide and leaves each split joined by exactly one opening.

**Raw-pointer `Maze` class.** The grid is a `char**` allocated with explicit
`new`/`delete`, with a destructor that frees every row and then the row-pointer
array. No STL containers are used for the core grid.

**Flood-fill validation.** `--validate` floods outward from `S` using a manual
stack and confirms every open cell is reached, catching disconnected or looping
mazes. Piping the generator straight into it gives a fast correctness check for any
seed and size.

**Deterministic and linear.** Both generation and validation run in `O(width x
height)`. Because the RNG is seeded once, the output is a pure function of the
arguments, which is what makes the maze reproducible under a fixed seed and easy to
test.

## Project layout

```
maze.{h,cpp}     Maze class: grid, recursive division, entrance/exit, validation
main-gen.cpp     maze-gen entry point (generate + --validate)
Makefile         builds bin/maze-gen
tests/           black-box test inputs and expected output
TESTING.md       black-box testing methodology
REPORT.md        design write-up of the generation module
CONTRIBUTING.md  authorship and contribution notes
```

## Testing

The generator is covered by black-box tests that verify `stdout` and `stderr`
separately with `diff`, across edge cases including invalid dimensions, non-square
mazes, deterministic seeding, the invariant that every generated maze passes
`--validate`, and validation of disconnected and empty input. See
[TESTING.md](TESTING.md) for every case and the exact commands to reproduce them.

## Author

Henry Lewis
