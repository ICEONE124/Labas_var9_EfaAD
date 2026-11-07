#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"


void Add(List *list, Item *item) {
	if (list == NULL || item == NULL) return; 

	item->next = NULL; 
	item->prev = NULL;

	if (list->head == NULL) {
		list->head = item;
		list->tail = item;
	} else {
		item->prev = list->tail;
		list->tail->next = item;
		list->tail = item;
	}
}

Item* GetItem(const List *list, const int n) {
	if (list == NULL || n < 0) return NULL; 
	
	Item *item = NULL;
	int i;
	for (i = 0, item = list->head; item != NULL && i < n; i++) {
		item = item->next;
	}
	return item;
}

Item* Remove(List *list, const int n) {
	if (list == NULL) return NULL; 

	Item *item = GetItem(list, n);
	if (item != NULL) {
		if (list->head == list->tail) {
			list->head = NULL;
			list->tail = NULL;
		} else {
			if (item == list->head) {
				list->head = item->next;
				if (list->head != NULL) list->head->prev = NULL;
			} else if (item == list->tail) {
				list->tail = item->prev;
				if (list->tail != NULL) list->tail->next = NULL;
			} else {
				if (item->next != NULL) item->next->prev = item->prev;
				if (item->prev != NULL) item->prev->next = item->next;
			}
		}
		item->next = NULL;
		item->prev = NULL;
		return item;
	}
	return NULL;
}

void Delete(List *list, const int n) {
	if (list == NULL) return; 

	Item *item = Remove(list, n);
	if (item == NULL) {
		printf("Объедок не найден\n");
	} else {
		FreeItemData(item);
		free(item);
	}
}

int Count(const List *list) {
	if (list == NULL) return 0; 

	Item *item;
	int n;
	for (n = 0, item = list->head; item != NULL; item = item->next) {
		n++;
	}
	return n;
}

void Clear(List *list) {
	if (list == NULL) return; 

	while (list->head != NULL) {
		Delete(list, 0);
	}
}

int GetIndex(const List *list, const Item *item) {
	if (list == NULL || item == NULL) return -1; 

	Item *i;
	int number;
	for (number = 0, i = list->head; i != NULL; i = i->next) {
		if (i == item) {
			return number;
		}
		number++;
	}
	return -1;
}

void Insert(List *list, Item *item, const int n) {
	if (list == NULL || item == NULL) return; 

	item->next = NULL; 
	item->prev = NULL;

	Item *old_item = GetItem(list, n);
	if (old_item != NULL) {
		if (old_item == list->head) {
			list->head = item;
			old_item->prev = item;
			item->next = old_item;
		} else {
			item->next = old_item;
			item->prev = old_item->prev;
			if (old_item->prev != NULL) {
				old_item->prev->next = item;
			}
			old_item->prev = item;
		}
	} else {
		Add(list, item);
	}
}



Item* CreateScalar(double val) {
	Item *item = (Item*)calloc(1, sizeof(Item));
	if (item == NULL) return NULL; 

	item->next = NULL; 
	item->prev = NULL;
	item->type = TYPE_SCALAR;
	item->rows = 1;
	item->cols = 1;

	item->data = (double**)malloc(sizeof(double*));
	if (item->data == NULL) {
		free(item);
		return NULL;
	}
	item->data[0] = (double*)malloc(sizeof(double));
	if (item->data[0] == NULL) {
		free(item->data);
		free(item);
		return NULL;
	}

	item->data[0][0] = val;
	return item;
}

Item* CreateVectorRow(int cols, const double *arr) {
	if (cols <= 0) return NULL;

	Item *item = (Item*)calloc(1, sizeof(Item));
	if (item == NULL) return NULL;

	item->next = NULL;
	item->prev = NULL;
	item->type = TYPE_ROW_VECTOR;
	item->rows = 1;
	item->cols = cols;

	item->data = (double**)malloc(sizeof(double*));
	if (item->data == NULL) {
		free(item);
		return NULL;
	}
	item->data[0] = (double*)malloc(cols * sizeof(double));
	if (item->data[0] == NULL) {
		free(item->data);
		free(item);
		return NULL;
	}

	int j;
	for (j = 0; j < cols; j++) {
		item->data[0][j] = arr ? arr[j] : 0.0;
	}
	return item;
}

Item* CreateVectorCol(int rows, const double *arr) {
	if (rows <= 0) return NULL;

	Item *item = (Item*)calloc(1, sizeof(Item));
	if (item == NULL) return NULL;

	item->next = NULL;
	item->prev = NULL;
	item->type = TYPE_COL_VECTOR;
	item->rows = rows;
	item->cols = 1;

	item->data = (double**)malloc(rows * sizeof(double*));
	if (item->data == NULL) {
		free(item);
		return NULL;
	}

	int i;
	for (i = 0; i < rows; i++) {
		item->data[i] = (double*)malloc(sizeof(double));
		if (item->data[i] == NULL) {
			for (int k = 0; k < i; k++) free(item->data[k]);
			free(item->data);
			free(item);
			return NULL;
		}
		item->data[i][0] = arr ? arr[i] : 0.0;
	}
	return item;
}

