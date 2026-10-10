#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "subj.h"

#define EPS 1e-9

const char * const TypeName[] = {
    "Неизвестный тип", "Скаляр", "Вектор-строка", "Вектор-столбец", "Матрица"
};

typedef struct Scalar    { Item *next; Item *prev; TypeObjects type; int rows; int cols; double **data; } Scalar;
typedef struct RowVector { Item *next; Item *prev; TypeObjects type; int rows; int cols; double **data; } RowVector;
typedef struct ColVector { Item *next; Item *prev; TypeObjects type; int rows; int cols; double **data; } ColVector;
typedef struct Matrix    { Item *next; Item *prev; TypeObjects type; int rows; int cols; double **data; } Matrix;

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

void FreeBase(Base *base) {
    if (!base) return;
    if (base->data) {
        for (int i = 0; i < base->rows; i++)
            if (base->data[i]) free(base->data[i]);
        free(base->data);
    }
    free(base);
}

void InputScalar(Scalar *p) {
    p->rows = 1; p->cols = 1;
    p->data = (double**)malloc(sizeof(double*));
    p->data[0] = (double*)malloc(sizeof(double));
    printf("Введите значение скаляра: ");
    scanf("%lf", &p->data[0][0]);
}

void InputRowVector(RowVector *p) {
    p->rows = 1;
    printf("Введите длину вектор-строки: ");
    scanf("%d", &p->cols);
    p->data = (double**)malloc(sizeof(double*));
    p->data[0] = (double*)malloc(p->cols * sizeof(double));
    printf("Введите %d элементов: ", p->cols);
    for (int j = 0; j < p->cols; j++) scanf("%lf", &p->data[0][j]);
}

void InputColVector(ColVector *p) {
    p->cols = 1;
    printf("Введите длину вектор-столбца: ");
    scanf("%d", &p->rows);
    p->data = (double**)malloc(p->rows * sizeof(double*));
    for (int i = 0; i < p->rows; i++) p->data[i] = (double*)malloc(sizeof(double));
    printf("Введите %d элементов: ", p->rows);
    for (int i = 0; i < p->rows; i++) scanf("%lf", &p->data[i][0]);
}

void InputMatrix(Matrix *p) {
    printf("Введите количество строк и столбцов: ");
    scanf("%d %d", &p->rows, &p->cols);
    p->data = (double**)malloc(p->rows * sizeof(double*));
    for (int i = 0; i < p->rows; i++) p->data[i] = (double*)malloc(p->cols * sizeof(double));
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

void PrintScalar(Scalar *p)    { printf("%.4lf\n", p->data[0][0]); }
void PrintRowVector(RowVector *p) {
    for (int j = 0; j < p->cols; j++) printf("%.4lf ", p->data[0][j]);
    printf("\n");
}
void PrintColVector(ColVector *p) {
    for (int i = 0; i < p->rows; i++) printf("%.4lf\n", p->data[i][0]);
}
void PrintMatrix(Matrix *p) {
    for (int i = 0; i < p->rows; i++) {
        for (int j = 0; j < p->cols; j++) printf("%.4lf ", p->data[i][j]);
        printf("\n");
    }
}

void Print(Base *base) {
    if (!base) { printf("NULL\n"); return; }
    printf("[%s %dx%d]:\n", TypeName[base->type], base->rows, base->cols);
    switch (base->type) {
        case TypeScalar:    PrintScalar((Scalar*)base);    break;
        case TypeRowVector: PrintRowVector((RowVector*)base); break;
        case TypeColVector: PrintColVector((ColVector*)base); break;
        case TypeMatrix:    PrintMatrix((Matrix*)base);    break;
        default: printf("Неизвестный тип\n");
    }
}

void PrintList(List *list) {
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
            if (b1->rows * b1->cols > b2->rows * b2->cols) {
                TypeObjects tt = b1->type; int tr = b1->rows; int tc = b1->cols; double **td = b1->data;
                b1->type = b2->type; b1->rows = b2->rows; b1->cols = b2->cols; b1->data = b2->data;
                b2->type = tt; b2->rows = tr; b2->cols = tc; b2->data = td;
            }
        }
    }
    printf("Сортировка завершена.\n");
}

