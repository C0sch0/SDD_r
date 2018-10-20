#include "grid.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/** http://theory.stanford.edu/~arbrad/pfe/06/matrix.c **/
/* Creates a ``rows by cols'' matrix with all values 0.
Returns NULL if rows <= 0 or cols <= 0, otherwise a pointer to the new matrix. */
int magnet_counter = 0;

Grid * newGrid(int rows, int cols) {
  if (rows <= 0 || cols <= 0) return NULL;
  Grid * grid = (Grid*) malloc(sizeof(Grid));
  grid -> rows = rows;
  grid -> cols = cols;
  grid -> data = malloc(sizeof(Cell **) * rows);
  grid -> cells = malloc(sizeof(Cell *) * (rows * cols / 2));
  return grid;
}


void set_values_cell(Grid* grid,
                    int row,
                    int col,
                    int dev_row,
                    int dev_col,
                    bool main){
  Cell * cell = malloc(sizeof(Cell));
  cell->polarity = 0;
  cell->direccion = main;
  cell->position_x = row;
  cell->position_y = col;
  cell->other_half_x = row + dev_row;
  cell->other_half_y = col + dev_col;
  grid->data[row][col] = cell;
  if (main){
    grid->cells[magnet_counter] = cell;
    magnet_counter++;
  }
}
