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
	Grid * grid = _newGrid(argv[1]);




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
