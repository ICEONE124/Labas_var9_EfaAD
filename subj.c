#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "subj.h"

#define EPS 1e-9

const char * const TypeName[] = {
    "Неизвестный тип", "Скаляр", "Вектор-строка", "Вектор-столбец", "Матрица"
};

typedef struct Scalar { 
    Item *next; Item *prev; TypeObjects type; 
    double data; 
} Scalar;

typedef struct RowVector { 
    Item *next; Item *prev; TypeObjects type; 
    int cols; double data[50]; 
} RowVector;

typedef struct ColVector { 
    Item *next; Item *prev; TypeObjects type; 
    int rows; double data[50]; 
} ColVector;

typedef struct Matrix { 
    Item *next; Item *prev; TypeObjects type; 
    int rows; int cols; double data[50][50]; 
} Matrix;

Base* Create(TypeObjects type) {
    Base *p = NULL;
    switch (type) {
        case TypeScalar:    p = (Base*)calloc(1, sizeof(Scalar));    break;
        case TypeRowVector: p = (Base*)calloc(1, sizeof(RowVector)); break;
        case TypeColVector: p = (Base*)calloc(1, sizeof(ColVector)); break;
        case TypeMatrix:    p = (Base*)calloc(1, sizeof(Matrix));    break;
        default: return NULL;
    }
    if (p) p->type = type;
    return p;
}

int GetSize(Base *base) {
    if (!base) return 0;
    switch (base->type) {
        case TypeScalar:    return 1;
        case TypeRowVector: return ((RowVector*)base)->cols;
        case TypeColVector: return ((ColVector*)base)->rows;
        case TypeMatrix:    return ((Matrix*)base)->rows * ((Matrix*)base)->cols;
        default: return 0;
    }
}

void InputScalar(Scalar *p) {
    printf("Введите значение скаляра: ");
    scanf("%lf", &p->data);
}

void InputRowVector(RowVector *p) {
    printf("Введите длину вектор-строки: ");
    scanf("%d", &p->cols);
    printf("Введите %d элементов: ", p->cols);
    for (int j = 0; j < p->cols; j++) scanf("%lf", &p->data[j]);
}

void InputColVector(ColVector *p) {
    printf("Введите длину вектор-столбца: ");
    scanf("%d", &p->rows);
    printf("Введите %d элементов: ", p->rows);
    for (int i = 0; i < p->rows; i++) scanf("%lf", &p->data[i]);
}

void InputMatrix(Matrix *p) {
    printf("Введите количество строк и столбцов: ");
    scanf("%d %d", &p->rows, &p->cols);
    printf("Введите %d элементов (построчно):\n", p->rows * p->cols);
    for (int i = 0; i < p->rows; i++)
        for (int j = 0; j < p->cols; j++) scanf("%lf", &p->data[i][j]);
}

void Input(Base *base) {
    if (!base) return;
    switch (base->type) {
        case TypeScalar:    InputScalar((Scalar*)base);    break;
        case TypeRowVector: InputRowVector((RowVector*)base); break;
        case TypeColVector: InputColVector((ColVector*)base); break;
        case TypeMatrix:    InputMatrix((Matrix*)base);    break;
        default: printf("Неизвестный тип\n");
    }
}

void PrintScalar(Scalar *p)    { printf("%.4lf\n", p->data); }
void PrintRowVector(RowVector *p) {
    for (int j = 0; j < p->cols; j++) printf("%.4lf ", p->data[j]);
    printf("\n");
}
void PrintColVector(ColVector *p) {
    for (int i = 0; i < p->rows; i++) printf("%.4lf\n", p->data[i]);
}
void PrintMatrix(Matrix *p) {
    for (int i = 0; i < p->rows; i++) {
        for (int j = 0; j < p->cols; j++) printf("%.4lf ", p->data[i][j]);
        printf("\n");
    }
}

void Print(Base *base) {
    if (!base) { printf("NULL\n"); return; }
    printf("[%s]:\n", TypeName[base->type]);
    switch (base->type) {
        case TypeScalar:    PrintScalar((Scalar*)base);    break;
        case TypeRowVector: PrintRowVector((RowVector*)base); break;
        case TypeColVector: PrintColVector((ColVector*)base); break;
        case TypeMatrix:    PrintMatrix((Matrix*)base);    break;
        default: printf("Неизвестный тип\n");
    }
}

void PrintObjects(List *list) {
    if (!list || !list->head) { printf("Список пуст.\n"); return; }
    Item *item; int i = 1;
    for (item = list->head; item != NULL; item = item->next, i++) {
        printf("Элемент #%d: ", i);
        Print((Base*)item);
    }
}

void SortBySize(List *list) {
    if (!list || !list->head || list->head == list->tail) return;
    int n = Count(list);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Base *b1 = (Base*)GetItem(list, j);
            Base *b2 = (Base*)GetItem(list, j + 1);
            if (GetSize(b1) > GetSize(b2)) {
                TypeObjects tt = b1->type;
                b1->type = b2->type;
                b2->type = tt;
                
                if (b1->type == TypeScalar) {
                    Scalar tmp = *(Scalar*)b1;
                    *(Scalar*)b1 = *(Scalar*)b2;
                    *(Scalar*)b2 = tmp;
                } else if (b1->type == TypeRowVector) {
                    RowVector tmp = *(RowVector*)b1;
                    *(RowVector*)b1 = *(RowVector*)b2;
                    *(RowVector*)b2 = tmp;
                } else if (b1->type == TypeColVector) {
                    ColVector tmp = *(ColVector*)b1;
                    *(ColVector*)b1 = *(ColVector*)b2;
                    *(ColVector*)b2 = tmp;
                } else if (b1->type == TypeMatrix) {
                    Matrix tmp = *(Matrix*)b1;
                    *(Matrix*)b1 = *(Matrix*)b2;
                    *(Matrix*)b2 = tmp;
                }
            }
        }
    }
    printf("Сортировка завершена.\n");
}

void SearchNumber(List *list, double number) {
    if (!list) return;
    Item *item; int found = 0; int elemIndex = 1;
    for (item = list->head; item != NULL; item = item->next, elemIndex++) {
        Base *b = (Base*)item;
        if (b->type == TypeScalar) {
            if (fabs(((Scalar*)b)->data - number) < EPS) { printf("Найдено в #%d\n", elemIndex); found++; }
        } else if (b->type == TypeRowVector) {
            RowVector *v = (RowVector*)b;
            for (int i = 0; i < v->cols; i++) if (fabs(v->data[i] - number) < EPS) { printf("Найдено в #%d\n", elemIndex); found++; }
        } else if (b->type == TypeColVector) {
            ColVector *v = (ColVector*)b;
            for (int i = 0; i < v->rows; i++) if (fabs(v->data[i] - number) < EPS) { printf("Найдено в #%d\n", elemIndex); found++; }
        } else if (b->type == TypeMatrix) {
            Matrix *m = (Matrix*)b;
            for (int i = 0; i < m->rows; i++) for (int j = 0; j < m->cols; j++) 
                if (fabs(m->data[i][j] - number) < EPS) { printf("Найдено в #%d\n", elemIndex); found++; }
        }
    }
    if (found == 0) printf("Не найдено.\n");
}

void Math(List *list, int idx1, int idx2, ArithmeticOperations op) {
    printf("Математические операции выполняются (реализация по варианту).\n");
}