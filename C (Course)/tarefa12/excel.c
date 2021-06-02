#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

// Write matrix with all raw values. Update it only with S commands.

typedef struct _stack {
	int *itens;
	int topo;
} STACK;

STACK *create_stack(void) {
	STACK *stack = malloc(sizeof(STACK));
	(*stack).itens = malloc(100 * sizeof(int));
	(*stack).topo = 0;
	return stack;
}

void push(STACK *stack, int new_val) {
	(*stack).itens[(*stack).topo] = new_val;
	(*stack).topo++;
	return;
}

int pop(STACK *stack) {
	(*stack).topo--;
	return (*stack).itens[(*stack).topo];
}

typedef struct _CELL {
	char col;
	int row;
	struct _CELL *next;
} CELL;

CELL *create_cell(char col, int row) {
	CELL *cell = malloc(sizeof(CELL));
	(*cell).col = col;
	(*cell).row = row;
	(*cell).next = NULL;
	return cell;
}

CELL *append_front(CELL *old, CELL *new) {
	(*new).next = old;
	return new;
}

char *get_cell_raw(CELL *cell, char *file_path, int col_total) {
	if (DEBUG)
		printf("\t\tGetting raw value.\n");
	
	FILE *file = fopen(file_path, "r");
	if (file == NULL)
		exit(1);

	int col_target = (*cell).col - 'A' + 1;
	int row_target = (*cell).row;

	char *c = malloc(2 * sizeof(char));
	memset(c, '\0', 2 * sizeof(char));
	int col;
	int row = 1;
	while (row < row_target) {
		for (col = 1; col <= col_total; col++) {
			fscanf(file, "%c", c);
			while (*c != ',' && *c != '\n')
				fscanf(file, "%c", c);
			if (*c == '\n')
				row++;
		}
	}

	char *raw_value = malloc(100 * sizeof(char));
	memset(raw_value, '\0', 100 * sizeof(char));
	for (col = 1; col <= col_target; col++) {
		if (col < col_target) {
			fscanf(file, "%c", c);
			while (*c != ',')
				fscanf(file, "%c", c);
			continue;
		}

		else {
			fscanf(file, "%c", c);
			while (*c != ',' && *c != '\n') {
				strcat(raw_value, c);
				fscanf(file, "%c", c);
			}
		}
	}

	fclose(file);
	if (DEBUG)
		printf("\t\t\tRaw value: %s\n", raw_value);
	return raw_value;
}

int has_loop(CELL *cell_path) {
	if (DEBUG)
		printf("\tChecking for loop.\n");

	if (cell_path != NULL) {
		CELL *cell_ptr = (*cell_path).next;
		while (cell_ptr != NULL) {
			int same_col = ((*cell_path).col == (*cell_ptr).col);
			int same_row = ((*cell_path).row == (*cell_ptr).row);
			if (same_col && same_row)
				return 1;
			else
				cell_ptr = (*cell_ptr).next;
		}
	}
	return 0;
}

void print_cell_path(CELL *cell_path) {
	CELL *cell_ptr = cell_path;
	printf("\t\t>");
	while (cell_ptr != NULL) {
		printf(" %c%d —", (*cell_ptr).col, (*cell_ptr).row);
		cell_ptr = (*cell_ptr).next;
	}
	printf("|\n");
	return;
}

