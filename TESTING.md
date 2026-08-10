# Testing

Document every black-box test case here. For each test, include:
- The edge condition or behaviour being tested
- The exact CLI command to reproduce it
- The expected `stdout` output
- The expected `stderr` output

---

## `maze-gen` tests

### Henry Lewis

### Test 1: Valid 5x5 generation
Verify that the generator produces a non-empty ASCII grid to `stdout` for dimensions with the smallest possible odd input, and nothing on `stderr`.

Command:
./bin/maze-gen 5 5 1 > out.txt 2> err.txt
diff out.txt tests/gen_5x5_seed1.expected_out
diff err.txt tests/gen_5x5_seed1.expected_err

Expected stdout: 
XXXXX
S X X
X X X
X   E
XXXXX

Expected stderr: (empty)
Exit code: 0

### Test 2: Valid non-square generation (7×5)
Confirms width != height. Confirms rectangular mazes generate
correctly and that `S`/`E` still land on opposing left/right walls.

Command: 
./bin/maze-gen 7 5 3 > out.txt 2> err.txt
diff out.txt tests/gen_7x5_seed3.expected_out
diff err.txt tests/gen_7x5_seed3.expected_err

Expected stdout:
XXXXXXX
S X   X
X XXX X
X     E
XXXXXXX

Expected stderr: (empty)
Exit code: 0

### Test 3: Deterministic seeding
The same seed must reproduce the same maze; different seeds must differ. Verifies the generator is a pure function of its arguments.

Command:
./bin/maze-gen 11 11 1 > tests/g3_seed1.txt
./bin/maze-gen 11 11 1 > tests/g3_seed1b.txt
./bin/maze-gen 11 11 2 > tests/g3_seed2.txt
diff tests/g3_seed1.txt tests/g3_seed1b.txt   # same seed  -> no output, exit 0
diff tests/g3_seed1.txt tests/g3_seed2.txt    # diff seeds -> output,    exit 1

Expected: first `diff` prints nothing and exits `0`; second `diff` prints
differing lines and exits `1`. Both `maze-gen` runs write nothing to `stderr`.

### Test 4: Generated maze is always valid
Any generated maze must pass `--validate`. This is the key correctness check on the generator and works for every seed/size.

Command: 
./bin/maze-gen 11 11 99 | ./bin/maze-gen --validate

Expected stdout: (empty)
Expected stderr: VALID
Exit code: 0

### Test 5: Even dimensions rejected
Even width/height results in an error. Nothing must be written to `stdout`; the error goes to `stderr`.

Command: 
./bin/maze-gen 6 6 1 > out.txt 2> err.txt
diff out.txt tests/gen_even.expected_out
diff err.txt tests/gen_even.expected_err

Expected stdout: (empty)
Expected stderr: Error: width and height must be odd numbers and >= 3
Exit code: 1

### Test 6: Dimensions too small
Generate a maze with dimensions below the minimum of 3.

Command: 
./bin/maze-gen 2 2 1 > out.txt 2> err.txt
diff out.txt tests/gen_small.expected_out
diff err.txt tests/gen_small.expected_err

Expected stdout: (empty)
Expected stderr: Error: width and height must be odd numbers and >= 3
Exit code: 1

### Test 7: Mixed parity rejected (one even dimension)
Confirms that a maze's dimensions are checked independently.

Command:
./bin/maze-gen 5 4 1 > out.txt 2> err.txt
diff out.txt tests/gen_mixed.expected_out
diff err.txt tests/gen_mixed.expected_err

Expected stdout: (empty)
Expected stderr: Error: width and height must be odd numbers and >= 3
Exit code: 1

---

### Test 8: Validate rejects an isolated region
A maze whose open cells are split into disconnected components. Flood-fill from `S` cannot reach `E`, so the maze is not valid.

Must create tests/val_isolated.in with this content:
XXXXX
S X X
XXXXX
X   E
XXXXX

Command:
./bin/maze-gen --validate < tests/val_isolated.in

Expected stdout: (empty)
Expected stderr: INVALID
Exit code: 0

### Test 9: Validate on empty input
Empty stdin: there is no maze to validate.

Command:
printf '' | ./bin/maze-gen --validate

Expected stdout: (empty)
Expected stderr: INVALID
Exit code: 1

---
