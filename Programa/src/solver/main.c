#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "grid.h"


bool backtrack(Grid * grid, int count)
{
	bool complete = _finished(grid);
	if (complete)
		return true;
	else if (!complete && count == (grid->cols * grid->rows / 2)){
		return false;}
	Cell * actual_magnet = grid->cells[count];
	int array[3] = {1, -1, 0};
	for (int i = 0; i < 3; i++){
		int cell_value = array[i];
		if (_valid_move(grid, actual_magnet, cell_value)){
			_put(grid, actual_magnet, cell_value);
			if (backtrack(grid, count+1))return true;
			_eliminate(grid, actual_magnet);}
	}
	return false;
}
int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Modo de uso: %s test.txt\n", argv[0]);
		return 0;}
	watcher_load_layout(argv[1]);

	FILE* test_file = fopen(argv[1], "r");
	int cols;
	int rows;
	fscanf(test_file, "%d %d\n", &cols, &rows);

	Grid * grid = malloc(sizeof(Grid));
	grid->cols = cols;
	grid->rows = rows;
	grid->data = malloc(sizeof(Cell ** ) * rows);
	grid->cells = malloc(sizeof(Cell *) * (rows * cols / 2));

	grid->top_restrictions = malloc(sizeof(int) * cols);
	grid->left_restrictions = malloc(sizeof(int) * rows);
	grid->bottom_restrictions = malloc(sizeof(int) * cols);
	grid->right_restrictions = malloc(sizeof(int) * rows);


	/* Leemos los numeros de las orillas de arriba y abajo*/
	for (int side = 0; side < 2; side++)
	{
		for (int col = 0; col < cols; col++)
		{
			int val;
			fscanf(test_file, "%d ", &val);
			if (val == 0){
				val = -2;
			}
			if (side == 0){
				grid->top_restrictions[col] = val;
			}
			else{
				grid->bottom_restrictions[col] = val;
			}
		}
	}

	for (int side = 2; side < 4; side++)
	{
		for (int row = 0; row < rows; row++)
		{
			int val;
			fscanf(test_file, "%d ", &val);
			if (val == 0){
				val = -2;
			}

			if (side == 2){
				grid->left_restrictions[row] = val;
			}
			else{
				grid->right_restrictions[row] = val;
			}
		}
	}

	for (int row= 0; row< rows; row++)
	{
		grid->data[row] = malloc(sizeof(Cell *) * grid->cols);
	}

	for (int row = 0; row < rows; row++){
		for (int col = 0; col < cols; col++)
		{
			char type;
			fscanf(test_file, "%c ", &type);
			if (type == 'T'){
				_set(grid, row, col, +1, 0, true);
				_set(grid, row + 1, col,  -1, 0, false);
			}
			else if (type == 'L'){
				_set(grid, row, col, 0, + 1, true);
				_set(grid, row, col + 1, 0, -1, false);
			}
		}
	}
	fclose(test_file);


	if(!backtrack(grid, 0))
		printf("No pude resolverlo / algo salio mal\n");
	else
		printf("Termine\n");
	for (int i = 0; i < (grid->cols * grid->rows / 2); i++){
		Cell * pointer = grid->cells[i];
		if (pointer->polarity != 0){
			bool polarity = true;
			bool direccion = true;
			if (pointer->position_x == pointer->other_half_x){
				direccion = false;}
			if (pointer->polarity == -1){
				polarity = false;}
			watcher_set_magnet(pointer->position_x, pointer->position_y, direccion, polarity);}
	}
	watcher_close();
	bonus(grid);
	return 0;
}