Item* CreateMatrix(int rows, int cols, double **arr) {
	if (rows <= 0 || cols <= 0) return NULL;

	Item *item = (Item*)calloc(1, sizeof(Item));
	if (item == NULL) return NULL;

	item->next = NULL;
	item->prev = NULL;
	item->type = (rows == 1 && cols == 1) ? TYPE_SCALAR :
	             (rows == 1) ? TYPE_ROW_VECTOR :
	             (cols == 1) ? TYPE_COL_VECTOR : TYPE_MATRIX;
	item->rows = rows;
	item->cols = cols;

	item->data = (double**)malloc(rows * sizeof(double*));
	if (item->data == NULL) {
		free(item);
		return NULL;
	}

	int i, j;
	for (i = 0; i < rows; i++) {
		item->data[i] = (double*)malloc(cols * sizeof(double));
		if (item->data[i] == NULL) {
			for (int k = 0; k < i; k++) free(item->data[k]);
			free(item->data);
			free(item);
			return NULL;
		}
		for (j = 0; j < cols; j++) {
			item->data[i][j] = arr ? arr[i][j] : 0.0;
		}
	}
	return item;
}

void FreeItemData(Item *item) {
	if (item == NULL || item->data == NULL) return; 
	int i;
	for (i = 0; i < item->rows; i++) {
		if (item->data[i] != NULL) {
			free(item->data[i]);
		}
	}
	free(item->data);
	item->data = NULL;
}



static int GetSize(const Item *item) {
	if (item == NULL) return 0;
	return item->rows * item->cols;
}

void SortBySize(List *list) {
	if (list == NULL || list->head == NULL || list->head == list->tail) return; 

	Item *i, *j, *min_item;
	int size_j, min_size;

	for (i = list->head; i != NULL; i = i->next) {
		min_item = i;
		min_size = GetSize(i);
		for (j = i->next; j != NULL; j = j->next) {
			size_j = GetSize(j);
			if (size_j < min_size) {
				min_size = size_j;
				min_item = j;
			}
		}
		if (min_item != i) {
			ObjectType t_type = i->type;
			int t_rows = i->rows;
			int t_cols = i->cols;
			double **t_data = i->data;

			i->type = min_item->type;
			i->rows = min_item->rows;
			i->cols = min_item->cols;
			i->data = min_item->data;

			min_item->type = t_type;
			min_item->rows = t_rows;
			min_item->cols = t_cols;
			min_item->data = t_data;
		}
	}
}

void SearchInList(const List *list, double target, double eps) {
	if (list == NULL) return; 

	Item *item;
	int idx, i, j;
	int found = 0;

	for (idx = 0, item = list->head; item != NULL; item = item->next, idx++) {
		if (item->data == NULL) continue;
		for (i = 0; i < item->rows; i++) {
			if (item->data[i] == NULL) continue;
			for (j = 0; j < item->cols; j++) {
				if (fabs(item->data[i][j] - target) < eps) {
					printf("Найдено в элементе №%d [%d][%d]: %.2f\n", idx, i, j, item->data[i][j]);
					found = 1;
				}
			}
		}
	}
	if (!found) {
		printf("Число %.2f в элементах списка не найдено.\n", target);
	}
}

int AddElements(List *list, const int n1, const int n2) {
	if (list == NULL) return 0;

	Item *a = GetItem(list, n1);
	Item *b = GetItem(list, n2);

	if (a == NULL || b == NULL || a->rows != b->rows || a->cols != b->cols) {
		return 0;
	}

	Item *res = CreateMatrix(a->rows, a->cols, NULL);
	if (res == NULL) return 0;

	int i, j;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->cols; j++) {
			res->data[i][j] = a->data[i][j] + b->data[i][j];
		}
	}
	Add(list, res);
	return 1;
}

int SubtractElements(List *list, const int n1, const int n2) {
	if (list == NULL) return 0;

	Item *a = GetItem(list, n1);
	Item *b = GetItem(list, n2);

	if (a == NULL || b == NULL || a->rows != b->rows || a->cols != b->cols) {
		return 0;
	}

	Item *res = CreateMatrix(a->rows, a->cols, NULL);
	if (res == NULL) return 0;

	int i, j;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->cols; j++) {
			res->data[i][j] = a->data[i][j] - b->data[i][j];
		}
	}
	Add(list, res);
	return 1;
}

int MultiplyElements(List *list, const int n1, const int n2) {
	if (list == NULL) return 0;

	Item *a = GetItem(list, n1);
	Item *b = GetItem(list, n2);

	if (a == NULL || b == NULL) return 0;

	if (a->type == TYPE_SCALAR) {
		Item *res = CreateMatrix(b->rows, b->cols, NULL);
		if (res == NULL) return 0;
		int i, j;
		for (i = 0; i < b->rows; i++) {
			for (j = 0; j < b->cols; j++) {
				res->data[i][j] = a->data[0][0] * b->data[i][j];
			}
		}
		Add(list, res);
		return 1;
	}

	if (a->cols != b->rows) return 0;

	Item *res = CreateMatrix(a->rows, b->cols, NULL);
	if (res == NULL) return 0;

	int i, j, k;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < b->cols; j++) {
			res->data[i][j] = 0;
			for (k = 0; k < a->cols; k++) {
				res->data[i][j] += a->data[i][k] * b->data[k][j];
			}
		}
	}
	Add(list, res);
	return 1;
}

int DivideElements(List *list, const int n1, const int n2) {
	if (list == NULL) return 0;

	Item *a = GetItem(list, n1);
	Item *b = GetItem(list, n2);

	if (a == NULL || b == NULL || b->type != TYPE_SCALAR) return 0;
	if (fabs(b->data[0][0]) < 1e-9) return 0;

	Item *res = CreateMatrix(a->rows, a->cols, NULL);
	if (res == NULL) return 0;

	int i, j;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->cols; j++) {
			res->data[i][j] = a->data[i][j] / b->data[0][0];
		}
	}
	Add(list, res);
	return 1;
}