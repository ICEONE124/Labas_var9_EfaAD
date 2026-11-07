#ifndef LIST_H
#define LIST_H

//мат_объедки
typedef enum {
	TYPE_SCALAR,      
	TYPE_ROW_VECTOR,  
	TYPE_COL_VECTOR,  
	TYPE_MATRIX       
} ObjectType;

//Элэмент списка с данными
typedef struct Item {
	struct Item *next;
	struct Item *prev;
	
	ObjectType type;
	int rows;
	int cols;
	double **data;     
} Item;

//Структура спи_сок
typedef struct List {
	Item *head;
	Item *tail;
} List;



void Add(List *list, Item *item);

Item* GetItem(const List *list, const int n);

Item* Remove(List *list, const int n);

void Delete(List *list, const int n);

int Count(const List *list);

void Clear(List *list);

int GetIndex(const List *list, const Item *item);

void Insert(List *list, Item *item, const int n);


Item* CreateScalar(double val);

Item* CreateVectorRow(int cols, const double *arr);

Item* CreateVectorCol(int rows, const double *arr);

Item* CreateMatrix(int rows, int cols, double **arr);

void FreeItemData(Item *item);


void SortBySize(List *list);

void SearchInList(const List *list, double target, double eps);

int AddElements(List *list, const int n1, const int n2);

int SubtractElements(List *list, const int n1, const int n2);

int MultiplyElements(List *list, const int n1, const int n2);

int DivideElements(List *list, const int n1, const int n2);

#endif