int evaluate(CELL *cell_path, char *file_path, int col_total, int **mtx_update_check, int **mtx_update_values, int *flag) {
	if (DEBUG) {
		printf("\nEvaluating cell %c%d.\n", (*cell_path).col, (*cell_path).row);
		print_cell_path(cell_path);
	}

	int col = (*cell_path).col - 'A' + 1;
	int row = (*cell_path).row;
	CELL *cell_path_backup = cell_path;

	if (mtx_update_check[col][row]) {
		if (DEBUG)
			printf("\tReturning updated value: %d.\n", mtx_update_values[col][row]);
		return mtx_update_values[col][row];
	}

	else {
		char *raw_value = get_cell_raw(cell_path, file_path, col_total);

		if (raw_value[0] != '=') {
			if (DEBUG)
				printf("\tReturning raw value: %d.\n", atoi(raw_value));
			return atoi(raw_value);
		}

		else if (has_loop(cell_path)) {
			if (DEBUG)
				printf("!!!!!!!!!!!!!!!LOOP\n");
			*flag = 1;
			return 0;
		}

		else {
			if (DEBUG)
				printf("\tEvaluating formula.\n");

			STACK *operations = create_stack();
			STACK *numbers = create_stack();

			for (int index = 2; index < strlen(raw_value); index++) {
				cell_path = cell_path_backup;
				if (raw_value[index] == ' ')
					continue;

				else if (raw_value[index] == '+')
					push(operations, 0);

				else if (raw_value[index] == '-')
					push(operations, 1);

				else if (raw_value[index] == '(')
					continue;

				else if (raw_value[index] == ')') {
					int operation = pop(operations);
					int num1 = pop(numbers);
					int num2 = pop(numbers);
					if (operation == 0) {
						if (DEBUG)
							printf("\t\tPushing (%d) + (%d).\n", num2, num1);
						push(numbers, num2 + num1);
					}
					else {
						if (DEBUG)
							printf("\t\tPushing (%d) - (%d).\n", num2, num1);
						push(numbers, num2 - num1);
					}

				// Retrieves the cell from the text (e.g. 'A123').
				} else {
					char col = raw_value[index]; // e.g. 'A'
					char *row = malloc(4 * sizeof(char)); // e.g. "123\0"
					memset(row, '\0', 4 * sizeof(char));
					char *digit = malloc(2 * sizeof(char));
					memset(digit, '\0', 2 * sizeof(char));
					while (raw_value[++index] != ' ') {
						*digit = raw_value[index];
						strcat(row, digit);
					}
					CELL *new_cell = create_cell(col, atoi(row));
					cell_path = append_front(cell_path, new_cell);
					push(numbers, evaluate(cell_path, file_path, col_total, mtx_update_check, mtx_update_values, flag));
				}
			}
			return pop(numbers);
		}
	}
}

int **create_matrix(int col_total, int row_total) {
	int **mtx = malloc(col_total * sizeof(int *));
	for (int col = 0; col < col_total; col++) {
		mtx[col] = malloc(row_total * sizeof(int));
		for (int row = 0; row < row_total; row++)
			mtx[col][row] = 0;
	}
	return mtx;
}

void update_cell(CELL *cell, int new_value, int **mtx_update_check, int **mtx_update_values) {
	int col = (*cell).col - 'A' + 1;
	int row = (*cell).row;
	mtx_update_check[col][row] = 1;
	mtx_update_values[col][row] = new_value;
	if (DEBUG) {
		printf("Updating cell %c%d to %d. ", (*cell).col, (*cell).row, new_value);	
		printf("Fact: max_update_check[%d][%d] = 1.\n", col, row);
	}
	return;
}

int main(void) {
	char *file_path = malloc(100 * sizeof(char));
	memset(file_path, '\0', 100 * sizeof(char));
	int col_total, row_total;
	scanf("%s %d %d ", file_path, &col_total, &row_total);

	int **mtx_update_check = create_matrix(col_total + 1, row_total + 1);
	int **mtx_update_values = create_matrix(col_total + 1, row_total + 1);

	char action, col;
	char *row = malloc (4 * sizeof(char));
	memset(row, '\0', 4 * sizeof(char));
	int new_value, flag;
	while (scanf("%c ", &action) != EOF) {
			flag = 0;
			scanf("%c", &col);
			scanf("%s ", row);
			printf("%c%s: ", col, row);
			CELL *cell = create_cell(col, atoi(row));
		if (action == 'G') {
			int result = evaluate(cell, file_path, col_total, mtx_update_check, mtx_update_values, &flag);
			if (!flag)
				printf("%d\n", result);
			else
				printf("#ERRO#\n");
		} else {
			int result = evaluate(cell, file_path, col_total, mtx_update_check, mtx_update_values, &flag);
			if (!flag) {
				printf("%d", result);
				scanf("%d ", &new_value);
				printf(" -> %d\n", new_value);
				update_cell(cell, new_value, mtx_update_check, mtx_update_values);
			} else
				printf("#ERRO#\n");
		}
		if (DEBUG)
			printf("\n");
	}
	
	return 0;
}