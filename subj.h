#ifndef SUBJ_H
#define SUBJ_H

#include "list.h"

typedef enum {
    TypeNone, TypeScalar, TypeRowVector, TypeColVector, TypeMatrix
} TypeObjects;

typedef enum {
    OpNone, OpPlus, OpMinus, OpMultiply, OpDivide
} ArithmeticOperations;

typedef struct Base {
    Item *next;
    Item *prev;
    TypeObjects type;
    int rows;
    int cols;
    double **data;
} Base;

Base* Create(TypeObjects type);
void Input(Base *base);
void Print(Base *base);
void FreeBase(Base *base);

void PrintList(List *list);
void SortBySize(List *list);
void SearchNumber(List *list, double number);
void Math(List *list, int idx1, int idx2, ArithmeticOperations op);

#endif