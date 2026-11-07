#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "subj.h"

#define EPS 1e-9

const char * const TypeName[] = {
    "Неизвестный тип", "Скаляр", "Вектор-строка", "Вектор-столбец", "Матрица"
};

typedef struct Scalar { 
    Item *next; Item *prev; 
    TypeObjects type; 
    int rows; int cols; 
    double data; 
} Scalar;

typedef struct RowVector { 
    Item *next; Item *prev; 
    TypeObjects type; 
    int rows; int cols; 
    double data[50]; 
} RowVector;

typedef struct ColVector { 
    Item *next; Item *prev; 
    TypeObjects type; 
    int rows; int cols; 
    double data[50]; 
} ColVector;

typedef struct Matrix { 
    Item *next; Item *prev; 
    TypeObjects type; 
    int rows; int cols; 
    double data[50][50]; 
} Matrix;

Base* Create(TypeObjects type, int rows, int cols) {
    Base *p = NULL;
    switch (type) {
        case TypeScalar:    p = (Base*)calloc(1, sizeof(Scalar)); break;
        case TypeRowVector: p = (Base*)calloc(1, sizeof(RowVector)); break;
        case TypeColVector: p = (Base*)calloc(1, sizeof(ColVector)); break;
        case TypeMatrix:    p = (Base*)calloc(1, sizeof(Matrix)); break;
        default: return NULL;
    }
    if (p) {
        p->type = type;
        p->rows = rows;
        p->cols = cols;
    }
    return p;
}

void PrintScalar(Scalar *p) { printf("%.4lf\n", p->data); }
void PrintRowVector(RowVector *p) {
    for (int i = 0; i < p->cols; i++) printf("%.4lf ", p->data[i]);
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
    printf("[%s %dx%d]:\n", TypeName[base->type], base->rows, base->cols);
    switch (base->type) {
        case TypeScalar:    PrintScalar((Scalar*)base); break;
        case TypeRowVector: PrintRowVector((RowVector*)base); break;
        case TypeColVector: PrintColVector((ColVector*)base); break;
        case TypeMatrix:    PrintMatrix((Matrix*)base); break;
        default: printf("Неизвестный тип\n");
    }
}

void InputScalar(Scalar *p) { printf("Введите значение: "); scanf("%lf", &p->data); }
void InputRowVector(RowVector *p) {
    printf("Введите %d элементов: ", p->cols);
    for (int i = 0; i < p->cols; i++) scanf("%lf", &p->data[i]);
}
void InputColVector(ColVector *p) {
    printf("Введите %d элементов: ", p->rows);
    for (int i = 0; i < p->rows; i++) scanf("%lf", &p->data[i]);
}
void InputMatrix(Matrix *p) {
    printf("Введите %d элементов (построчно): ", p->rows * p->cols);
    for (int i = 0; i < p->rows; i++)
        for (int j = 0; j < p->cols; j++) scanf("%lf", &p->data[i][j]);
}

void Input(Base *base) {
    if (!base) return;
    switch (base->type) {
        case TypeScalar:    InputScalar((Scalar*)base); break;
        case TypeRowVector: InputRowVector((RowVector*)base); break;
        case TypeColVector: InputColVector((ColVector*)base); break;
        case TypeMatrix:    InputMatrix((Matrix*)base); break;
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
                Item *item1 = Remove(list, j);
                Item *item2 = Remove(list, j);
                Insert(list, item2, j);
                Insert(list, item1, j + 1);
            }
        }
    }
    printf("Сортировка завершена.\n");
}

