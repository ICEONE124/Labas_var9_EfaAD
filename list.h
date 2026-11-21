#ifndef LIST_H
#define LIST_H

typedef struct Item {
	struct Item *next;
	struct Item *prev;
} Item;

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
int Insert(List *list, Item *item, const int n);
void PrintList(const List *list);

#endif