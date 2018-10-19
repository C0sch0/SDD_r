#include "grid.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/** http://theory.stanford.edu/~arbrad/pfe/06/matrix.c **/
/* Creates a ``rows by cols'' matrix with all values 0.
Returns NULL if rows <= 0 or cols <= 0, otherwise a pointer to the new matrix. */

Grid* newGrid(int rows, int cols) {
  if (rows <= 0 || cols <= 0) return NULL;
  // allocate a matrix structure
  Grid* m = (Grid*) malloc(sizeof(Grid));
  // set dimensions
  m -> rows = rows;
  m -> cols = cols;

  // allocate a double array of length rows * cols
  m -> data = (Cell**) malloc(rows*cols*sizeof(Cell*));
  // set all data to initial values
  for (int i = 0; i < rows*cols; i++){
    m -> data[i] = (Cell*) malloc(sizeof(Cell));
  }
  return m;
}
