#include <stdlib.h>
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
	if (item != NULL) {
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
		if (i == item) return number;
		number++;
	}
	return -1;
}

int Insert(List *list, Item *item, const int n) {
	if (list == NULL || item == NULL) return -1;
	item->next = NULL;
	item->prev = NULL;

	Item *old_item = GetItem(list, n);
	if (old_item != NULL) {
		if (old_item == list->head) {
			list->head = item;
			item->prev = NULL;
			old_item->prev = item;
			item->next = old_item;
		} else {
			item->next = old_item;
			item->prev = old_item->prev;
			old_item->prev->next = item;
			old_item->prev = item;
		}
	} else {
		Add(list, item);
	}
	return 0;
}

void PrintList(const List *list) {
	if (list == NULL) {
		printf("List: NULL\n");
		return;
	}

	Item *i;
	int n;

	printf("List: %p, Head: %p, Tail: %p\n",
			(void*)list, (void*)list->head, (void*)list->tail);
	printf("#\tp\tprev\tnext\n");
	for (i = list->head, n = 0; i != NULL; i = i->next, n++) {
		printf("%d\t%p\t%p\t%p\n", n,
				(void*)i, (void*)i->prev, (void*)i->next);
	}
}