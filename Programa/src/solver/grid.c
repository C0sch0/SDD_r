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

// ---------------------------

int value_cell(Grid * grid, int x, int y){
  return grid->data[x][y]->polarity;
}


bool check_around(Grid * grid, int x, int y, int cell_value){

  if (cell_value != 0){
    if (x + 1 < grid->rows){
      if (value_cell(grid, x + 1, y) == cell_value){
        return false;
      }
    }
    if (x - 1 >= 0){
      if (value_cell(grid, x - 1, y) == cell_value){
        return false;
      }
    }
    if (y + 1 < grid->cols){
      if (value_cell(grid, x, y + 1) == cell_value){
        return false;
      }
    }
    if (y - 1 >= 0){
      if (value_cell(grid, x, y - 1) == cell_value){
        return false;
      }
    }
  }
  //printf("Pased Around\n");
  return true;
}

bool check_restrictions(Grid * grid, int x, int y, int cell_value){
  if (cell_value == 0){
      return true;
  }
  int x_pos = grid->left_restrictions[x];
  int x_neg = grid->right_restrictions[x];
  int y_pos = grid->top_restrictions[y];
  int y_neg = grid->bottom_restrictions[y];
  if ((cell_value == 1) && (x_pos == 0 || y_pos == 0)){
    return false;
  }
  else if ((cell_value == -1) && (x_neg == 0 || y_neg == 0)){
    return false;
  }
  //printf("Pased restrictions\n");
  return true;
}




bool check_validity(Grid * board, Cell * cell, int cell_value){
  int x = cell->position_x;
  int y = cell->position_y;
  int x_2 = cell->other_half_x;
  int y_2 = cell->other_half_y;
  int cell_value_2 = - cell_value;
  if (!check_around(board, x, y, cell_value)){
    return false;}
  if (!check_around(board, x_2, y_2, cell_value_2)){
    return false;}
  if (!check_restrictions(board, x, y, cell_value)){
    return false;}
  if (!check_restrictions(board, x_2, y_2, cell_value_2)){
    return false;}
  return true;}


bool is_complete(Grid * grid){
  for (int row = 0; row < grid->rows; row++){
    int neg = grid->right_restrictions[row];
    int pos = grid->left_restrictions[row];
    if (!(neg == -2 || neg == 0)){
      return false;}
    if (!(pos == -2 || pos == 0)){
      return false;}
  }

  for (int col = 0; col < grid->cols; col++){
    int neg = grid->bottom_restrictions[col];
    int pos = grid->top_restrictions[col];
    if (!(neg == -2 || neg == 0)){
      return false;}
    if (!(pos == -2 || pos == 0)){
      return false;}
  }
return true;
}


void board_place(Grid * grid, Cell * cell, int cell_value){

  int x = cell->position_x;
  int y = cell->position_y;
  int x_2 = cell->other_half_x;
  int y_2 = cell->other_half_y;
  grid->data[x][y]->polarity = cell_value;

  if (cell_value == 1){
    if (grid->left_restrictions[x] != -2) grid->left_restrictions[x]--;
    if (grid->top_restrictions[y] != -2) grid->top_restrictions[y]--;
  }

  else if (cell_value == -1){
    if (grid->right_restrictions[x] != -2) grid->right_restrictions[x]--;
    if (grid->bottom_restrictions[y] != -2) grid->bottom_restrictions[y]--;
  }

  grid->data[x_2][y_2]->polarity = -cell_value;

  if (cell_value == -1){
    if (grid->left_restrictions[x_2] != -2) grid->left_restrictions[x_2]--;
    if (grid->top_restrictions[y_2] != -2) grid->top_restrictions[y_2]--;
  }

  else if (cell_value == 1){
    if (grid->right_restrictions[x_2] != -2) grid->right_restrictions[x_2]--;
    if (grid->bottom_restrictions[y_2] != -2) grid->bottom_restrictions[y_2]--;
  }
}




void board_remove(Grid * grid, Cell * cell){

  int x = cell->position_x;
  int y = cell->position_y;
  int x_2 = cell->other_half_x;
  int y_2 = cell->other_half_y;


  int cell_value = value_cell(grid, x, y);

  if (cell_value == 1){
    if (grid->left_restrictions[x] != -2) grid->left_restrictions[x]++;
    if (grid->top_restrictions[y] != -2) grid->top_restrictions[y]++;

    if (grid->right_restrictions[x_2] != -2) grid->right_restrictions[x_2]++;
    if (grid->bottom_restrictions[y_2] != -2) grid->bottom_restrictions[y_2]++;
  }

  else if (cell_value == -1){
    if (grid->right_restrictions[x] != -2) grid->right_restrictions[x]++;
    if (grid->bottom_restrictions[y] != -2) grid->bottom_restrictions[y]++;

    if (grid->left_restrictions[x_2] != -2) grid->left_restrictions[x_2]++;
    if (grid->top_restrictions[y_2] != -2) grid->top_restrictions[y_2]++;
  }

  grid->data[x][y]->polarity = 0;
  grid->data[x_2][y_2]->polarity = 0;
}

void free_board(Grid * grid){

  for (int row = 0; row < grid->rows; row++){
    for (int col = 0; col < grid->cols; col++){
      free(grid->data[row][col]);
    }
    free(grid->data[row]);
  }
  free(grid->data);
  free(grid->cells);
  free(grid->bottom_restrictions);
  free(grid->top_restrictions);
  free(grid->left_restrictions);
  free(grid->right_restrictions);
  free(grid);
}
