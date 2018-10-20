#include "../watcher/watcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "grid.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Modo de uso: %s test.txt\n", argv[0]);
		return 0;
	}

	FILE* input_file = fopen(argv[1], "r");
	if(!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}
	int width;
	int height;
	fscanf(input_file, "%d %d", &height, &width);

	// Abrimos la interfaz grafica y cargamos el test
	watcher_load_layout(argv[1]);

	// Inicializamos grid

	Grid* grid = newGrid(height, width);

	grid->top_restrictions = malloc(sizeof(int) * width);
	grid->bottom_restrictions = malloc(sizeof(int) * width);
	grid->left_restrictions = malloc(sizeof(int) * height);
	grid->right_restrictions = malloc(sizeof(int) * height);


	// Leemos restricciones
	for (int side = 0; side < 2; side++){
		for (int col = 0; col < width; col++){
			int val;
			fscanf(input_file, "%d ", &val);
			if (val == 0){
				val = -2;}
			if (side == 0){
				grid->top_restrictions[col] = val;}
			else{
				grid->bottom_restrictions[col] = val;}
		}
	}

	for (int side = 2; side < 4; side++){
		for (int row = 0; row < height; row++){
			int val;
			fscanf(input_file, "%d ", &val);
			if (val == 0){
				val = -2;}
			if (side == 2){
				grid->left_restrictions[row] = val;}
			else{
				grid->right_restrictions[row] = val;}
		}
	}
	// Leemos posicionamiento
	/** Leemos las casillas del tablero para poblarlo */
  	for (int row= 0; row< height; row++){
      grid->data[row] = malloc(sizeof(Cell *) * width);}

    for (int row = 0; row < height; row++){
  		for (int col = 0; col < width; col++){
  			char type;
  			fscanf(input_file, "%c ", &type);
  			if (type == 'L'){
          set_values_cell(grid, row, col, 0, 1, true);
          set_values_cell(grid, row, col+1, 0, -1, false);
        }

				else if (type == 'T'){
					set_values_cell(grid, row, col, 1, 0, true);
					set_values_cell(grid, row + 1, col,  -1, 0, false);
				}
  		}
  	}
		fclose(input_file);


	// Llamamos al backtracking


	// Si hay solucion, se la damos al watcher y terminamos

	// Ponemos un sleep para poder ver los cambios en la interfaz
	sleep(3);
	// Cerramos la interfaz (no eliminar esta linea)

	watcher_close();

	/* Free */
	// deleteMatrix(m, height * width);
	// retornamos OK
	return 0;
}
