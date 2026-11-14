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
} Base;

Base* Create(TypeObjects type);
void Input(Base *base);
void Print(Base *base);
int GetSize(Base *base);

void PrintObjects(List *list);
void SortBySize(List *list);
void SearchNumber(List *list, double number);
void Math(List *list, int idx1, int idx2, ArithmeticOperations op);

#endif