#pragma once

// Grid and nodes def

struct grid_structure
{
  int rows;
  int cols;
  int * top_restrictions; // Positive restriction
  int * bottom_restrictions; // Negative restriction
  int * right_restrictions; // Negative restriction
  int * left_restrictions; // Positive restriction
  struct cell_structure ** data;
  struct cell_structure ** cells;
};

struct cell_structure
{
  int polarity; // 1 (positive), 0 (neutral), -1 neg
  int position_x;
  int position_y;
  int other_half_x;
  int other_half_y;
  bool main;
};

typedef struct cell_structure Cell;
typedef struct grid_structure Grid;


// functions
Grid* newGrid(int rows, int cols);
int setType(Grid* matrix, int row, int col, int t);