void SearchNumber(List *list, double number) {
    if (!list) return;
    Item *item; 
    int found = 0;
    int elemIndex = 1;
    
    for (item = list->head; item != NULL; item = item->next, elemIndex++) {
        Base *b = (Base*)item;
        switch (b->type) {
            case TypeMatrix: {
                Matrix *m = (Matrix*)b;
                for (int i = 0; i < b->rows; i++)
                    for (int j = 0; j < b->cols; j++)
                        if (fabs(m->data[i][j] - number) < EPS) {
                            printf("Число %.4lf найдено в элементе #%d (%s %dx%d) на позиции [%d][%d].\n",
                                   number, elemIndex, TypeName[b->type], b->rows, b->cols, i, j);
                            found++;
                        }
                break;
            }
            case TypeRowVector: {
                RowVector *v = (RowVector*)b;
                for (int i = 0; i < b->cols; i++)
                    if (fabs(v->data[i] - number) < EPS) {
                        printf("Число %.4lf найдено в элементе #%d (%s 1x%d) на позиции [0][%d].\n",
                               number, elemIndex, TypeName[b->type], b->cols, i);
                        found++;
                    }
                break;
            }
            case TypeColVector: {
                ColVector *v = (ColVector*)b;
                for (int i = 0; i < b->rows; i++)
                    if (fabs(v->data[i] - number) < EPS) {
                        printf("Число %.4lf найдено в элементе #%d (%s %dx1) на позиции [%d][0].\n",
                               number, elemIndex, TypeName[b->type], b->rows, i);
                        found++;
                    }
                break;
            }
            case TypeScalar: {
                Scalar *s = (Scalar*)b;
                if (fabs(s->data - number) < EPS) {
                    printf("Число %.4lf найдено в элементе #%d (Скаляр).\n", number, elemIndex);
                    found++;
                }
                break;
            }
            default: break;
        }
    }
    
    if (found == 0) {
        printf("Число %.4lf не найдено ни в одном элементе списка.\n", number);
    } else {
        printf("Всего найдено вхождений: %d\n", found);
    }
}

TypeObjects GetResultType(int r, int c) {
    if (r == 1 && c == 1) return TypeScalar;
    if (r == 1) return TypeRowVector;
    if (c == 1) return TypeColVector;
    return TypeMatrix;
}

