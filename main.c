#include <stdio.h>
#include <stdlib.h>
#include "subj.h"

const char * const Menu[] = {
    "Неизвестная команда", "Добавить элемент", "Вывести список",
    "Сортировать по размеру", "Найти число", "Арифметическая операция", "Выйти"
};

int DoMenu(const char * const s[], int max) {
    int i;
    for (i = 1; i < max; i++) printf("%d - %s\n", i, s[i]);
    printf("-> ");
    while (scanf("%d", &i) != 1) { while (getchar() != '\n'); printf("Ошибка ввода.\n-> "); }
    return (i < 1 || i >= max) ? 0 : i;
}

int main() {
    system("chcp 65001 > nul");
    List list = {NULL, NULL};
    int key, stop = 1, typeKey, idx1, idx2, opKey;
    double searchNum;
    Base *p;

    do {
        key = DoMenu(Menu, 7);
        switch (key) {
            case 0: printf("Неизвестная команда\n"); break;
            case 1:
                printf("Тип:\n1 - Скаляр\n2 - Вектор-строка\n3 - Вектор-столбец\n4 - Матрица\n-> ");
                scanf("%d", &typeKey);
                TypeObjects t = TypeNone;
                if (typeKey == 1) t = TypeScalar;
                else if (typeKey == 2) t = TypeRowVector;
                else if (typeKey == 3) t = TypeColVector;
                else if (typeKey == 4) t = TypeMatrix;
                else { printf("Неверный тип.\n"); break; }

                p = Create(t);
                if (p) { Input(p); Add(&list, (Item*)p); }
                else printf("Ошибка создания элемента.\n");
                break;
            case 2: PrintObjects(&list); break;
            case 3: SortBySize(&list); break;
            case 4:
                printf("Число для поиска: "); scanf("%lf", &searchNum);
                SearchNumber(&list, searchNum);
                break;
            case 5:
                if (Count(&list) < 2) { printf("Нужно минимум 2 элемента.\n"); break; }
                printf("Индексы (с 0): "); scanf("%d %d", &idx1, &idx2);
                printf("Операция:\n1 - +\n2 - -\n3 - *\n4 - /\n-> "); scanf("%d", &opKey);
                ArithmeticOperations op = OpNone;
                if (opKey == 1) op = OpPlus;
                else if (opKey == 2) op = OpMinus;
                else if (opKey == 3) op = OpMultiply;
                else if (opKey == 4) op = OpDivide;
                else { printf("Неверная операция.\n"); break; }
                Math(&list, idx1, idx2, op);
                break;
            case 6: stop = 0; break;
        }
        printf("\n");
    } while (stop);

    Clear(&list);
    system("pause");
    return 0;
}