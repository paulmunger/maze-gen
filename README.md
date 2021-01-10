# maze-gen
Simple labyrinth generator

## Compilation

	clang -O3 maze.c -o maze
	clang -O3 bigmaze.c -o bigmaze

## Use

    maze 400 400 11 30 26 > out.pgm
    bigmaze 800 400 11 30 26 6 4 > out.pgm

Parameters are:
1. image size
1. number of iterations
1. random seed
1. first LFSR tap
1. second tap
1. amount of scale variation (bigmaze only)
1. minimum scale (bigmaze only)
