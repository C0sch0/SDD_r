#pragma once

// Grid and nodes def

struct grid_structure
{
  int rows;
  int cols;
  struct cell_structure ** data;
};

struct cell_structure
{
  int polarity; // 1 (positive), 0 (neutral), -1 (you get the idea)
  struct cell_structure *up;
  struct cell_structure *down;
  struct cell_structure *left;
  struct cell_structure *right;
};

typedef struct cell_structure Cell;
typedef struct grid_structure Grid;


// functions
Grid* newGrid(int rows, int cols);