//3ур
Base* OpScalarScalar(Scalar *l, Scalar *r, ArithmeticOperations op) {
    Scalar *res = (Scalar*)Create(TypeScalar, 1, 1);
    switch (op) {
        case OpPlus:    res->data = l->data + r->data; break;
        case OpMinus:   res->data = l->data - r->data; break;
        case OpMultiply:res->data = l->data * r->data; break;
        case OpDivide:
            if (fabs(r->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
            res->data = l->data / r->data;
            break;
        default: free(res); return NULL;
    }
    return (Base*)res;
}

Base* OpScalarRowVector(Scalar *l, RowVector *r, ArithmeticOperations op) {
    RowVector *res = (RowVector*)Create(TypeRowVector, 1, r->cols);
    switch (op) {
        case OpPlus:
        case OpMinus:
            printf("Ошибка: скаляр и вектор-строка несовместимы для +/-. \n");
            free(res); return NULL;
        case OpMultiply:
            for (int i = 0; i < r->cols; i++) res->data[i] = l->data * r->data[i];
            break;
        case OpDivide:
            if (fabs(l->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
            for (int i = 0; i < r->cols; i++) {
                if (fabs(r->data[i]) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
                res->data[i] = l->data / r->data[i];
            }
            break;
        default: free(res); return NULL;
    }
    return (Base*)res;
}

Base* OpScalarColVector(Scalar *l, ColVector *r, ArithmeticOperations op) {
    ColVector *res = (ColVector*)Create(TypeColVector, r->rows, 1);
    switch (op) {
        case OpPlus:
        case OpMinus:
            printf("Ошибка: скаляр и вектор-столбец несовместимы для +/-. \n");
            free(res); return NULL;
        case OpMultiply:
            for (int i = 0; i < r->rows; i++) res->data[i] = l->data * r->data[i];
            break;
        case OpDivide:
            if (fabs(l->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
            for (int i = 0; i < r->rows; i++) {
                if (fabs(r->data[i]) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
                res->data[i] = l->data / r->data[i];
            }
            break;
        default: free(res); return NULL;
    }
    return (Base*)res;
}

Base* OpScalarMatrix(Scalar *l, Matrix *r, ArithmeticOperations op) {
    Matrix *res = (Matrix*)Create(TypeMatrix, r->rows, r->cols);
    switch (op) {
        case OpPlus:
        case OpMinus:
            printf("Ошибка: скаляр и матрица несовместимы для +/-. \n");
            free(res); return NULL;
        case OpMultiply:
            for (int i = 0; i < r->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data * r->data[i][j];
            break;
        case OpDivide:
            if (fabs(l->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
            for (int i = 0; i < r->rows; i++)
                for (int j = 0; j < r->cols; j++) {
                    if (fabs(r->data[i][j]) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
                    res->data[i][j] = l->data / r->data[i][j];
                }
            break;
        default: free(res); return NULL;
    }
    return (Base*)res;
}

Base* OpRowVectorRowVector(RowVector *l, RowVector *r, ArithmeticOperations op) {
    switch (op) {
        case OpPlus: {
            RowVector *res = (RowVector*)Create(TypeRowVector, 1, l->cols);
            for (int i = 0; i < l->cols; i++) res->data[i] = l->data[i] + r->data[i];
            return (Base*)res;
        }
        case OpMinus: {
            RowVector *res = (RowVector*)Create(TypeRowVector, 1, l->cols);
            for (int i = 0; i < l->cols; i++) res->data[i] = l->data[i] - r->data[i];
            return (Base*)res;
        }
        case OpMultiply: {
            Scalar *res = (Scalar*)Create(TypeScalar, 1, 1);
            res->data = 0;
            for (int i = 0; i < l->cols; i++) res->data += l->data[i] * r->data[i];
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
            ColVector *res = (ColVector*)Create(TypeColVector, l->rows, 1);
            for (int i = 0; i < l->rows; i++) res->data[i] = l->data[i] + r->data[i];
            return (Base*)res;
        }
        case OpMinus: {
            ColVector *res = (ColVector*)Create(TypeColVector, l->rows, 1);
            for (int i = 0; i < l->rows; i++) res->data[i] = l->data[i] - r->data[i];
            return (Base*)res;
        }
        case OpMultiply: {
            Scalar *res = (Scalar*)Create(TypeScalar, 1, 1);
            res->data = 0;
            for (int i = 0; i < l->rows; i++) res->data += l->data[i] * r->data[i];
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
            Matrix *res = (Matrix*)Create(TypeMatrix, l->rows, l->cols);
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < l->cols; j++)
                    res->data[i][j] = l->data[i][j] + r->data[i][j];
            return (Base*)res;
        }
        case OpMinus: {
            Matrix *res = (Matrix*)Create(TypeMatrix, l->rows, l->cols);
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < l->cols; j++)
                    res->data[i][j] = l->data[i][j] - r->data[i][j];
            return (Base*)res;
        }
        case OpMultiply: {
            Matrix *res = (Matrix*)Create(TypeMatrix, l->rows, r->cols);
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++) {
                    double sum = 0;
                    for (int k = 0; k < l->cols; k++)
                        sum += l->data[i][k] * r->data[k][j];
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


//2ур
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
            RowVector *res = (RowVector*)Create(TypeRowVector, 1, l->cols);
            switch (op) {
                case OpPlus:
                case OpMinus:
                    printf("Ошибка: вектор-строка и скаляр несовместимы для +/-. \n");
                    free(res); return NULL;
                case OpMultiply:
                    for (int i = 0; i < l->cols; i++) res->data[i] = l->data[i] * ((Scalar*)right)->data;
                    break;
                case OpDivide:
                    if (fabs(((Scalar*)right)->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
                    for (int i = 0; i < l->cols; i++) res->data[i] = l->data[i] / ((Scalar*)right)->data;
                    break;
                default: free(res); return NULL;
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
            if (op != OpMultiply) {
                printf("Ошибка: операция неприменима к строке и столбцу.\n");
                return NULL;
            }
            if (l->cols != r->rows) {
                printf("Ошибка: размерности несовместимы.\n");
                return NULL;
            }
            Scalar *res = (Scalar*)Create(TypeScalar, 1, 1);
            res->data = 0;
            for (int i = 0; i < l->cols; i++) res->data += l->data[i] * r->data[i];
            return (Base*)res;
        }
        case TypeMatrix: {
            Matrix *r = (Matrix*)right;
            if (op != OpMultiply) {
                printf("Ошибка: операция неприменима к строке и матрице.\n");
                return NULL;
            }
            if (l->cols != r->rows) {
                printf("Ошибка: размерности несовместимы.\n");
                return NULL;
            }
            RowVector *res = (RowVector*)Create(TypeRowVector, 1, r->cols);
            for (int j = 0; j < r->cols; j++) {
                double sum = 0;
                for (int k = 0; k < l->cols; k++) sum += l->data[k] * r->data[k][j];
                res->data[j] = sum;
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
            ColVector *res = (ColVector*)Create(TypeColVector, l->rows, 1);
            switch (op) {
                case OpPlus:
                case OpMinus:
                    printf("Ошибка: вектор-столбец и скаляр несовместимы для +/-. \n");
                    free(res); return NULL;
                case OpMultiply:
                    for (int i = 0; i < l->rows; i++) res->data[i] = l->data[i] * ((Scalar*)right)->data;
                    break;
                case OpDivide:
                    if (fabs(((Scalar*)right)->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
                    for (int i = 0; i < l->rows; i++) res->data[i] = l->data[i] / ((Scalar*)right)->data;
                    break;
                default: free(res); return NULL;
            }
            return (Base*)res;
        }
        case TypeRowVector: {
            RowVector *r = (RowVector*)right;
            if (op != OpMultiply) {
                printf("Ошибка: операция неприменима к столбцу и строке.\n");
                return NULL;
            }
            if (l->rows != r->cols) {
                printf("Ошибка: размерности несовместимы.\n");
                return NULL;
            }
            Matrix *res = (Matrix*)Create(TypeMatrix, l->rows, r->cols);
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data[i] * r->data[j];
            return (Base*)res;
        }
        case TypeColVector:
            if (l->rows != ((ColVector*)right)->rows) {
                printf("Ошибка: длины вектор-столбцов не совпадают.\n");
                return NULL;
            }
            return OpColVectorColVector(l, (ColVector*)right, op);
        case TypeMatrix: {
            Matrix *r = (Matrix*)right;
            if (op != OpMultiply) {
                printf("Ошибка: операция неприменима к столбцу и матрице.\n");
                return NULL;
            }
            if (l->rows != r->rows) {
                printf("Ошибка: размерности несовместимы.\n");
                return NULL;
            }

            ColVector *res = (ColVector*)Create(TypeColVector, l->rows, 1);
            for (int i = 0; i < l->rows; i++) res->data[i] = l->data[i] * r->data[i][0];
            return (Base*)res;
        }
        default: printf("Неизвестный тип правого операнда.\n"); return NULL;
    }
}

Base* CheckMatrix(Base *left, Base *right, ArithmeticOperations op) {
    Matrix *l = (Matrix*)left;
    switch (right->type) {
        case TypeScalar: {
            Matrix *res = (Matrix*)Create(TypeMatrix, l->rows, l->cols);
            switch (op) {
                case OpPlus:
                case OpMinus:
                    printf("Ошибка: матрица и скаляр несовместимы для +/-. \n");
                    free(res); return NULL;
                case OpMultiply:
                    for (int i = 0; i < l->rows; i++)
                        for (int j = 0; j < l->cols; j++)
                            res->data[i][j] = l->data[i][j] * ((Scalar*)right)->data;
                    break;
                case OpDivide:
                    if (fabs(((Scalar*)right)->data) < EPS) { printf("Деление на ноль.\n"); free(res); return NULL; }
                    for (int i = 0; i < l->rows; i++)
                        for (int j = 0; j < l->cols; j++)
                            res->data[i][j] = l->data[i][j] / ((Scalar*)right)->data;
                    break;
                default: free(res); return NULL;
            }
            return (Base*)res;
        }
        case TypeRowVector: {
            RowVector *r = (RowVector*)right;
            if (op != OpMultiply) {
                printf("Ошибка: операция неприменима к матрице и строке.\n");
                return NULL;
            }
            if (l->cols != r->cols) {
                printf("Ошибка: размерности несовместимы.\n");
                return NULL;
            }
            Matrix *res = (Matrix*)Create(TypeMatrix, l->rows, r->cols);
            for (int i = 0; i < l->rows; i++)
                for (int j = 0; j < r->cols; j++)
                    res->data[i][j] = l->data[i][j] * r->data[j];
            return (Base*)res;
        }
        case TypeColVector: {
            ColVector *r = (ColVector*)right;
            if (op != OpMultiply) {
                printf("Ошибка: операция неприменима к матрице и столбцу.\n");
                return NULL;
            }
            if (l->cols != r->rows) {
                printf("Ошибка: размерности несовместимы.\n");
                return NULL;
            }
            ColVector *res = (ColVector*)Create(TypeColVector, l->rows, 1);
            for (int i = 0; i < l->rows; i++) {
                double sum = 0;
                for (int k = 0; k < l->cols; k++) sum += l->data[i][k] * r->data[k];
                res->data[i] = sum;
            }
            return (Base*)res;
        }
        case TypeMatrix:
            if (l->rows != ((Matrix*)right)->rows || l->cols != ((Matrix*)right)->cols) {
                if (op != OpMultiply) {
                    printf("Ошибка: размерности матриц не совпадают.\n");
                    return NULL;
                }
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