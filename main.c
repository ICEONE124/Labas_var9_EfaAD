#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void PrintList(const List *list) {
	Item *i;
	int n = 0;
	int r, c;
	
	printf("List: %p, Head: %p, Tail: %p\n", list, list->head, list->tail);
	printf("#\ttype\t\tsize\tvalues\n");
	for (i = list->head, n = 0; i != NULL; i = i->next, n++) {
		printf("%d\t", n);
		switch (i->type) {
			case TYPE_SCALAR: printf("Scalar\t\t"); break;
			case TYPE_ROW_VECTOR: printf("RowVector\t"); break;
			case TYPE_COL_VECTOR: printf("ColVector\t"); break;
			case TYPE_MATRIX: printf("Matrix\t\t"); break;
		}
		printf("%dx%d\t", i->rows, i->cols);
		
		for (r = 0; r < i->rows; r++) {
			if (r > 0) printf("\n\t\t\t\t");
			for (c = 0; c < i->cols; c++) {
				printf("%.2f ", i->data[r][c]);
			}
		}
		printf("\n");
	}
}

Item* InputItemFromUser() {
	int type;
	printf("Выберите тип объекта (0 - Скаляр, 1 - Вектор-строка, 2 - Вектор-столбец, 3 - Матрица): ");
	if (scanf("%d", &type) != 1 || type < 0 || type > 3) {
		printf("Некорректный тип!\n");
		return NULL;
	}

	if (type == 0) {
		double val;
		printf("Введите число: ");
		scanf("%lf", &val);
		return CreateScalar(val);
	} else if (type == 1) {
		int cols, j;
		printf("Введите длину вектора-строки: ");
		scanf("%d", &cols);
		double *arr = (double*)malloc(cols * sizeof(double));
		for (j = 0; j < cols; j++) {
			printf("[%d]: ", j);
			scanf("%lf", &arr[j]);
		}
		Item *item = CreateVectorRow(cols, arr);
		free(arr);
		return item;
	} else if (type == 2) {
		int rows, i;
		printf("Введите длину вектора-столбца: ");
		scanf("%d", &rows);
		double *arr = (double*)malloc(rows * sizeof(double));
		for (i = 0; i < rows; i++) {
			printf("[%d]: ", i);
			scanf("%lf", &arr[i]);
		}
		Item *item = CreateVectorCol(rows, arr);
		free(arr);
		return item;
	} else {
		int rows, cols, i, j;
		printf("Введите количество строк и столбцов: ");
		scanf("%d %d", &rows, &cols);
		double **arr = (double**)malloc(rows * sizeof(double*));
		for (i = 0; i < rows; i++) {
			arr[i] = (double*)malloc(cols * sizeof(double));
			for (j = 0; j < cols; j++) {
				printf("[%d][%d]: ", i, j);
				scanf("%lf", &arr[i][j]);
			}
		}
		Item *item = CreateMatrix(rows, cols, arr);
		for (i = 0; i < rows; i++) free(arr[i]);
		free(arr);
		return item;
	}
}

int main() {
	system("chcp 65001 > nul");
	
	List list = {NULL, NULL};
	
	int key;
	int stop = 1;
	int n, n1, n2;
	double target;
	Item *item;
	
	while (stop) {
//		fflush(stdin);
		printf("Выберите операцию:"
				"\n0 - Выход"
				"\n1 - Добавить элемент в конец списка"
				"\n2 - Удалить элемент"
				"\n3 - Вставить элемент"
				"\n4 - Вывести количество элементов"
				"\n5 - Очистить список"
				"\n6 - Вывести список"
				"\n7 - Сортировать элементы по размеру"
				"\n8 - Найти число в элементах"
				"\n9 - Сложение элементов (n1 + n2)"
				"\n10 - Вычитание элементов (n1 - n2)"
				"\n11 - Умножение элементов (n1 * n2)"
				"\n12 - Деление элементов (n1 / n2)"
				"\n-> ");	
		if (scanf("%d", &key) != 1) {
			printf("Данные введены некорректно!\n\n");
			continue;
		}
		switch (key) {
			case 0:
				stop = 0;
				break;
			case 1:
				item = InputItemFromUser();
				if (item != NULL) Add(&list, item);
				break;
			case 2:
				printf("Введите номер объекта: ");
				scanf("%d", &n);
				Delete(&list, n);
				break;
			case 3:
				printf("Введите номер позиции: ");
				scanf("%d", &n);
				item = InputItemFromUser();
				if (item != NULL) Insert(&list, item, n);
				break;
			case 4:
				printf("Количество элементов: %d\n", Count(&list));
				break;
			case 5:
				Clear(&list);
				break;
			case 6:
				PrintList(&list);
				break;
			case 7:
				SortBySize(&list);
				printf("Список отсортирован по размеру элементов.\n");
				break;
			case 8:
				printf("Введите число для поиска: ");
				scanf("%lf", &target);
				SearchInList(&list, target, 1e-6);
				break;
			case 9:
				printf("Введите номера двух элементов (n1 n2): ");
				scanf("%d %d", &n1, &n2);
				if (!AddElements(&list, n1, n2)) printf("Операция неправомерна!\n");
				break;
			case 10:
				printf("Введите номера двух элементов (n1 n2): ");
				scanf("%d %d", &n1, &n2);
				if (!SubtractElements(&list, n1, n2)) printf("Операция неправомерна!\n");
				break;
			case 11:
				printf("Введите номера двух элементов (n1 n2): ");
				scanf("%d %d", &n1, &n2);
				if (!MultiplyElements(&list, n1, n2)) printf("Операция неправомерна!\n");
				break;
			case 12:
				printf("Введите номера двух элементов (n1 n2): ");
				scanf("%d %d", &n1, &n2);
				if (!DivideElements(&list, n1, n2)) printf("Операция неправомерна!\n");
				break;
		}
		printf("\n");
	}
	
	Clear(&list);
	
	system("pause");
	
	return 0;
}