// --- Поиск числа ---
void SearchNumber(List *list, double number) {
    if (!list) return;
    Item *item; int found = 0; int elemIndex = 1;
    for (item = list->head; item != NULL; item = item->next, elemIndex++) {
        Base *b = (Base*)item;
        if (!b->data) continue;
        for (int i = 0; i < b->rows; i++)
            for (int j = 0; j < b->cols; j++)
                if (fabs(b->data[i][j] - number) < EPS) {
                    printf("Число %.4lf найдено в элементе #%d (%s %dx%d) на позиции [%d][%d].\n",
                           number, elemIndex, TypeName[b->type], b->rows, b->cols, i, j);
                    found++;
                }
    }
    if (found == 0) printf("Число %.4lf не найдено ни в одном элементе списка.\n", number);
    else printf("Всего найдено вхождений: %d\n", found);
}

Base* OpScalarScalar(Scalar *l, Scalar *r, ArithmeticOperations op) {
    Scalar *res = (Scalar*)Create(TypeScalar);
    res->rows = 1; res->cols = 1;
    res->data = (double**)malloc(sizeof(double*));
    res->data[0] = (double*)malloc(sizeof(double));
    switch (op) {
        case OpPlus:     res->data[0][0] = l->data[0][0] + r->data[0][0]; break;
        case OpMinus:    res->data[0][0] = l->data[0][0] - r->data[0][0]; break;
        case OpMultiply: res->data[0][0] = l->data[0][0] * r->data[0][0]; break;
        case OpDivide:
            if (fabs(r->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
            res->data[0][0] = l->data[0][0] / r->data[0][0];
            break;
        default: FreeBase((Base*)res); return NULL;
    }
    return (Base*)res;
}

Base* OpScalarRowVector(Scalar *l, RowVector *r, ArithmeticOperations op) {
    RowVector *res = (RowVector*)Create(TypeRowVector);
    res->rows = 1; res->cols = r->cols;
    res->data = (double**)malloc(sizeof(double*));
    res->data[0] = (double*)malloc(r->cols * sizeof(double));
    switch (op) {
        case OpPlus:
        case OpMinus:
            printf("Ошибка: скаляр и вектор-строка несовместимы для +/-. \n");
            FreeBase((Base*)res); return NULL;
        case OpMultiply:
            for (int i = 0; i < r->cols; i++) res->data[0][i] = l->data[0][0] * r->data[0][i];
            break;
        case OpDivide:
            if (fabs(l->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
            for (int i = 0; i < r->cols; i++) {
                if (fabs(r->data[0][i]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
                res->data[0][i] = l->data[0][0] / r->data[0][i];
            }
            break;
        default: FreeBase((Base*)res); return NULL;
    }
    return (Base*)res;
}

Base* OpScalarColVector(Scalar *l, ColVector *r, ArithmeticOperations op) {
    ColVector *res = (ColVector*)Create(TypeColVector);
    res->rows = r->rows; res->cols = 1;
    res->data = (double**)malloc(r->rows * sizeof(double*));
    for (int i = 0; i < r->rows; i++) res->data[i] = (double*)malloc(sizeof(double));
    switch (op) {
        case OpPlus:
        case OpMinus:
            printf("Ошибка: скаляр и вектор-столбец несовместимы для +/-. \n");
            FreeBase((Base*)res); return NULL;
        case OpMultiply:
            for (int i = 0; i < r->rows; i++) res->data[i][0] = l->data[0][0] * r->data[i][0];
            break;
        case OpDivide:
            if (fabs(l->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
            for (int i = 0; i < r->rows; i++) {
                if (fabs(r->data[i][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
                res->data[i][0] = l->data[0][0] / r->data[i][0];
            }
            break;
        default: FreeBase((Base*)res); return NULL;
    }
    return (Base*)res;
}

Base* OpScalarMatrix(Scalar *l, Matrix *r, ArithmeticOperations op) {
    Matrix *res = (Matrix*)Create(TypeMatrix);
    res->rows = r->rows; res->cols = r->cols;
    res->data = (double**)malloc(r->rows * sizeof(double*));
    for (int i = 0; i < r->rows; i++) res->data[i] = (double*)malloc(r->cols * sizeof(double));
    switch (op) {
        case OpPlus:
        case OpMinus:
            printf("Ошибка: скаляр и матрица несовместимы для +/-. \n");
            FreeBase((Base*)res); return NULL;
        case OpMultiply:
            for (int i = 0; i < r->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data[0][0] * r->data[i][j];
            break;
        case OpDivide:
            if (fabs(l->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
            for (int i = 0; i < r->rows; i++)
                for (int j = 0; j < r->cols; j++) {
                    if (fabs(r->data[i][j]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
                    res->data[i][j] = l->data[0][0] / r->data[i][j];
                }
            break;
        default: FreeBase((Base*)res); return NULL;
    }
    return (Base*)res;
}

Base* OpRowVectorRowVector(RowVector *l, RowVector *r, ArithmeticOperations op) {
    switch (op) {
        case OpPlus: {
            RowVector *res = (RowVector*)Create(TypeRowVector);
            res->rows = 1; res->cols = l->cols;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(l->cols * sizeof(double));
            for (int i = 0; i < l->cols; i++) res->data[0][i] = l->data[0][i] + r->data[0][i];
            return (Base*)res;
        }
        case OpMinus: {
            RowVector *res = (RowVector*)Create(TypeRowVector);
            res->rows = 1; res->cols = l->cols;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(l->cols * sizeof(double));
            for (int i = 0; i < l->cols; i++) res->data[0][i] = l->data[0][i] - r->data[0][i];
            return (Base*)res;
        }
        case OpMultiply: {
            Scalar *res = (Scalar*)Create(TypeScalar);
            res->rows = 1; res->cols = 1;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(sizeof(double));
            res->data[0][0] = 0;
            for (int i = 0; i < l->cols; i++) res->data[0][0] += l->data[0][i] * r->data[0][i];
            return (Base*)res;
        }
        case OpDivide:
            printf("Ошибка: деление векторов не определено.\n");
            return NULL;
        default: return NULL;
    }
}

Base* OpColVectorColVector(ColVector *l, ColVector *r, ArithmeticOperations op) {
    switch (op) {
        case OpPlus: {
            ColVector *res = (ColVector*)Create(TypeColVector);
            res->rows = l->rows; res->cols = 1;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(sizeof(double));
            for (int i = 0; i < l->rows; i++) res->data[i][0] = l->data[i][0] + r->data[i][0];
            return (Base*)res;
        }
        case OpMinus: {
            ColVector *res = (ColVector*)Create(TypeColVector);
            res->rows = l->rows; res->cols = 1;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(sizeof(double));
            for (int i = 0; i < l->rows; i++) res->data[i][0] = l->data[i][0] - r->data[i][0];
            return (Base*)res;
        }
        case OpMultiply: {
            Scalar *res = (Scalar*)Create(TypeScalar);
            res->rows = 1; res->cols = 1;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(sizeof(double));
            res->data[0][0] = 0;
            for (int i = 0; i < l->rows; i++) res->data[0][0] += l->data[i][0] * r->data[i][0];
            return (Base*)res;
        }
        case OpDivide:
            printf("Ошибка: деление векторов не определено.\n");
            return NULL;
        default: return NULL;
    }
}

Base* OpMatrixMatrix(Matrix *l, Matrix *r, ArithmeticOperations op) {
    switch (op) {
        case OpPlus: {
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = l->cols;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(l->cols * sizeof(double));
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < l->cols; j++)
                    res->data[i][j] = l->data[i][j] + r->data[i][j];
            return (Base*)res;
        }
        case OpMinus: {
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = l->cols;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(l->cols * sizeof(double));
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < l->cols; j++)
                    res->data[i][j] = l->data[i][j] - r->data[i][j];
            return (Base*)res;
        }
        case OpMultiply: {
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = r->cols;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(r->cols * sizeof(double));
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++) {
                    double sum = 0;
                    for (int k = 0; k < l->cols; k++) sum += l->data[i][k] * r->data[k][j];
                    res->data[i][j] = sum;
                }
            return (Base*)res;
        }
        case OpDivide:
            printf("Ошибка: деление матриц не определено.\n");
            return NULL;
        default: return NULL;
    }
}

Base* CheckScalar(Base *left, Base *right, ArithmeticOperations op) {
    Scalar *l = (Scalar*)left;
    switch (right->type) {
        case TypeScalar:    return OpScalarScalar(l, (Scalar*)right, op);
        case TypeRowVector: return OpScalarRowVector(l, (RowVector*)right, op);
        case TypeColVector: return OpScalarColVector(l, (ColVector*)right, op);
        case TypeMatrix:    return OpScalarMatrix(l, (Matrix*)right, op);
        default: printf("Неизвестный тип правого операнда.\n"); return NULL;
    }
}

Base* CheckRowVector(Base *left, Base *right, ArithmeticOperations op) {
    RowVector *l = (RowVector*)left;
    switch (right->type) {
        case TypeScalar: {
            RowVector *res = (RowVector*)Create(TypeRowVector);
            res->rows = 1; res->cols = l->cols;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(l->cols * sizeof(double));
            switch (op) {
                case OpPlus:
                case OpMinus:
                    printf("Ошибка: вектор-строка и скаляр несовместимы для +/-. \n");
                    FreeBase((Base*)res); return NULL;
                case OpMultiply:
                    for (int i = 0; i < l->cols; i++) res->data[0][i] = l->data[0][i] * ((Scalar*)right)->data[0][0];
                    break;
                case OpDivide:
                    if (fabs(((Scalar*)right)->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
                    for (int i = 0; i < l->cols; i++) res->data[0][i] = l->data[0][i] / ((Scalar*)right)->data[0][0];
                    break;
                default: FreeBase((Base*)res); return NULL;
            }
            return (Base*)res;
        }
        case TypeRowVector:
            if (l->cols != ((RowVector*)right)->cols) {
                printf("Ошибка: длины вектор-строк не совпадают.\n");
                return NULL;
            }
            return OpRowVectorRowVector(l, (RowVector*)right, op);
        case TypeColVector: {
            ColVector *r = (ColVector*)right;
            if (op != OpMultiply) { printf("Ошибка: операция неприменима к строке и столбцу.\n"); return NULL; }
            if (l->cols != r->rows) { printf("Ошибка: размерности несовместимы.\n"); return NULL; }
            Scalar *res = (Scalar*)Create(TypeScalar);
            res->rows = 1; res->cols = 1;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(sizeof(double));
            res->data[0][0] = 0;
            for (int i = 0; i < l->cols; i++) res->data[0][0] += l->data[0][i] * r->data[i][0];
            return (Base*)res;
        }
        case TypeMatrix: {
            Matrix *r = (Matrix*)right;
            if (op != OpMultiply) { printf("Ошибка: операция неприменима к строке и матрице.\n"); return NULL; }
            if (l->cols != r->rows) { printf("Ошибка: размерности несовместимы.\n"); return NULL; }
            RowVector *res = (RowVector*)Create(TypeRowVector);
            res->rows = 1; res->cols = r->cols;
            res->data = (double**)malloc(sizeof(double*));
            res->data[0] = (double*)malloc(r->cols * sizeof(double));
            for (int j = 0; j < r->cols; j++) {
                double sum = 0;
                for (int k = 0; k < l->cols; k++) sum += l->data[0][k] * r->data[k][j];
                res->data[0][j] = sum;
            }
            return (Base*)res;
        }
        default: printf("Неизвестный тип правого операнда.\n"); return NULL;
    }
}

Base* CheckColVector(Base *left, Base *right, ArithmeticOperations op) {
    ColVector *l = (ColVector*)left;
    switch (right->type) {
        case TypeScalar: {
            ColVector *res = (ColVector*)Create(TypeColVector);
            res->rows = l->rows; res->cols = 1;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(sizeof(double));
            switch (op) {
                case OpPlus:
                case OpMinus:
                    printf("Ошибка: вектор-столбец и скаляр несовместимы для +/-. \n");
                    FreeBase((Base*)res); return NULL;
                case OpMultiply:
                    for (int i = 0; i < l->rows; i++) res->data[i][0] = l->data[i][0] * ((Scalar*)right)->data[0][0];
                    break;
                case OpDivide:
                    if (fabs(((Scalar*)right)->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
                    for (int i = 0; i < l->rows; i++) res->data[i][0] = l->data[i][0] / ((Scalar*)right)->data[0][0];
                    break;
                default: FreeBase((Base*)res); return NULL;
            }
            return (Base*)res;
        }
        case TypeRowVector: {
            RowVector *r = (RowVector*)right;
            if (op != OpMultiply) { printf("Ошибка: операция неприменима к столбцу и строке.\n"); return NULL; }
            if (l->rows != r->cols) { printf("Ошибка: размерности несовместимы.\n"); return NULL; }
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = r->cols;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(r->cols * sizeof(double));
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data[i][0] * r->data[0][j];
            return (Base*)res;
        }
        case TypeColVector:
            if (l->rows != ((ColVector*)right)->rows) {
                printf("Ошибка: длины вектор-столбцов не совпадают.\n");
                return NULL;
            }
            return OpColVectorColVector(l, (ColVector*)right, op);
        case TypeMatrix:
            printf("Ошибка: умножение столбца на матрицу не определено.\n");
            return NULL;
        default: printf("Неизвестный тип правого операнда.\n"); return NULL;
    }
}

Base* CheckMatrix(Base *left, Base *right, ArithmeticOperations op) {
    Matrix *l = (Matrix*)left;
    switch (right->type) {
        case TypeScalar: {
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = l->cols;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(l->cols * sizeof(double));
            switch (op) {
                case OpPlus:
                case OpMinus:
                    printf("Ошибка: матрица и скаляр несовместимы для +/-. \n");
                    FreeBase((Base*)res); return NULL;
                case OpMultiply:
                    for (int i = 0; i < l->rows; i++)
                        for (int j = 0; j < l->cols; j++)
                            res->data[i][j] = l->data[i][j] * ((Scalar*)right)->data[0][0];
                    break;
                case OpDivide:
                    if (fabs(((Scalar*)right)->data[0][0]) < EPS) { printf("Деление на ноль.\n"); FreeBase((Base*)res); return NULL; }
                    for (int i = 0; i < l->rows; i++)
                        for (int j = 0; j < l->cols; j++)
                            res->data[i][j] = l->data[i][j] / ((Scalar*)right)->data[0][0];
                    break;
                default: FreeBase((Base*)res); return NULL;
            }
            return (Base*)res;
        }
        case TypeRowVector: {
            RowVector *r = (RowVector*)right;
            if (op != OpMultiply) { printf("Ошибка: операция неприменима к матрице и строке.\n"); return NULL; }
            if (l->cols != r->cols) { printf("Ошибка: размерности несовместимы.\n"); return NULL; }
            Matrix *res = (Matrix*)Create(TypeMatrix);
            res->rows = l->rows; res->cols = r->cols;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(r->cols * sizeof(double));
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data[i][j] * r->data[0][j];
            return (Base*)res;
        }
        case TypeColVector: {
            ColVector *r = (ColVector*)right;
            if (op != OpMultiply) { printf("Ошибка: операция неприменима к матрице и столбцу.\n"); return NULL; }
            if (l->cols != r->rows) { printf("Ошибка: размерности несовместимы.\n"); return NULL; }
            ColVector *res = (ColVector*)Create(TypeColVector);
            res->rows = l->rows; res->cols = 1;
            res->data = (double**)malloc(l->rows * sizeof(double*));
            for (int i = 0; i < l->rows; i++) res->data[i] = (double*)malloc(sizeof(double));
            for (int i = 0; i < l->rows; i++) {
                double sum = 0;
                for (int k = 0; k < l->cols; k++) sum += l->data[i][k] * r->data[k][0];
                res->data[i][0] = sum;
            }
            return (Base*)res;
        }
        case TypeMatrix:
            if (op != OpMultiply && (l->rows != ((Matrix*)right)->rows || l->cols != ((Matrix*)right)->cols)) {
                printf("Ошибка: размерности матриц не совпадают.\n");
                return NULL;
            }
            if (op == OpMultiply && l->cols != ((Matrix*)right)->rows) {
                printf("Ошибка: несовместимые размерности для умножения матриц.\n");
                return NULL;
            }
            return OpMatrixMatrix(l, (Matrix*)right, op);
        default: printf("Неизвестный тип правого операнда.\n"); return NULL;
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
        default: printf("Неизвестный тип левого операнда.\n"); return;
    }

    if (res) {
        Add(list, (Item*)res);
        printf("Результат добавлен в конец списка.\n");
    }
}