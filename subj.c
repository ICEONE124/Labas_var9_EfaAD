#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "subj.h"

#define EPS 1e-9

const char * const TypeName[] = {
    "Неизвестный тип", "Скаляр", "Вектор-строка", "Вектор-столбец", "Матрица"
};

typedef struct Scalar {
    Item *next;
    Item *prev;
    TypeObjects type;
    double data;
} Scalar;

typedef struct RowVector {
    Item *next;
    Item *prev;
    TypeObjects type;
    int cols;
    double data[50];
} RowVector;

typedef struct ColVector {
    Item *next;
    Item *prev;
    TypeObjects type;
    int rows;
    double data[50];
} ColVector;

typedef struct Matrix {
    Item *next;
    Item *prev;
    TypeObjects type;
    int rows;
    int cols;
    double data[50][50];
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

void PrintScalar(Scalar *p) { 
    printf("%.4lf\n", p->data); 
}

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
                Item *tmp = Remove(list, j + 1);
                Insert(list, tmp, j);
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

Base* OpScalarScalar(Scalar *l, Scalar *r, ArithmeticOperations op) {
    Scalar *res = (Scalar*)Create(TypeScalar);
    if (!res) return NULL;

    switch (op) {
        case OpPlus:
            res->data = l->data + r->data;
            break;

        case OpMinus:
            res->data = l->data - r->data;
            break;

        case OpMultiply:
            res->data = l->data * r->data;
            break;

        case OpDivide:
            if (fabs(r->data) < EPS) {
                free(res);
                return NULL;
            }
            res->data = l->data / r->data;
            break;

        default:
            free(res);
            return NULL;
    }

    return (Base*)res;
}

Base* OpScalarRowVector(Scalar *l, RowVector *r, ArithmeticOperations op) {
    RowVector *res;

    if (op != OpMultiply) {
        return NULL;
    }

    res = (RowVector*)Create(TypeRowVector);
    if (!res) return NULL;

    res->cols = r->cols;

    for (int i = 0; i < r->cols; i++) {
        res->data[i] = l->data * r->data[i];
    }

    return (Base*)res;
}

Base* OpScalarColVector(Scalar *l, ColVector *r, ArithmeticOperations op) {
    ColVector *res;

    if (op != OpMultiply) {
        return NULL;
    }

    res = (ColVector*)Create(TypeColVector);
    if (!res) return NULL;

    res->rows = r->rows;

    for (int i = 0; i < r->rows; i++) {
        res->data[i] = l->data * r->data[i];
    }

    return (Base*)res;
}

Base* OpScalarMatrix(Scalar *l, Matrix *r, ArithmeticOperations op) {
    Matrix *res;

    if (op != OpMultiply) {
        return NULL;
    }

    res = (Matrix*)Create(TypeMatrix);
    if (!res) return NULL;

    res->rows = r->rows;
    res->cols = r->cols;

    for (int i = 0; i < r->rows; i++) {
        for (int j = 0; j < r->cols; j++) {
            res->data[i][j] = l->data * r->data[i][j];
        }
    }

    return (Base*)res;
}

Base* OpRowVectorRowVector(RowVector *l, RowVector *r, ArithmeticOperations op) {
    if (l->cols != r->cols) {
        return NULL;
    }

    if (op == OpDivide) {
        return NULL;
    }

    if (op == OpPlus || op == OpMinus) {
        RowVector *res = (RowVector*)Create(TypeRowVector);
        if (!res) return NULL;

        res->cols = l->cols;

        for (int i = 0; i < l->cols; i++) {
            if (op == OpPlus) {
                res->data[i] = l->data[i] + r->data[i];
            } else {
                res->data[i] = l->data[i] - r->data[i];
            }
        }

        return (Base*)res;
    }

    if (op == OpMultiply) {
        Scalar *res = (Scalar*)Create(TypeScalar);
        if (!res) return NULL;

        res->data = 0.0;

        for (int i = 0; i < l->cols; i++) {
            res->data += l->data[i] * r->data[i];
        }

        return (Base*)res;
    }

    return NULL;
}

Base* OpColVectorColVector(ColVector *l, ColVector *r, ArithmeticOperations op) {
    if (l->rows != r->rows) {
        return NULL;
    }

    if (op == OpDivide) {
        return NULL;
    }

    if (op == OpPlus || op == OpMinus) {
        ColVector *res = (ColVector*)Create(TypeColVector);
        if (!res) return NULL;

        res->rows = l->rows;
 

        for (int i = 0; i < l->rows; i++) {
            if (op == OpPlus) {
                res->data[i] = l->data[i] + r->data[i];
            } else {
                res->data[i] = l->data[i] - r->data[i];
            }
        }

        return (Base*)res;
    }

    if (op == OpMultiply) {
        Scalar *res = (Scalar*)Create(TypeScalar);
        if (!res) return NULL;

        res->data = 0.0;

        for (int i = 0; i < l->rows; i++) {
            res->data += l->data[i] * r->data[i];
        }

        return (Base*)res;
    }

    return NULL;
}

Base* OpMatrixMatrix(Matrix *l, Matrix *r, ArithmeticOperations op) {
    Matrix *res;

    if (op == OpMultiply) {
        if (l->cols != r->rows) {
            return NULL;
        }

        res = (Matrix*)Create(TypeMatrix);
        if (!res) return NULL;

        res->rows = l->rows;
        res->cols = r->cols;

        for (int i = 0; i < l->rows; i++) {
            for (int j = 0; j < r->cols; j++) {
                res->data[i][j] = 0.0;

                for (int k = 0; k < l->cols; k++) {
                    res->data[i][j] += l->data[i][k] * r->data[k][j];
                }
            }
        }

        return (Base*)res;
    }

    if (op != OpPlus && op != OpMinus) {
        return NULL;
    }

    if (l->rows != r->rows || l->cols != r->cols) {
        return NULL;
    }

    res = (Matrix*)Create(TypeMatrix);
    if (!res) return NULL;

    res->rows = l->rows;
    res->cols = l->cols;

    for (int i = 0; i < l->rows; i++) {
        for (int j = 0; j < l->cols; j++) {
            if (op == OpPlus) {
                res->data[i][j] = l->data[i][j] + r->data[i][j];
            } else {
                res->data[i][j] = l->data[i][j] - r->data[i][j];
            }
        }
    }

    return (Base*)res;
}


Base* CheckScalar(Base *left, Base *right, ArithmeticOperations op) {
    Scalar *l = (Scalar*)left;
    switch (right->type) {
        case TypeScalar:    return OpScalarScalar(l, (Scalar*)right, op);
        case TypeRowVector: return OpScalarRowVector(l, (RowVector*)right, op);
        case TypeColVector: return OpScalarColVector(l, (ColVector*)right, op);
        case TypeMatrix:    return OpScalarMatrix(l, (Matrix*)right, op);
        default: return NULL;
    }
}

Base* CheckRowVector(Base *left, Base *right, ArithmeticOperations op) {
    RowVector *l = (RowVector*)left;
    switch (right->type) {
        case TypeRowVector: 
            if (l->cols != ((RowVector*)right)->cols) return NULL;
            return OpRowVectorRowVector(l, (RowVector*)right, op);
        case TypeColVector: {
            if (op != OpMultiply) return NULL;
            ColVector *r = (ColVector*)right;
            if (l->cols != r->rows) return NULL;
            Scalar *res = (Scalar*)Create(TypeScalar);
            res->data = 0;
            for (int i = 0; i < l->cols; i++) res->data += l->data[i] * r->data[i];
            return (Base*)res;
        }
        default: return NULL;
    }
}

Base* CheckColVector(Base *left, Base *right, ArithmeticOperations op) {
    ColVector *l = (ColVector*)left;
    switch (right->type) {
        case TypeColVector: 
            if (l->rows != ((ColVector*)right)->rows) return NULL;
            return OpColVectorColVector(l, (ColVector*)right, op);
        case TypeRowVector: {
            if (op != OpMultiply) return NULL;
            RowVector *r = (RowVector*)right;
            if (l->rows != r->cols) return NULL;
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = r->cols;
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data[i] * r->data[j];
            return (Base*)res;
        }
        default: return NULL;
    }
}

Base* CheckMatrix(Base *left, Base *right, ArithmeticOperations op) {
    Matrix *l = (Matrix*)left;
    switch (right->type) {
        case TypeScalar: {
            Scalar *r = (Scalar*)right;
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = l->cols;
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < l->cols; j++)
                    res->data[i][j] = (op == OpMultiply) ? (l->data[i][j] * r->data) : (l->data[i][j] / r->data);
            return (Base*)res;
        }
        case TypeMatrix: {
            Matrix *r = (Matrix*)right;
            if (op != OpMultiply && (l->rows != r->rows || l->cols != r->cols)) return NULL;
            if (op == OpMultiply && l->cols != r->rows) return NULL;
            return OpMatrixMatrix(l, r, op);
        }
        default: return NULL;
    }
}

void Math(List *list, int idx1, int idx2, ArithmeticOperations op) {
    Base *b1 = (Base*)GetItem(list, idx1);
    Base *b2 = (Base*)GetItem(list, idx2);
    if (!b1 || !b2) { printf("Элемент не найден.\n"); return; }

    Base *res = NULL;
    switch (b1->type) {
        case TypeScalar:    res = CheckScalar(b1, b2, op); break;
        case TypeRowVector: res = CheckRowVector(b1, b2, op); break;
        case TypeColVector: res = CheckColVector(b1, b2, op); break;
        case TypeMatrix:    res = CheckMatrix(b1, b2, op); break;
    }

    if (res) {
        Add(list, (Item*)res);
        printf("Результат добавлен в конец списка.\n");
    } else {
        printf("Операция невозможна для этих типов.\n");
    }
}